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
module cpu_paddle (
	 output reg [9:0] cpu_y_pos,
    input [9:0] ball_y_pos,
    input clk,
	 input rst
   );
	
	
	initial begin
		cpu_y_pos = 224;
	end

	always @(posedge clk) begin
		if (rst) begin
			cpu_y_pos <= 224;
		end

		else begin
			if (ball_y_pos <= 452 && ball_y_pos >= 32) begin
				cpu_y_pos <= ball_y_pos-32;
			end
		end
		
	end

endmodule 