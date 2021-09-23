#ifndef COMPUTE_ROW_SUM_H
#define COMPUTE_ROW_SUM_H

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "mc_scverify.h"

static const int N = 5;
static const int M = 3;

#pragma hls_design top
class ComputeRowSum {
private:
public:
  ComputeRowSum(){};

#pragma hls_design interface
void CCS_BLOCK(compute_row_sum)(short a[N][M], short row_sum[N]) {
  
  short temp_sum;
  ROWS:for (int i=0; i < N; i++) {
      temp_sum = 0;
    COLS:for (int j=0; j < M; j++) {
        temp_sum += a[i][j];
    }
    row_sum[i] = temp_sum;
  }
}

};

#endif


