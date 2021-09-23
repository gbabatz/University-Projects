#include <stdio.h> 
#include <stdlib.h> //for exit() function

#define N 330
#define M 440
#define filename "./dog_440x330.yuv"
#define file_yuv_init "./dog_init.yuv"
#define file_yuv_final "./dog_final.yuv"

#define MAX_CLR 255

//for solarize algorithm
#define THRESHOLD 128
#define OPTION 0

//for warhol algorithm
#define K 8 //warhol effect bins
//red, lime, blue, yellow, cyan, magenta, maroon, purple
unsigned const char bin_colors[K][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255},{128,0,0},{128,0,128}};
unsigned int bin_limits[K+1];
unsigned int incr = (MAX_CLR + 1)/K; 
unsigned int limit = 0;

unsigned int i,j,k;
unsigned char red_curr;
unsigned char green_curr;
unsigned char blue_curr;
unsigned char gray_curr;

unsigned int red_temp_conv;
unsigned int green_temp_conv;
unsigned int blue_temp_conv;

unsigned char y_init[N][M];
unsigned char u_init[N/2][M/2];
unsigned char v_init[N/2][M/2];

unsigned char u_init_resized[N][M]; //y does not need resize
unsigned char v_init_resized[N][M];

unsigned char y_curr;
unsigned char u_curr;
unsigned char v_curr;

unsigned char y_final[N][M];
unsigned char u_final[N/2][M/2]; //these are the final final values
unsigned char v_final[N/2][M/2];

unsigned char u_final_resized[N][M];
unsigned char v_final_resized[N][M];
unsigned int tmp_i,tmp_j;

void read_yuv_img()
{
  FILE *frame_c;
  if((frame_c=fopen(filename,"rb"))==NULL)
  {
    printf("current frame doesn't exist\n");
    exit(-1);
  }

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      y_init[i][j]=fgetc(frame_c);
    }
  }
  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      u_init[i][j]=fgetc(frame_c);
    }
  }
  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      v_init[i][j]=fgetc(frame_c);
    }
  }
  fclose(frame_c);
}

unsigned char clamp(int n){ //for Y'UV444 to RGB888 conversion. Limits the value between 0-255
    int a = 255;
    a -= n;
    a >>= 31;
    a |= n;
    n >>= 31;
    n = ~n;
    n &= a;
    return n;
}

void warhol_loop_fusion(){
  
  //~~~~~~~~Initialize bins and limits~~~~~~~~

  for(i=0;i<K+1;i++){
    bin_limits[i] = limit;
    limit = limit + incr;
  }
  i = 0;
  if(i<N){
    do{
      j = 0;
      if(j<M){
       do{

          //~~~~~~~~Perform the Conversion YUV to RGB~~~~~~~~
          y_curr = y_init[i][j];
          u_curr = u_init_resized[i][j];
          v_curr = v_init_resized[i][j];

          //Y'UV444 to RGB888 conversion
          //it exists on this field but copied the code from the last field with the code from android

          red_temp_conv = y_curr + (1.370705 * (v_curr-128));
          green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
          blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

          red_curr   = clamp(red_temp_conv);
          green_curr =  clamp(green_temp_conv);
          blue_curr  = clamp(blue_temp_conv);	     

          //~~~~~~~~Convert RGB to GrayScale~~~~~~~~

          gray_curr = 0.2126*red_curr + 0.7152*green_curr + 0.0722*blue_curr;

          //~~~~~~~~Execute the Algorithm~~~~~~~~

          for(k=0;k<K;k++){
            if(gray_curr >= bin_limits[k] && gray_curr <= bin_limits[k+1]){

               red_curr = bin_colors[k][0];
               green_curr = bin_colors[k][1];
               blue_curr = bin_colors[k][2];
               break;  //break when you find the bin matching
            }
          }

          //~~~~~~~~Perform the Conversion RGB to YUV~~~~~~~~

          //Y'UV444 to RGB888 conversion
          //ON older non-SIMD architectures WORKS
          y_final[i][j] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
          u_final_resized[i][j] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
          v_final_resized[i][j] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;

        j++;
        }while(j<M);
      }
    i++;
    }while(i<N);
  }
}

