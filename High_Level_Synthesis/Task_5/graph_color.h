#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ac_int.h"

#include "mc_scverify.h"

const int N = 5; //graph nodes
const int C_LEN = ac::log2_ceil<N>::val; //worst case one color for each node
typedef ac_int<N,false> dtype_adj;
typedef ac_int<C_LEN,false> dtype_color;

#pragma hls_design top
class graphColor{
    private:
	public:
	graphColor(){}
	
#pragma hls_design interface 
void CCS_BLOCK(run)(const dtype_adj Adj_G[N],dtype_color colors_G[N],dtype_color &color_num){
    ac_int<1,false> exists;
	  dtype_color working_color; 
   	dtype_color max_color = 1;

    NODES: for (int i=0;i<N;++i){
        working_color = 1;

        FIND_CLR: do{ //can be replaced with for loop
            exists = 0;
            CHECK_CLR: for (int j=0;j<i;j++){
                if(Adj_G[i][j] && colors_G[j] == working_color){
                    exists = 1;
                    working_color++;
                    break;
                }
			}
        }while(exists);
            colors_G[i] = working_color;
            max_color = (working_color>max_color)?working_color:max_color;
	}
        color_num = max_color;
}
};

