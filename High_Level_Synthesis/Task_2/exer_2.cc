#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ac_int.h"

const int TESTS = 10;
const int Wout = 32;	//the output of the multiplier will be standard bit width
			//we need it to be high enough so the value does not overflow
const int TEST_BIT_LEN = 16;	//the bit width of the factors

template <int W>
void csd_encode (ac_int<W,true> &num, ac_int<W,false> &x_p, ac_int<W,false> &x_n){

   x_p = 0;
   x_n = 0;
   int i = 0;
   bool carry = 0;
   ac_int<W+1,true> working_num;
   working_num = num;
   working_num[W] = num[W-1];
   ac_int<2,false> temp;

   while (i < W){
      temp = working_num.template slc<2>(i);
      if(carry == 0){
         if(temp[0] == 0){
            x_p[i] = 0;
            carry = 0;
         }else if(temp[1] == 0){
            x_p[i] = 1;
            carry =0; 	
         }else{
            x_n[i] = 1;
            carry = 1;
         }	
      }else{
         if(temp[0] == 1){
            x_n[i] = 0;
            carry = 1;
         }else if(temp[1] == 1){
            x_n[i] = 1;
            carry =1; 	
         }else{
            x_p[i] = 1;
            carry = 0;
         }        
      }
      
      ++i;
   }
}

template<int W>
ac_int<Wout,true> csd_mult(ac_int<W,true> &in, const ac_int<W,false> &x_p, const ac_int<W,false> &x_n){

	ac_int<Wout,true> result = 0;
	ac_int<Wout,true> temp_in = in; 
//without a temp variable we lose the number after 
//shift if bit length is small
	
	for (int i = 0;i < W;++i){
		if (x_p[i]){
 			result = result + (temp_in << i);
		}else if(x_n[i]){
			result = result - (temp_in << i);
		}	
	}
	return result;
}

template <int W,bool sign>
void binary(ac_int<W,sign> inp){
	for (int i = W-1;i > -1 ; --i){
		std::cout << inp[i];
	}
	std::cout << std::endl;
}


int main(){

	std::srand(std::time(NULL));
	int test_passed = 0;

	for (int i = 0; i < TESTS; ++i){
		
		std::cout << "TEST " << (i+1) << std::endl;
	
		ac_int<TEST_BIT_LEN,true> test_input;
		ac_int<TEST_BIT_LEN,false> x_p,x_n;

		test_input = std::rand();
		csd_encode(test_input,x_p,x_n); //x_p,x_n acquire their values
		
		std::cout << "test input: " << test_input << std::endl;
		std::cout << "x_p	: " << x_p << std::endl;
		std::cout << "x_n	: " << x_n << std::endl;
		if ((x_p - x_n) == test_input) std::cout << "--CSD_ENCODE_PASSED--\n\n";
		

//		ac_int<TEST_BIT_LEN,true> factor = 10;
		ac_int<TEST_BIT_LEN,true> factor = std::rand();
		ac_int<Wout,true> test_mult_resul;
		test_mult_resul = csd_mult(factor,x_p,x_n);
		
		std::cout << "factor of multiplication is: " << factor << std::endl;
		std::cout << "result for the above x_p and x_n is: " << test_mult_resul << std::endl;
		int groundtruth_result = factor * (int)test_input; 

		if (groundtruth_result == test_mult_resul) {
			std::cout << "--MULT_PASSED--\n"; 
			test_passed++;
		}
		std::cout << "\n\n";
	}
	std::cout << "successful tests: " << test_passed << "/" << TESTS << "\n\n";

	return 0;


}
