#include <iostream>
#include "ac_int.h"
#include <cstdlib>
#include <ctime>
  
#include "ca_module.h" 

const int GENERATIONS = 200;
bool VERBOSE = 0; // set verbose output

class TestCA{
  private:
    dtype state[5];

  public:
    TestCA(){}
  
    dtype apply_rule(dtype rule_lut[LUT_DIM1][LUT_DIM2]){
      ac_int<BITS+2,false> sum;   
      dtype cell = state[0];
      sum = state[1] + state[2] + state[3] + state[4];

      return rule_lut[cell][sum];
    } 

    void compute_generation(dtype table_a[N][M],dtype rule_lut[LUT_DIM1][LUT_DIM2]){
      dtype new_table[N][M];

      for (int i=0;i<N;i++){
        for(int j=0;j<M;j++){		

          // repeating the borders
          state[0] = table_a[i][j]; 
          state[1] = (j>0)?table_a[i][j-1]:table_a[i][0]; 
          state[2] = (i>0)?table_a[i-1][j]:table_a[0][j]; 
          state[3] = (j<M-1)?table_a[i][j+1]:table_a[i][M-1]; 
          state[4] = (i<N-1)?table_a[i+1][j]:table_a[N-1][j]; 
       
          new_table[i][j] = apply_rule(rule_lut);
        }
      }

       for (int i=0;i<N;i++){
        for(int j=0;j<M;j++){		
          table_a[i][j] = new_table[i][j];
        }
      }
    }   
 
};

void print_table_symbols(dtype table[N][M],char symbols[10], bool turn){ // turn 1 for test and 0 for groundtruth

  std::cout << (turn?"Test:":"GroundTruth:") << "\n";

  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      std::cout << symbols[table[i][j]] << " "; 
    }
    std::cout << "\n";  
  }
}

void print_table_simple(dtype table[N][M], bool turn){ // turn 1 for test and 0 for groundtruth

  std::cout << (turn?"Test:":"GroundTruth:") << "\n";

  for (int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      std::cout << table[i][j] << " ";
    }
    std::cout << "\n";  
  }
}

bool evaluate_generation(dtype groundtruth[N][M],dtype table[N][M]){
  
  bool correct = true; // pretend its correct until a mistake is found
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){

      if(table[i][j] != groundtruth[i][j]){ // after the mistake found then correct = false
        correct = false;
        std::cout << "False at row: " << i << ", col: " << j << "\n";
        break; 
      }
    }      

    if(!correct){ // break from the outer loop if mistake is found 
      break;
    }
  }

  return correct; // return if generation is correct or not
}

void generate_random_lut(dtype lut[LUT_DIM1][LUT_DIM2]){
    for(int i=0;i<LUT_DIM1;i++){
      for(int j=0;j<LUT_DIM2;j++){
        lut[i][j] = std::rand() % DISCRETE_VALUES;
      }
    } 
}

CCS_MAIN(int argc, char* argv[]){

  std::srand(std::time(NULL)); 
  //std::srand(0);

  char avail_symbols[10] = {' ','+','X','U','?','&','<','^','>','T'};

  dtype ca_table_test[N][M];
  dtype ca_table[N][M];   
  dtype lut[LUT_DIM1][LUT_DIM2];

 /* //test for a given lut 
  lut[0][0] = 0;
  lut[1][0] = 1;
  lut[0][1] = 1;
  lut[1][1] = 1;
  lut[0][2] = 0;
  lut[1][2] = 1;
  lut[0][3] = 1;
  lut[1][3] = 1;
  lut[0][4] = 1;
  lut[1][4] = 0;
*/  
  // initialize tables with zero
  for (int k=0;k<N;k++){
    for(int j=0;j<M;j++){
      ca_table_test[k][j] = 0;
      ca_table[k][j] = 0;
    }
  }

  // set seed
  ca_table_test[N/2][M/2] = 1;
  ca_table[N/2][M/2] = 1;

  // instanciate modules
  RunCA dut;
  TestCA tb;

  // evaluation variables
  int correct_generations = 0;  
  bool correct;

  // run for GENERATIONS
  for (int i=0;i<GENERATIONS;i++){
    
    generate_random_lut(lut); // generate random lut 

    dut.compute_generation(ca_table, lut); 
    tb.compute_generation(ca_table_test,lut); 

    if(VERBOSE){
      std::cout<< "Generation: " << (i+1) << "\n";
      print_table_symbols(ca_table,avail_symbols,1);
      print_table_symbols(ca_table_test,avail_symbols,0);
    }

    correct = evaluate_generation(ca_table_test,ca_table);
    correct_generations += correct;

    if(VERBOSE) std::cout << "Generation: " << (i+1) << (correct?", PASSED":", FAILED") << "\n\n";

  }
  std::cout << "Generations Passed: " << correct_generations << "/" << GENERATIONS << "\n";

  CCS_RETURN(0);
}