void solarize_loop_fusion(){

  if (OPTION){
    i = 0;
    if(i<N){
      do{
        j = 0;
        if(j<M){
         do{
          
          //~~~~~~~~Perform the Conversion YUV to RGB~~~~~~~~
          y_curr = y_init[i][j];
          u_curr = u_init_resized[i][j];
          v_curr = v_init_resized[i][j];

          //Y'UV444 to RGB888 conversion
          //it exists on this field but copied the code from the last field with the code from android

          red_temp_conv = y_curr + (1.370705 * (v_curr-128));
          green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
          blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

          red_curr   = clamp(red_temp_conv);
          green_curr =  clamp(green_temp_conv);
          blue_curr  = clamp(blue_temp_conv);	     

          //~~~~~~~~Execute the Algorithm~~~~~~~~
          if(red_curr < THRESHOLD){
            red_curr = MAX_CLR - red_curr;
          }

          if(green_curr < THRESHOLD){
            green_curr = MAX_CLR - green_curr;
          }

          if(blue_curr < THRESHOLD){
            blue_curr = MAX_CLR - blue_curr;
          }

          //~~~~~~~~Perform the Conversion RGB to YUV~~~~~~~~

          //Y'UV444 to RGB888 conversion
          //ON older non-SIMD architectures WORKS
          y_final[i][j] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
          u_final_resized[i][j] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
          v_final_resized[i][j] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;
      
          j++;
          }while(j<M);
        }
      i++;
      }while(i<N);
    }
 }else{
  i = 0;
  if(i<N){
    do{
      j = 0;
      if(j<M){
        do{

          //~~~~~~~~Perform the Conversion YUV to RGB~~~~~~~~
          y_curr = y_init[i][j];
          u_curr = u_init_resized[i][j];
          v_curr = v_init_resized[i][j];

          //Y'UV444 to RGB888 conversion
          //it exists on this field but copied the code from the last field with the code from android

          red_temp_conv = y_curr + (1.370705 * (v_curr-128));
          green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
          blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

          red_curr   = clamp(red_temp_conv);
          green_curr =  clamp(green_temp_conv);
          blue_curr  = clamp(blue_temp_conv);	     

          //~~~~~~~~Execute the Algorithm~~~~~~~~
          if(red_curr > THRESHOLD){
            red_curr = MAX_CLR - red_curr;

          }

          if(green_curr > THRESHOLD){
            green_curr = MAX_CLR - green_curr;
          }

          if(blue_curr > THRESHOLD){
            blue_curr = MAX_CLR - blue_curr;
          }

          //~~~~~~~~Perform the Conversion RGB to YUV~~~~~~~~

          //Y'UV444 to RGB888 conversion
          //ON older non-SIMD architectures WORKS
          y_final[i][j] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
          u_final_resized[i][j] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
          v_final_resized[i][j] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;


			j++;
			}while(j<M);
		  }
		i++;
		}while(i<N);
	 }
 }
}


//these functions have 'img' on the declaration to know that it creates binary file not txt file with the elements
void write_yuv_init_img_full()
{
  FILE *frame_yuv;
  frame_yuv=fopen(file_yuv_init,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(y_init[i][j],frame_yuv);
    }
  }

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(u_init[i][j],frame_yuv);
    }
  }

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(v_init[i][j],frame_yuv);
    }
  }
  fclose(frame_yuv);
}

void write_yuv_final_img_full()
{
  FILE *frame_yuv;
  frame_yuv=fopen(file_yuv_final,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(y_final[i][j],frame_yuv);
    }
  }

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(u_final[i][j],frame_yuv);
      //fputc(u_final_resized[i][j],frame_yuv);
    }
  }

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(v_final[i][j],frame_yuv);
      //fputc(v_final_resized[i][j],frame_yuv);
    }
  }
  fclose(frame_yuv);
}

void resize_up_uv(){

  for(i=0;i<N/2;i++){
    tmp_i = 2*i;
    for(j=0;j<M/2;j++){
      
      tmp_j = 2*j; 
      u_curr = u_init[i][j];
      v_curr = v_init[i][j]; 
      
      u_init_resized[tmp_i][tmp_j] = u_curr;  
      u_init_resized[tmp_i][tmp_j+1] = u_curr;  
      u_init_resized[tmp_i+1][tmp_j] = u_curr;  
      u_init_resized[tmp_i+1][tmp_j+1] = u_curr;  

      v_init_resized[tmp_i][tmp_j] = v_curr;  
      v_init_resized[tmp_i][tmp_j+1] = v_curr;  
      v_init_resized[tmp_i+1][tmp_j] = v_curr;  
      v_init_resized[tmp_i+1][tmp_j+1] = v_curr;  

    }
  }
}

void resize_down_uv(){ //you could aquire all the elements and extract a mean value to append

  for(i=0;i<N;i+=2){
    tmp_i = i/2;
    for(j=0;j<M;j+=2){
      
      tmp_j = j/2; 
      u_curr = u_final_resized[i][j];
      v_curr = v_final_resized[i][j]; 
      
      u_final[tmp_i][tmp_j] = u_curr;  
      v_final[tmp_i][tmp_j] = v_curr;  

    }
  }
}

/* ~~~~~Available functions~~~~~~

  read_yuv_img();  fills the yuv_init tables NEEDED

  write_yuv_init_img_full();  writes the yuv image from yuv_init

  write_yuv_final_img_full();  writes the yuv image from yuv_final

  resize_up_uv(); fills the uv_init_resized tables NEEDED
  
  resize_down_uv(); fills the uv_final tables NEEDED

  clamp(); clamps values between 0-255 NEEDED

  warhol_loop_fusion(); fills the rgb_final

  solarize_loop_fusion(); fills the rgb_final

*/


int main(){
  //~~~~~~READ IMAGE~~~~~~
  read_yuv_img(); //updates yuv init array
  resize_up_uv(); 
  
  //write_yuv_init_to_txt();

  //~~~~~~PERFORM ALGORITHM~~~~~~
  //warhol_loop_fusion();
  solarize_loop_fusion();

  //~~~~~~WRITE IMAGE~~~~~~
  resize_down_uv(); //yuv final tables loaded
  write_yuv_final_img_full(); //export final yuv to img 
  //write_yuv_init_img_full();
  
  printf("Process Done\n");
  system("pause");
  return 0;
}
