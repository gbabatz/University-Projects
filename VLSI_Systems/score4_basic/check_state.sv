module check_state(
input logic clk,
input logic rst,
input logic [1:0] panel[5:0][6:0],
input logic [6:0] play,
input logic turn,
output logic win_a,
output logic win_b,
output logic full_panel,
output logic player
);

assign player = turn ; //player A red/01 - 0, player B green/10 - 1

logic [5:0][1:0] horizontal_temp;
logic [1:0] horizontal;
logic [6:0][1:0] vertical_temp;
logic [1:0] vertical;
logic [2:0][1:0] diag1_temp;
logic [1:0] diag1;
logic [2:0][1:0] diag2_temp;
logic [1:0] diag2;
logic [1:0] winner;

logic [5:0][6:0] full_temp;

int i;
int j;
always_comb begin
	//winner logic

	for (i=0;i<6;i=i+1) begin
		
		horizontal_temp[i] =(panel[i][0] & panel[i][1] & panel[i][2] & panel[i][3]) | 
							(panel[i][1] & panel[i][2] & panel[i][3] & panel[i][4]) |
							(panel[i][2] & panel[i][3] & panel[i][4] & panel[i][5]) |
							(panel[i][3] & panel[i][4] & panel[i][5] & panel[i][6]);
	
	end
	
	horizontal  = horizontal_temp[0] | horizontal_temp[1] | horizontal_temp[2] | horizontal_temp[3] | horizontal_temp[4] | horizontal_temp[5];
	// horizontal holds 00 if no horizontal win , 01 if player A/red wins, 10 if player B/green wins, horizontally only 
	
	for (i=0;i<7;i=i+1) begin
		
		vertical_temp[i] =	(panel[5][i] & panel[4][i] & panel[3][i] & panel[2][i]) | 
							(panel[4][i] & panel[3][i] & panel[2][i] & panel[1][i]) |
							(panel[3][i] & panel[2][i] & panel[1][i] & panel[0][i]);
	
	end
	
	vertical = vertical_temp[0] | vertical_temp[1] | vertical_temp[2] | vertical_temp[3] | vertical_temp[4] | vertical_temp[5] | vertical_temp[6] ;
	
	
	for (i=0;i<3;i=i+1) begin
		
		diag1_temp[i] =		(panel[i][6] & panel[i+1][5] & panel[i+2][4] & panel[i+3][3]) | 
							(panel[i][5] & panel[i+1][4] & panel[i+2][3] & panel[i+3][2]) |
							(panel[i][4] & panel[i+1][3] & panel[i+2][2] & panel[i+3][1]) | 
							(panel[i][3] & panel[i+1][2] & panel[i+2][1] & panel[i+3][0]);
	
	end
	
	diag1 = diag1_temp[0] | diag1_temp[1] | diag1_temp[2];
	
	for (i=0;i<3;i=i+1) begin
		
		diag2_temp[i] =		(panel[i][3] & panel[i+1][4] & panel[i+2][5] & panel[i+3][6]) | 
							(panel[i][2] & panel[i+1][3] & panel[i+2][4] & panel[i+3][5]) |
							(panel[i][1] & panel[i+1][2] & panel[i+2][3] & panel[i+3][4]) | 
							(panel[i][0] & panel[i+1][1] & panel[i+2][2] & panel[i+3][3]);
	
	end
	
	diag2 = diag2_temp[0] | diag2_temp[1] | diag2_temp[2];
	
	winner = horizontal | vertical | diag1 | diag2;
	
	//a = red, b = green
	if (winner == 2'b01) win_a = 1;
	else if(winner == 2'b10) win_b = 1;
	else begin
		win_a = 0;
		win_b = 0;
	end
	
	
	// full logic 
	// filling a temp panel with 1 if it has a player value and with 0 if the value is 00 ...then reducing the panel with AND  
	for (i=0; i<6; i=i+1) begin
		for (j=0; j<7; j=j+1) begin
			if (panel[i][j] != 2'b00) full_temp[i][j] = 1'b1;
			else full_temp[i][j] = 0;			
		end
	end
	
	full_panel = &full_temp;
	

end 

endmodule
