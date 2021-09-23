#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ac_int.h"
#include "ac_channel.h"

typedef ac_int<4,false> Card;
typedef ac_int<5,false> dtype; 

const int MAX_CARD = 10; //not sure if max card is 10 or 11. I assume 10
const int TESTS = 10;
const int TEST_LENGTH = 5; //no need to be higher if inp values are checked

class BlackJack {
	private:
	dtype card_sum;
	Card card_count;
	
	public:
	BlackJack() {
	card_sum = 0;
	card_count = 0; 
	} 
	
	void print(){ //for debugging purposes
	std::cout << "\tcard_sum = " << card_sum << " card_count = " << card_count << "\n";
	}
	
	void print_sum(){
	std::cout << "cards_sum: " << card_sum << "\t";
	}

	void run (ac_channel<Card> &card_in, bool &end_round, bool &win) {
		Card current_card;
		
		end_round = false;
		win = false;
		
		if(card_in.available(1)){
			current_card = card_in.read();
			if(current_card == 1){
				card_count++;
				card_sum += 11;
			//if its not controlled on inputs
			}else if(current_card <= MAX_CARD && current_card > 0){
				card_count++;
				card_sum += current_card;
			}else{
				//do nothing. invalid card num
			}

			//print(); //could think the conditions better to compact them but didnt
			if(card_count == 2 && card_sum == 22){
				win = true;
				end_round = true;
				BlackJack();
			}else if(card_sum > 21){
				end_round = true;
			}else if(card_count == 5 || card_sum == 21){ //at this point I have checked that the value is 21 or less
				win = true;
				end_round = true;
				BlackJack();
			}
      //calling BlackJack() to reinitialize the public variables
      //if its not synthesizable I can just write the assignments
		}
	}
};

int main(int argc, char* argv[]){

///*
	//----creating the tests----
//	std::srand(50);
	std::srand(std::time(NULL));
	Card test[TESTS][TEST_LENGTH];
	Card rand_num;
	for(int i = 0; i < TESTS; ++i){
		for (int j = 0; j < TEST_LENGTH; ++j){
			do{
				rand_num = std::rand();
			}while(rand_num > MAX_CARD || rand_num == 0);	
//			}while(rand_num == 0);	
//check the validity of the cards here.. if not checked nothing happens
//it just needs more rounds.. need to have higher TEST_LENGTH
			test[i][j] = rand_num;
		}
	}
//*/
//	Card test[1][6] = {1,1,3,3,3,3};
//	Card test[1][5] = {7,5,9,1,3};
	
	//----applying tests----
	for(int i = 0; i < TESTS; ++i){
		std::cout << "----TEST_" << (i+1) << "----\n";	

		ac_channel<Card> stream_in;
		bool out_win = false;
		bool out_end_round = false;
		BlackJack module1;

		//----fill the channel and test----
		//break the loop when round ends
		std::cout << "giving cards: . . . \n";
		for(int j = 0; j < TEST_LENGTH; ++j){

			std::cout << "card : " << test[i][j] << "\t";
			stream_in.write(test[i][j]);			

			//checking the cards as soon as they are given
			module1.run(stream_in,out_end_round,out_win);
			module1.print_sum();	

			if(!out_end_round){
				std::cout << "next card...\n"; 
			}else if(out_win){
				std::cout << "WON!\n";
				break;
			}else{
				std::cout << "lost. . . \n";
				break;
			}	
			
		}
		std::cout << "\n\n";
	}
	return 0;
}
