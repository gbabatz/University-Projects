#include <stdio.h> 
#include <stdlib.h>

//#define N 280 /* frame dimension for QCIF format */
//#define M 420 /* frame dimension for QCIF format */

#define N 330 /* frame dimension for QCIF format */
#define M 440 /* frame dimension for QCIF format */

#define filename "./dog_440x330.yuv"
#define file_y_init "./dog_init.y"
#define file_u_init "./dog_init.u"
#define file_v_init "./dog_init.v"
#define file_y_final "./dog_final.y"
#define file_u_final "./dog_final.u"
#define file_v_final "./dog_final.v"
#define file_yuv_init "./dog_init.yuv"
#define file_yuv_final "./dog_final.yuv"

//for warhol algorithm
#define MAX_CLR 255
#define K 8 //warhol effect bins

//for solarize algorithm
#define THRESHOLD 128
#define OPTION 0

//red, lime, blue, yellow, cyan, magenta, maroon, purple
int bin_colors[K][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255},{128,0,0},{128,0,128}};
int bin_limits[K+1];

int i,j,k;
unsigned char red_init[N][M];
unsigned char green_init[N][M];
unsigned char blue_init[N][M];

unsigned char red_curr;
unsigned char green_curr;
unsigned char blue_curr;

unsigned int red_temp_conv;
unsigned int green_temp_conv;
unsigned int blue_temp_conv;

unsigned char red_final[N][M];
unsigned char green_final[N][M];
unsigned char blue_final[N][M];

unsigned char gray[N][M];
unsigned char gray_curr;

int y_init[N][M];
int u_init[N/2][M/2];
int v_init[N/2][M/2];

int u_init_resized[N][M]; //y does not need resize
int v_init_resized[N][M];

int y_curr;
int u_curr;
int v_curr;

int y_final[N][M];
int u_final[N/2][M/2]; //these are the final final values
int v_final[N/2][M/2];

int u_final_resized[N][M];
int v_final_resized[N][M];
int tmp_i;
int tmp_j;

//	partial unrolling - 2:
void rgb_to_grayscale(){
  for(i=0;i<N;i+=2){
    for(j=0;j<M;j+=2){
      gray[i][j] = 0.2126*red_init[i][j] + 0.7152*green_init[i][j] + 0.0722*blue_init[i][j];
      gray[i][j+1] = 0.2126*red_init[i][j+1] + 0.7152*green_init[i][j+1] + 0.0722*blue_init[i][j+1];
      gray[i+1][j] = 0.2126*red_init[i+1][j] + 0.7152*green_init[i+1][j] + 0.0722*blue_init[i+1][j];
      gray[i+1][j+1] = 0.2126*red_init[i+1][j+1] + 0.7152*green_init[i+1][j+1] + 0.0722*blue_init[i+1][j+1];
    }
  }
}

