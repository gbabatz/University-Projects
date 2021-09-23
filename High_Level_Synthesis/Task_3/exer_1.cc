#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ac_int.h"
#include "ac_channel.h"
#include <vector>


typedef ac_int<4,false> dtype;
const int TESTS = 100;
const int TEST_LENGTH = 10;


class RunLengthEncoder {
	private:
	dtype current_val;
	dtype next_val;
	dtype counter; //counts up to 15 instances
	dtype init;
	public:
	RunLengthEncoder() {
		current_val = 0;
		next_val = 0;
		counter = 1;		
		init = 1; //init = 1 means its the first call of the func
		//prevents to print on the first call (0 1) or to misscount if 0 is
		//the first occurence..needed since these are the initial values of my parameters
	}
	// top-level interface
	void run (ac_channel<dtype> &in, ac_channel<dtype> &out) {
	
		if(in.available(1)){
			next_val = in.read();
			if(current_val == next_val){ 
				if(!init) counter++;
			}else if(!init){ 
				out.write(current_val);
				out.write(counter);
				counter = 1;
			}
			//if I dont have any other value then stream out the final values
			if(!(in.available(1))){
				out.write(next_val);
				out.write(counter);
				counter = 1;
			}
			
			//need to have some control over the first call
			if(init) init = 0;

			current_val = next_val;
		}
	}
};

//can be used to examine tests
void rle_decoder(ac_channel<dtype> &in, ac_channel<dtype> &out){
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

int main(int argc,char** argv){ 

	//creating the tests
	//std::srand(50);
	std::srand(std::time(NULL));
	dtype test[TESTS][TEST_LENGTH];
	int successful_tests = 0;
	std::vector<int> failed_tests; 
	for(int i = 0; i < TESTS; ++i){
		for (int j = 0; j < TEST_LENGTH; ++j){
			test[i][j] = std::rand();	
		}
	}

	//applying tests
	for(int i = 0; i < TESTS; ++i){
		std::cout << "----TEST_" << (i+1) << "----\n";	

		ac_channel<dtype> stream_in;
		ac_channel<dtype> stream_out;
		ac_channel<dtype> test_stream_in; //using decoder to verify
		ac_channel<dtype> test_stream_out; 
		dtype verification_output;
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
		//initialize and run module - fill output	
		RunLengthEncoder module1;
		while(stream_in.available(1)) {
			module1.run(stream_in,stream_out);
		}	

		std::cout << "\nstream out: ";
		while(stream_out.available(1)){
			test_output = stream_out.read();
			std::cout << test_output << " ";
			
			//for verification
			test_stream_in.write(test_output);
		}

		//for verification
		while(test_stream_in.available(1)){
			rle_decoder(test_stream_in,test_stream_out);
		}
				
		int indx = 0;
		int flag = 1;
		
		//std::cout << "\nverification: "; //for debugging
		while(test_stream_out.available(1)){
			verification_output = test_stream_out.read();
			std::cout << verification_output << " "; //for debugging
			if(verification_output != test[i][indx]){
				failed_tests.push_back(i+1);
				flag = 0;
				break;
			}
			indx++;
		}
		
		successful_tests += flag;

		std::cout << "\n\n";
	}
	std::cout << "tests passed: " << successful_tests << "/" << TESTS << "\n";
	std::cout << "tests failed: "; 
  for(size_t i = 0; i < failed_tests.size(); ++i){
    std::cout << failed_tests[i] << ", ";
  }
/*
	for(int i:failed_tests)
		std::cout << i << ", "; 
*/
	std::cout << "\n\n";

	return 0;
}
