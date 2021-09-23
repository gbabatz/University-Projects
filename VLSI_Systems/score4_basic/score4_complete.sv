module score4_complete(
input logic clk,
input logic rst,
input logic left,
input logic right,
input logic put,
output logic hsync,
output logic vsync,
output logic [3:0] red,
output logic [3:0] green,
output logic [3:0] blue,
output logic win_a,
output logic win_b,
output logic full_panel,
output logic player,
output logic invalid_move,
output logic [1:0] panel[5:0][6:0]
);


logic [9:0] v_counter;
logic [9:0] h_counter;

VGA_sync mod1 (clk,rst,hsync,vsync,v_counter,h_counter);

// logic [1:0] panel[5:0][6:0]; 
logic [6:0] play;
logic turn;

state_update mod2 (clk,rst,left,right,put,win_a,win_b,full_panel,panel,play,turn,invalid_move);

RGB_output mod3 (clk,rst,v_counter,h_counter,panel,play,turn,red,green,blue);

check_state mod4 (clk,rst,panel,play,turn,win_a,win_b,full_panel,player);

endmodule