void solarize3(){
//option = 1 pixel LESS than threshold
//option = 0 pixel GREATER than threshold

  if(OPTION){

//	partial unrolling  2:
    for(i=0;i<N;i+=2){
      for(j=0;j<M;j+=2){
      	
/*	~ ~ ~for pixel [i][j]~ ~ ~ 	*/
         red_curr = red_init[i][j];
         green_curr = green_init[i][j];
         blue_curr = blue_init[i][j];

         if(red_curr < THRESHOLD){
            red_final[i][j] = MAX_CLR - red_curr;

         }else{
            red_final[i][j] = red_curr;

         }

         if(green_curr < THRESHOLD){
            green_final[i][j] = MAX_CLR - green_curr;

         }else{
            green_final[i][j] = green_curr;

         }

         if(blue_curr < THRESHOLD){
            blue_final[i][j] = MAX_CLR - blue_curr;
         }else{
            blue_final[i][j] = blue_curr;

         }
              
/*	~ ~ ~for pixel [i][j+1]~ ~ ~ 	*/
         red_curr = red_init[i][j+1];
         green_curr = green_init[i][j+1];
         blue_curr = blue_init[i][j+1];

         if(red_curr < THRESHOLD){
            red_final[i][j+1] = MAX_CLR - red_curr;

         }else{
            red_final[i][j+1] = red_curr;

         }

         if(green_curr < THRESHOLD){
            green_final[i][j+1] = MAX_CLR - green_curr;

         }else{
            green_final[i][j+1] = green_curr;

         }

         if(blue_curr < THRESHOLD){
            blue_final[i][j+1] = MAX_CLR - blue_curr;  
         }else{
            blue_final[i][j+1] = blue_curr;

         }

/*	~ ~ ~for pixel [i+1][j]~ ~ ~ 	*/
         red_curr = red_init[i+1][j];
         green_curr = green_init[i+1][j];
         blue_curr = blue_init[i+1][j];

         if(red_curr < THRESHOLD){
            red_final[i+1][j] = MAX_CLR - red_curr;

         }else{
            red_final[i+1][j] = red_curr;

         }

         if(green_curr < THRESHOLD){
            green_final[i+1][j] = MAX_CLR - green_curr;

         }else{
            green_final[i+1][j] = green_curr;

         }

         if(blue_curr < THRESHOLD){
            blue_final[i+1][j] = MAX_CLR - blue_curr;
         }else{
            blue_final[i+1][j] = blue_curr;

         }
  
/*	~ ~ ~for pixel [i+1][j+1]~ ~ ~ 	*/
         red_curr = red_init[i+1][j+1];
         green_curr = green_init[i+1][j+1];
         blue_curr = blue_init[i+1][j+1];

         if(red_curr < THRESHOLD){
            red_final[i+1][j+1] = MAX_CLR - red_curr;

         }else{
            red_final[i+1][j+1] = red_curr;

         }

         if(green_curr < THRESHOLD){
            green_final[i+1][j+1] = MAX_CLR - green_curr;

         }else{
            green_final[i+1][j+1] = green_curr;

         }

         if(blue_curr < THRESHOLD){
            blue_final[i+1][j+1] = MAX_CLR - blue_curr;
         }else{
            blue_final[i+1][j+1] = blue_curr;

         }    
      }
    }

  }else{
//	partial unrolling 2 :
    for(i=0;i<N;i+=2){
      for(j=0;j<M;j+=2){
      	
/*	~ ~ ~for pixel [i][j]~ ~ ~ 	*/
         red_curr = red_init[i][j];
         green_curr = green_init[i][j];
         blue_curr = blue_init[i][j];

         if(red_curr > THRESHOLD){
            red_final[i][j] = MAX_CLR - red_curr;

         }else{
            red_final[i][j] = red_curr;

         }

         if(green_curr > THRESHOLD){
            green_final[i][j] = MAX_CLR - green_curr;

         }else{
            green_final[i][j] = green_curr;

         }

         if(blue_curr > THRESHOLD){
            blue_final[i][j] = MAX_CLR - blue_curr;

         }else{
            blue_final[i][j] = blue_curr;

         }

/*	~ ~ ~for pixel [i][j+1]~ ~ ~ 	*/
         red_curr = red_init[i][j+1];
         green_curr = green_init[i][j+1];
         blue_curr = blue_init[i][j+1];

         if(red_curr > THRESHOLD){
            red_final[i][j+1] = MAX_CLR - red_curr;

         }else{
            red_final[i][j+1] = red_curr;

         }

         if(green_curr > THRESHOLD){
            green_final[i][j+1] = MAX_CLR - green_curr;

         }else{
            green_final[i][j+1] = green_curr;

         }

         if(blue_curr > THRESHOLD){
            blue_final[i][j+1] = MAX_CLR - blue_curr;

         }else{
            blue_final[i][j+1] = blue_curr;

         }

/*	~ ~ ~for pixel [i+1][j]~ ~ ~ 	*/
         red_curr = red_init[i+1][j];
         green_curr = green_init[i+1][j];
         blue_curr = blue_init[i+1][j];

         if(red_curr > THRESHOLD){
            red_final[i+1][j] = MAX_CLR - red_curr;

         }else{
            red_final[i+1][j] = red_curr;

         }

         if(green_curr > THRESHOLD){
            green_final[i+1][j] = MAX_CLR - green_curr;

         }else{
            green_final[i+1][j] = green_curr;

         }

         if(blue_curr > THRESHOLD){
            blue_final[i+1][j] = MAX_CLR - blue_curr;

         }else{
            blue_final[i+1][j] = blue_curr;

         }
                    
/*	~ ~ ~for pixel [i+1][j+1]~ ~ ~ 	*/
         red_curr = red_init[i+1][j+1];
         green_curr = green_init[i+1][j+1];
         blue_curr = blue_init[i+1][j+1];

         if(red_curr > THRESHOLD){
            red_final[i+1][j+1] = MAX_CLR - red_curr;

         }else{
            red_final[i+1][j+1] = red_curr;

         }

         if(green_curr > THRESHOLD){
            green_final[i+1][j+1] = MAX_CLR - green_curr;

         }else{
            green_final[i+1][j+1] = green_curr;

         }

         if(blue_curr > THRESHOLD){
            blue_final[i+1][j+1] = MAX_CLR - blue_curr;

         }else{
            blue_final[i+1][j+1] = blue_curr;

         }         
  
      }
    }
  }
}

