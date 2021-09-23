#include <iostream>
#include "ac_int.h"

#include "mc_scverify.h"
  
const int N = 400;
const int M = 400;

const int DISCRETE_VALUES = 3;
const int BITS = ac::log2_ceil<DISCRETE_VALUES>::val;
typedef ac_int<BITS,false> dtype; 
  
const int LUT_DIM1 = DISCRETE_VALUES;
const int LUT_DIM2 = 4*(DISCRETE_VALUES-1) + 1; // 4 because we use 4 neighbors

/*
  local_kernel indexing match on a 3x3 space 
  - 2 3
  1 0 4
  - 6 5
*/

#pragma hls_design top
class RunCA{
  private:
    dtype local_kernel[7]; //simulates cross kernel 
    dtype row_buffers0[M+2];
    dtype row_buffers1[M+2];
    
  public:
    RunCA(){} 

    // repeating the borders
    int clip(int current,int max){
     if(current < 0){
        return 0;
      }else if(current > max){
        return max;
      }else{
        return current;
      }
    }

    dtype apply_rule_custom(dtype rule_lut[LUT_DIM1][LUT_DIM2]){
      ac_int<BITS+2,false> sum;   
      dtype cell = local_kernel[0];
      sum = local_kernel[1] + local_kernel[2] + local_kernel[4] + local_kernel[6];

      return rule_lut[cell][sum];
    }

    #pragma hls_design interface
    void CCS_BLOCK(compute_generation)(dtype ca[N][M], dtype rule_lut[LUT_DIM1][LUT_DIM2]){ // can update any ca table, given a rule as as lut 
      
      ROWS:for(int i=-1;i<N+1;i++){
        COLS:for(int j=-1;j<M+1;j++){

          local_kernel[1] = local_kernel[0];

          local_kernel[2] = local_kernel[3];
          local_kernel[0] = local_kernel[4];
          local_kernel[6] = local_kernel[5];

          local_kernel[3] = row_buffers0[j+1];
          local_kernel[4] = row_buffers1[j+1];

          row_buffers0[j+1] = row_buffers1[j+1];
          
          // handle invalid (for the image) indices
          if( (i<0) || (i>N-1) || (j<0) || (j>M-1) ){
            local_kernel[5] = row_buffers1[j+1] = ca[clip(i,N-1)][clip(j,M-1)];           
          }else{
            local_kernel[5] = row_buffers1[j+1] = ca[i][j];
          }

          // compute the output after a given moment
          if((i>0 && i<=N) && (j>0 && j<=M)){
            ca[i-1][j-1] = apply_rule_custom(rule_lut);
          }
        } 
      }
    }
};
