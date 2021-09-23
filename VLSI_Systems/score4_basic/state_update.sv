module state_update(
input logic clk,
input logic rst,
input logic left,
input logic right,
input logic put,
input logic win_a,
input logic win_b,
input logic full_panel,
output logic [1:0] panel[5:0][6:0],
output logic [6:0] play,
output logic turn,
output logic invalid_move
);


logic left_reg,left_event,right_reg,right_event,put_reg,put_event;


always_ff @(posedge clk, negedge rst) begin
	if (!rst) begin
		left_reg <= 1'b0;
	end 
	else begin 
		left_reg <= left;
	end
end 

assign left_event = left_reg & (~left);  //falling edge
// assign rising_edge = (~left_reg) & left;

always_ff @(posedge clk, negedge rst) begin
	if (!rst) begin
		right_reg <= 1'b0;
	end 
	else begin 
		right_reg <= right;
	end
end 

assign right_event = right_reg & (~right);


always_ff @(posedge clk, negedge rst) begin
	if (!rst) begin
		put_reg <= 1'b0;
	end 
	else begin 
		put_reg <= put;
	end
end 

assign put_event = put_reg & (~put);


int i;
int put_col;
int put_row;
logic found; // usage in finding the first open spot on panel

always_comb begin
	// I want to know what bit of play vector is at one ..calculating it with combinational logic
	for (i=0;i<7;i=i+1)begin
		if (play[i] == 1) put_col=i; //there is always an ace and only one since we operate with shifting
	end
	
	found = 0;
	// now I need to find the first open row for the put_col column if i dont find open spot its an invalid move
	for (i=0; i<6; i=i+1)begin  //starting from row 5 : the lowest
		if (panel[i][put_col]==2'b00 && !found) begin
			put_row = i;
			found = 1;
		end
	end
	
	invalid_move = 0;
	if (found == 0) invalid_move = 1;
	
end

enum {INITIAL,PLAY_UPDATE,PANEL_UPDATE,CHECK_STATUS,ENDGAME} state;

always_ff @ (posedge clk, negedge rst) begin
    if(!rst) state <= INITIAL;
	else begin
		case(state) 
            INITIAL: begin
				panel <= '{default:0};
				// panel <=  '{'{2'b10,2'b00,2'b00,2'b00,2'b00,2'b00,2'b00},
							// '{2'b10,2'b10,2'b00,2'b00,2'b00,2'b00,2'b00},
							// '{2'b10,2'b00,2'b10,2'b01,2'b00,2'b00,2'b00},
							// '{2'b01,2'b00,2'b01,2'b01,2'b00,2'b00,2'b00},
							// '{2'b10,2'b10,2'b01,2'b00,2'b00,2'b00,2'b00},
							// '{2'b01,2'b10,2'b01,2'b10,2'b01,2'b10,2'b01}};
				play <= 7'b1000000;
				turn <= 0; //red plays first
				state <= PLAY_UPDATE;
			end
			PLAY_UPDATE: begin
				if(put_event) begin 
					state <= PANEL_UPDATE;
				end
				else begin
					if (left_event) begin //circular shift
						play <= play << 1;
						play[0] <= play[6];
						state <= PLAY_UPDATE;
					end
					else if (right_event) begin
						play <= play >> 1;
						play[6] <= play[0];
						state <= PLAY_UPDATE;					
					end
				end
			end
			PANEL_UPDATE: begin
				if (invalid_move) begin					
					state <= PLAY_UPDATE;
				end
				else if (turn == 0) begin
					panel[put_row][put_col] <= 2'b01;
					state <= CHECK_STATUS;
				end				
				else if (turn == 1) begin
					panel[put_row][put_col] <= 2'b10;
					state <= CHECK_STATUS;
				end
			end
			CHECK_STATUS: begin
				if (win_a || win_b || full_panel) state <= ENDGAME;
				else begin 
					state <= PLAY_UPDATE;
					turn <= ~turn;   //changing player after i update panel ...better with testbench  
				end
			end
			ENDGAME: begin
				state <= ENDGAME; //if I go to INITIAL state I lose the last frame 
			end
		endcase
	end

end


endmodule
