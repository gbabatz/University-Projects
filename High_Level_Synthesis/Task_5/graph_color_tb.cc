#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ac_int.h"

#include "graph_color.h"

const int RUNS = 10;

CCS_MAIN(int argc,char* argv[]){

    dtype_color node_colors[N];
    dtype_adj adj_mat[N];
    dtype_color col_num;
    bool temp_num;
    graphColor dut; //my module
    
//    std::srand(std::time(NULL));
    std::srand(1);


    for (int r=0;r<RUNS;r++){
        std::cout << "-----~~~~/TEST-" << (r+1) << "\\~~~~-----\n\n";
        ac::init_array<AC_VAL_0>(node_colors,N);
        ac::init_array<AC_VAL_0>(adj_mat,N);
        col_num = 0;


        //create adjacency matrix
        int j_start = 1;
        for (int i = 0;i<N-1;i++){
            for(int j = j_start;j<N;j++){
                temp_num = std::rand() % 2; 
                adj_mat[i][j] = temp_num;
                adj_mat[j][i] = temp_num;
            }
            j_start++;
        }
        
        //print adjacency matrix
        std::cout << "Adjacency Matrix:\n";
        for (int i = 0;i<N;i++){
            for(int j = 0;j<N;j++){
                std::cout << adj_mat[i][j] << " ";
            }
            std::cout << "\n";
        }
        
        //run module
        dut = graphColor();
        dut.run(adj_mat,node_colors,col_num);
     
        //print result node colors 
        std::cout << "Node Colors:\n";
        for (int i=0;i<N;i++){
            std::cout << node_colors[i] << " ";
        }
        std::cout << "\nMax Color: " << col_num << "\n";
        std::cout << "\n\n";
    }        
///*
    int k = 4;
    dtype_adj test_mat[k];
 /*   test_mat[0] = 0b110010;
    test_mat[1] = 0b100101; 
    test_mat[2] = 0b001010;
    test_mat[3] = 0b010100;
    test_mat[4] = 0b101001;
    test_mat[5] = 0b010011;
*/
    test_mat[0] = 0b0010;
    test_mat[1] = 0b1101; 
    test_mat[2] = 0b1010;
    test_mat[3] = 0b0110;
    dtype_color node_clrs[k];

        std::cout << "-----~~~~/TEST-EXTRA\\~~~~-----\n\n";
        std::cout << "Adjacency Matrix:\n";
        for (int i = 0;i<k;i++){
            for(int j = 0;j<k;j++){
                std::cout << test_mat[i][j] << " ";
            }
            std::cout << "\n";
        }
        
        //run module
        dut = graphColor();
        dut.run(test_mat,node_clrs,col_num);
     
        //print result node colors 
        std::cout << "Node Colors:\n";
        for (int i=0;i<k;i++){
            std::cout << node_clrs[i] << " ";
        }
        std::cout << "\nMax Color: " << col_num << "\n";
        std::cout << "\n\n";
//*/


    CCS_RETURN(0);
}
