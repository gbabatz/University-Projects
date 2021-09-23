#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ac_int.h"
#include "ac_channel.h"

typedef ac_int<4,false> dtype;
const int COUNTER_BIT_LEN = 4; //counts up to 15 instances
const int TESTS = 10;
const int TEST_LENGTH = 12;

//if the channel contains at least 10 values it will flush out all of them
void runlength_encode(ac_channel<dtype> &in,ac_channel<dtype> &out){
	dtype num_val,num_next_val;
	ac_int<COUNTER_BIT_LEN,false> num_cnt,ch_size;

	if(in.available(10)){
		num_val = in.read();
		num_cnt = 1;
		ch_size = in.size();
//		ch_size = 10;	//or use this for exactly 10 values
		for(int i = 0; i < ch_size; i++){
			num_next_val = in.read();
			if(num_val == num_next_val){
				num_cnt++;
			}else{
				out.write(num_val);
				out.write(num_cnt);
				num_cnt = 1;	
			}	
			
// probably need to change this code since .available() is not synthesizable
//would do it by adding a flag or using the 'i' of the loop for example
			if(!in.available(1)){ //if I have read the final value
				out.write(num_next_val);
				out.write(num_cnt);	
			}
			
			num_val = num_next_val;
		}
	}
}

/* //version 2 coming soon ... never came
void runlength_encode2(ac_channel<dtype> &in,ac_channel<dtype> &out){
	
}
*/

//can be used to examine tests
void rle_decode(ac_channel<dtype> &in, ac_channel<dtype> &out){
	dtype in_char,in_cnt;

	if(in.available(2)){
		in_char = in.read();
		in_cnt = in.read();
	}

	while(in_cnt > 0){
	out.write(in_char);
	in_cnt--;
	}
}


int main(){

/*//manual tests
dtype test_1[10] = {2,1,1,4,4,5,5,4,3,1};
dtype test_2[10] = {2,2,2,2,5,5,5,6,6,6};
dtype test_3[10] = {0,0,5,1,2,2,2,1,1,4};
dtype test_4[13] = {4,4,4,4,3,2,1,1,1,5,5,5,3};
dtype test_5[7]	 = {3,3,3,1,1,1,3};
*/

	//creating the tests
	//std::srand(5);
	std::srand(std::time(NULL));
	dtype test[TESTS][TEST_LENGTH];
	for(int i = 0; i < TESTS; ++i){
		for (int j = 0; j < TEST_LENGTH; ++j){
			test[i][j] = std::rand();	
		}
	}

	//applying tests
	for(int i = 0; i < TESTS; ++i){
		
		ac_channel<dtype> stream_in;
		ac_channel<dtype> stream_out;
		dtype test_output;

		//fill the channel
		std::cout << "stream in: ";
		for(int j = 0; j < TEST_LENGTH; ++j){
			stream_in.write(test[i][j]);			
			std::cout << test[i][j] << " ";
		}
		//another way
		//for(dtype k:test[i]) stream_in.write(k);


		//operate on channel
		runlength_encode(stream_in,stream_out);
		
		std::cout << "\nstream out: ";
		while(stream_out.available(1)){
			test_output = stream_out.read();
			std::cout << test_output << " ";
		}
		std::cout << "\n\n";
	}

	return 0;
}
