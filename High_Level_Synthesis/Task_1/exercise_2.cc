#include <iostream>
#include <cstdlib>
#include <ctime> 

const int TESTS = 10;

typedef unsigned int Data_t;

void why_checksum_works(Data_t input){
	std::cout << "Why function checksum works.\n";
	std::cout << "For input = " << input << " we have:" << "\n";
	while (input){
	std::cout << input << " % 10 = " << input%10 << "\t";
	std::cout << input << "/10 = " << (input = input/10);
	std::cout << "\n";
	}	
}


int checksum(Data_t inp){
	bool dublicate = 0;
	int sum = 0;
	int temp = 0;
	while (inp!=0) {
		temp = inp%10;
		if (dublicate){
			temp = 2*temp;	
			(temp > 9)?(sum+=(temp%10 + temp/10)):(sum+=temp);
		}else{
			sum += temp;
		}			

    dublicate = !dublicate;	
    inp = inp/10;
	}	
	return sum;
}

	
int main(){

	//why the function works: 
	//Data_t test_int = 3561;
	//why_checksum_works(test_int);
	
	std::srand(std::time(NULL));
	std::cout << "\n------TESTS------" << "\n\n";
	for (int i=0; i<TESTS; ++i){
	
		int test_int = std::rand();
		int result = checksum(test_int);

		std::cout << "test int = " << test_int << "  \t";
		std::cout << "checksum result = " << result << "\n";
	}

	return 0;
}