void warhol(){
  
  int incr = (MAX_CLR + 1)/K; 
  int limit = 0;

  for(i=0;i<K+1;i+=2){
    bin_limits[i] = limit;
    limit = limit + incr;
    bin_limits[i+1] = limit;
    limit = limit + incr;
  }

//basic algorithm :

//  partial unrolling 2 :
  for(i=0;i<N;i+=2){
    for(j=0;j<M;j+=2){
    	
/*	~ ~ ~for pixel [i][j]~ ~ ~ 	*/
      gray_curr = gray[i][j];

      for(k=0;k<K;k++){
        if(gray_curr >= bin_limits[k] && gray_curr <= bin_limits[k+1]){

           red_final[i][j] = bin_colors[k][0];
           green_final[i][j] = bin_colors[k][1];
           blue_final[i][j] = bin_colors[k][2];
           break;  //break when you find the bin matching
        }   
      }
        
/*	~ ~ ~for pixel [i][j+1]~ ~ ~ 	*/        
      gray_curr = gray[i][j+1];

      for(k=0;k<K;k++){
        if(gray_curr >= bin_limits[k] && gray_curr <= bin_limits[k+1]){

           red_final[i][j+1] = bin_colors[k][0];
           green_final[i][j+1] = bin_colors[k][1];
           blue_final[i][j+1] = bin_colors[k][2];
           break;  //break when you find the bin matching
        }   
      }
      
/*	~ ~ ~for pixel [i+1][j]~ ~ ~ 	*/
      gray_curr = gray[i+1][j];

      for(k=0;k<K;k++){
        if(gray_curr >= bin_limits[k] && gray_curr <= bin_limits[k+1]){

           red_final[i+1][j] = bin_colors[k][0];
           green_final[i+1][j] = bin_colors[k][1];
           blue_final[i+1][j] = bin_colors[k][2];
           break;  //break when you find the bin matching
        }   
      }

/*	~ ~ ~for pixel [i+1][j+1]~ ~ ~ 	*/        
      gray_curr = gray[i+1][j+1];

      for(k=0;k<K;k++){
        if(gray_curr >= bin_limits[k] && gray_curr <= bin_limits[k+1]){

           red_final[i+1][j+1] = bin_colors[k][0];
           green_final[i+1][j+1] = bin_colors[k][1];
           blue_final[i+1][j+1] = bin_colors[k][2];
           break;  //break when you find the bin matching
        }   
      }

    }
  }
}


void no_algorithm(){ //updates the final tables without change..helps to see the conversion

  for(i=0;i<N;i++){
    for(j=0;j<M;j++){
      red_final[i][j] = red_init[i][j];
      green_final[i][j] = green_init[i][j];
      blue_final[i][j] = blue_init[i][j];
    }
  }

}

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

