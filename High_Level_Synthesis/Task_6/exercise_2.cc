#include <iostream>
#include <ac_int.h>

const int N = 10;
const int M = 10;
typedef ac_int<8> dtype;
//typedef int dtype;
//typedef float dtype;

void func(dtype img[N][M],dtype out[N][M] ){
    // scan the image row by row
    // N = #row, M = #col
    ROW:for (int i = 0; i < N; ++i) {
        // scan each row pixel by pixel from left to right
        COL:for (int j = 0; j < M; j++) {
        // get values of the pixels in the kernel
        dtype p1 = (j>1) ? img[i][j-2]: (dtype)0;
        dtype p2 = (j>0) ? img[i][j-1]: (dtype)0;
        dtype p3 = img[i][j];
        dtype p4 = (j<M-1) ? img[i][j+1]: (dtype)0 ;
        dtype p5 = (j<M-2) ? img[i][j+2]: (dtype)0 ;
        // compute the mean
        out[i][j] = (p1 + p2 + p3 + p4 + p5) / 5;
        }
    }
}

void filter(dtype img[N][M],dtype out[N][M]){
    dtype kernel[5]; 
    int k;
    ROW:for (int i = 0; i < N; ++i) {

        k = 0;
        kernel[0] = 0;
        kernel[1] = 0;
        kernel[2] = img[i][0];
        kernel[3] = img[i][1];
        kernel[4] = img[i][2];

        out[i][0] = (kernel[2] + kernel[3] + kernel[4]) / 5;

        COL:for (int j = 1; j < M; j++) {

          kernel[k] = (j>=M-2)?(dtype)0:img[i][j+2];

          // compute the mean
          out[i][j] = (kernel[0] + kernel[1] + kernel[2] + kernel[3] + kernel[4]) / 5;
          
          //circular buffer implementation
          k = (k==4)?(dtype)0:(dtype)(k+1);

          }
    }
}


int main(){
    /*
    dtype img[N][M] = {1,2,3,1,2,1,3,2,1};
    dtype out[N][M]; 

    func(img,out);
//    filter(img,out);

    for(int i=0;i<M;i++){
        std::cout << out[0][i] << "\n";
    }
*/

  return 0;
}
