module RGB_output(
input logic clk,
input logic rst,
input logic [9:0] v_counter,
input logic [9:0] h_counter,
input logic [1:0] panel[5:0][6:0],
input logic [6:0] play,
input logic turn,
output logic [3:0] red,
output logic [3:0] green,
output logic [3:0] blue
);
// assign rst = ~ rst; I changed the testbench instead

// ------------- LOGIC FOR PANEL ---------------------
// the input would be a 2d panel array 6x7 unpacked array with 2bits packed, 7 bits play array and 1 bit turn signal
// create some conditions of v_counter and h_counter and act accordingly.

localparam square_dim_h = 40;
localparam square_dim_v = 40;
localparam h_margin = 45;
localparam v_margin = 25;

// creating the square conditions
// with packed array
// left is the most significant bit
logic [5:0][6:0] square_conditions;
int row;
int col;
//with square_conditions[5-row][6-col] the enumeration is as shown at the presentation 
always_comb begin
	for (row = 0; row<6; row=row+1) begin
		for (col = 0; col<7; col=col+1) begin
		
			square_conditions[5-row][6-col] = ((h_counter >= h_margin+col*(h_margin+square_dim_h)) && (h_counter <= (h_margin+square_dim_h+col*(h_margin+square_dim_h))) && (v_counter >= v_margin+row*(v_margin+square_dim_v)) && (v_counter <= (v_margin+square_dim_v+row*(v_margin+square_dim_v))))?1:0;
		
		end
	end
end

// ---------------------------LOGIC FOR PLAY AND TURN ------------------
logic [6:0] play_conditions;
int playindex;
localparam play_offset = 6; //in order to use the same code as before...lengths were calculated leaving an extra row ..here we use that row
localparam half_square = 20; //an extra top margin for the play row
// left at 6 right at 0 
always_comb begin
	for (playindex= 0; playindex<7; playindex=playindex+1) begin
		
		play_conditions[6-playindex] = ((h_counter >= h_margin+playindex*(h_margin+square_dim_h)) && (h_counter <= (h_margin+square_dim_h+playindex*(h_margin+square_dim_h))) && (v_counter >= half_square+v_margin+play_offset*(v_margin+square_dim_v)) && (v_counter <= (v_margin+square_dim_v+play_offset*(v_margin+square_dim_v))))?1:0;
		
	end
end

int i;
int j;
int k;
// black pixel and rst pixel logic
always_ff @(posedge clk or negedge rst) begin

    if (!rst) begin 
    red <= 0;
    green <= 0;
    blue <= 0;
    end
    else begin
        //black pixels CHANGE: IT WAS "h_counter > 640 || v_counter > 480" MADE IT: and thats completly right after an analysis i made 
        if (h_counter >= 640 || v_counter >= 480) begin
        red <= 0;
        green <= 0;
        blue <= 0;
        end
		else begin 
			
			//setting rgb = 0 if no condition active this will be the value on posedge if true condition then the value will be the last of the block
			red <= 0;
			green <= 0;
			blue <= 0;
			
			for (i=0; i<6; i=i+1)begin
				for (j=0; j<7; j=j+1)begin
				
					if (square_conditions[i][j]) begin
						if (panel[i][j]==2'b01) begin
							red <= 4'b1111;
							green <= 0;
							blue <= 0;
						end
						else if(panel[i][j]==2'b10) begin
							red <= 0;
							green <= 4'b1111;
							blue <= 0;
						end	
					end
				end
			end

			// LOGIC FOR PLAY 
			for (k=0; k<7; k=k+1) begin
				if (play[k]) begin
					if(play_conditions[k]) begin
						if(turn == 0) begin
							red <= 4'b1111;
							green <= 0;
							blue <= 0;	
						end
						else begin
							red <= 0;
							green <= 4'b1111;
							blue <= 0;	
						end
					end 
				end
			end
			
		end
    end    
end


endmodule