//these functions have 'img' on the declaration to know that it creates binary file not txt file with the elements
//which we actually dont need since we can directly check yuv images with matlab or yuvplayer.exe

void write_yuv_init_img_splitted() //creates 3 separate files of each yuv channel for initial photo
{
  FILE *frame_y,*frame_u, *frame_v;
  frame_y=fopen(file_y_init,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(y_init[i][j],frame_y);
    }
  }
  fclose(frame_y);
  frame_u=fopen(file_u_init,"wb");

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(u_init[i][j],frame_u); 
    }
  }
  fclose(frame_u);
  frame_v=fopen(file_v_init,"wb");

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(v_init[i][j],frame_v);
    }
  }
  fclose(frame_v);
}

void write_yuv_final_img_splitted() //creates 3 separate files of each yuv channel for final photo
{

  FILE *frame_y,*frame_u, *frame_v;
  frame_y=fopen(file_y_final,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(y_final[i][j],frame_y);
    }
  }
  fclose(frame_y);
  frame_u=fopen(file_u_final,"wb");

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(u_final[i][j],frame_u);
    }
  }
  fclose(frame_u);
  frame_v=fopen(file_v_final,"wb");

  for(i=0;i<N/2;i++)
  {
    for(j=0;j<M/2;j++)
    {
      fputc(v_final[i][j],frame_v);
    }
  }
  fclose(frame_v);
}

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

