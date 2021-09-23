#include <iostream>
#include <cstdlib>
#include <ctime> 

const int INT_LEN = 32;
const int TESTS = 10;

typedef unsigned int Data_t;
typedef unsigned short int Data_t_short;

Data_t_short one_counter(Data_t inp){
	Data_t_short counter = 0;
	for (int i=0;i<INT_LEN;++i){
		counter = counter + ((inp >> i) & 0x0001);
	}
	return counter;
}

void binary_repr(int c) {  
        for (int i = 0; i < INT_LEN; i++) {  
                std::cout << !!((c << i) & 0x80000000); 
        }  
}

int main(){

	std::srand(std::time(NULL));
	for (int i=0; i<TESTS; ++i){
	
		Data_t test_int = std::rand();
		Data_t_short result = one_counter(test_int);

		std::cout << "test int = " << test_int << " \t";
		std::cout << "bin = ";
		binary_repr(test_int);
		std:: cout << "\t";
		std::cout << "number of 1's = " << result << "\n";
	}

	return 0;
}
