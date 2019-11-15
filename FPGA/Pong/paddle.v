`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:43:05 02/05/2017 
// Design Name: 
// Module Name:    vga_rectangle 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module p1_paddle(
	 output reg [9:0] p1_y_pos,
    input up,
    input down,
    input clk,
	 input rst,
	 input gameover_in
   );

	reg gameover;
	initial begin
		p1_y_pos <= 224;
	end

	always @(posedge clk) begin
		gameover <= gameover_in;
	end
	
	always @(posedge clk) begin
		if (rst) begin
			p1_y_pos <= 224;
		end
	
		else if (gameover) begin
			// do nothing
		end
		
		else if (up) begin
			if (p1_y_pos <= 396) begin
				p1_y_pos <= p1_y_pos + 32;
			end
		end
		
		else if (down) begin
			if (p1_y_pos >= 24) begin
				p1_y_pos <= p1_y_pos - 32;
			end
		end
	end


endmodule


				