void resize_down_uv(){

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

void yuv_to_rgb() {

// conversion from YUV to RGB

//Y'UV444 to RGB888 conversion
//it exists on this field but copied the code from the last field with the code from android
  
//	partial unrolling 2:
  for(i=0;i<N;i+=2){
    for(j=0;j<M;j+=2){
    	
/*	~ ~ ~for pixel [i][j]~ ~ ~ 	*/
      y_curr = y_init[i][j];
      u_curr = u_init_resized[i][j];
      v_curr = v_init_resized[i][j];

      red_temp_conv = y_curr + (1.370705 * (v_curr-128));
      green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
      blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

      red_init[i][j]   = clamp(red_temp_conv);
      green_init[i][j] =  clamp(green_temp_conv);
      blue_init[i][j]  = clamp(blue_temp_conv);	     

/*	~ ~ ~for pixel [i][j+1]~ ~ ~ 	*/
      y_curr = y_init[i][j+1];
      u_curr = u_init_resized[i][j+1];
      v_curr = v_init_resized[i][j+1];

      red_temp_conv = y_curr + (1.370705 * (v_curr-128));
      green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
      blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

      red_init[i][j+1]   = clamp(red_temp_conv);
      green_init[i][j+1] =  clamp(green_temp_conv);
      blue_init[i][j+1]  = clamp(blue_temp_conv);

/*	~ ~ ~for pixel [i+1][j]~ ~ ~ 	*/
      y_curr = y_init[i+1][j];
      u_curr = u_init_resized[i+1][j];
      v_curr = v_init_resized[i+1][j];

      red_temp_conv = y_curr + (1.370705 * (v_curr-128));
      green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
      blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

      red_init[i+1][j]   = clamp(red_temp_conv);
      green_init[i+1][j] =  clamp(green_temp_conv);
      blue_init[i+1][j]  = clamp(blue_temp_conv);

/*	~ ~ ~for pixel [i+1][j+1]~ ~ ~ 	*/
      y_curr = y_init[i+1][j+1];
      u_curr = u_init_resized[i+1][j+1];
      v_curr = v_init_resized[i+1][j+1];

      red_temp_conv = y_curr + (1.370705 * (v_curr-128));
      green_temp_conv = y_curr - (0.698001 * (v_curr-128)) - (0.337633 * (u_curr-128));
      blue_temp_conv = y_curr + (1.732446 * (u_curr-128));

      red_init[i+1][j+1]   = clamp(red_temp_conv);
      green_init[i+1][j+1] =  clamp(green_temp_conv);
      blue_init[i+1][j+1]  = clamp(blue_temp_conv);

	  }
  }
  
}

void rgb_to_yuv(){

//Y'UV444 to RGB888 conversion
//ON older non-SIMD architectures WORKS

//	partial unrolling 2:
  for(i=0;i<N;i+=2){
    for(j=0;j<M;j+=2){
    
/*	~ ~ ~for pixel [i][j]~ ~ ~ 	*/
      red_curr = red_final[i][j];
      green_curr = green_final[i][j];
      blue_curr = blue_final[i][j];

      y_final[i][j] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
      u_final_resized[i][j] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
      v_final_resized[i][j] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;

/*	~ ~ ~for pixel [i][j+1]~ ~ ~ 	*/
      red_curr = red_final[i][j+1];
      green_curr = green_final[i][j+1];
      blue_curr = blue_final[i][j+1];

      y_final[i][j+1] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
      u_final_resized[i][j+1] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
      v_final_resized[i][j+1] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;

/*	~ ~ ~for pixel [i+1][j]~ ~ ~ 	*/
      red_curr = red_final[i+1][j];
      green_curr = green_final[i+1][j];
      blue_curr = blue_final[i+1][j];

      y_final[i+1][j] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
      u_final_resized[i+1][j] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
      v_final_resized[i+1][j] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;

/*	~ ~ ~for pixel [i+1][j+1]~ ~ ~ 	*/
      red_curr = red_final[i+1][j+1];
      green_curr = green_final[i+1][j+1];
      blue_curr = blue_final[i+1][j+1];

      y_final[i+1][j+1] = ((66*red_curr + 129*green_curr + 25*blue_curr + 128) >> 8) + 16;
      u_final_resized[i+1][j+1] =((-38*red_curr - 74*green_curr + 112*blue_curr + 128) >> 8) + 128;
      v_final_resized[i+1][j+1] =((112*red_curr - 94*green_curr - 18*blue_curr + 128) >> 8) + 128;
	  }
  }
}

/* ~~~~~Available functions~~~~~~

  read_yuv_img();  fills the yuv_init tables NEEDED

  write_yuv_init_img_splitted(); writes out each channel of yuv_init tables

  write_yuv_final_img_splitted(); writes out each channel of yuv_final tables

  write_yuv_init_img_full();  writes the yuv image from yuv_init

  write_yuv_final_img_full();  writes the yuv image from yuv_final

  resize_up_uv(); fills the uv_init_resized tables NEEDED
  
  resize_down_uv(); fills the uv_final tables NEEDED

  clamp(); clamps values between 0-255 NEEDED

  yuv_to_rgb(); fills the red/green/blue_init tables; NEEDED

  rgb_to_yuv(); fills the y/u_resized/v_resized_final tables; NEEDED

  rgb_to_grayscale(); fills the gray table NEEDED for warhol()

  warhol(); fills the rgb_final

  solarize3(); fills the rgb_final

  no_algorithm; fills the rgb_final helps to check if conversion done correct

*/


int main(){

  //~~~~~~READ IMAGE~~~~~~
  read_yuv_img(); //updates yuv init array
  //write_yuv_init_img_full();
  resize_up_uv(); //now we can do yuv to rgb conversion
  yuv_to_rgb(); //rgb_init tables loaded

  //~~~~~~PERFORM ALGORITHM~~~~~~

  //no_algorithm();
//  solarize3(); //apply algoritm rgb_final tables loaded

  rgb_to_grayscale(); //updates gray array
  warhol();

  //~~~~~~WRITE IMAGE~~~~~~

  rgb_to_yuv(); //y_final table and uv_final_resized loaded
  //need to resize down uv channels
  resize_down_uv(); //yuv final tables loaded
  write_yuv_final_img_full(); //export final yuv to img 
//  write_yuv_final_img_splitted();
  
  printf("Process Done\n");
  system("pause");
  return 0;
}
