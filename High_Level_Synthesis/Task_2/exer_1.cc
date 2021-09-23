#include <iostream>
#include "ac_fixed.h"
#include "ac_int.h"


int main(){
	
	std::cout << "\na)" << "\n";
	ac_int<8,false> a,b; //range [0,2^8-1]
	ac_int<9,true> c,d; //range [-2^8,2^8-1]
	ac_int<18,true> e; //= a*b + c*d;

	a = b = 255; //= 0xFF;
	c = d = -256; //=-0x100;
	e = a*b + c*d;
	std::cout << "the biggest absolute value is: " << e << "\n";
	std::cout << "will be produced for a = b = " << a << ", c = d = " << c << "\n";
	std::cout  << "and is accurately represented with at least 18 bits." << "\n\n";

	
	std::cout << "b)" << std::endl;
	ac_fixed<5,2,false> f; //11.111 = 3.875 range [0, 3.875]
	ac_fixed<5,4,true> g; //xxxx.x SIGNED range [-8, 7.5] step = 0.5
	ac_fixed<10,6,true> h; //= f*g;
	//3.875*7.5 = 29.0625
	//3.875*(-8) = -31;
	//we need at least 6 bits for main
	// body [-32,31] and at least 4 bits after the 'comma'

	f = 3.875;
	std::cout << "f = " << f;
	g = 7.5;
	std::cout << " g = " << g;
	h = f*g;
	std::cout << " h = f*g = " << h << std::endl;	

	f = 3.875;
	std::cout << "f = " << f;
	g = -8;
	std::cout << " g = " << g;
	h = f*g;
	std::cout << " h = f*g = " << h << std::endl;	

	return 0;


}

