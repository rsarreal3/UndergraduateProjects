`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:29:36 04/23/2017 
// Design Name: 
// Module Name:    display 
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
module display(
	output reg red,
   output reg green,
   output reg blue,
	input blank,
	input [9:0] cpu_posy,
	input [9:0] p1_posy,
	input [9:0] ball_posx,
	input [9:0] ball_posy,
	input [9:0] pos_h,
   input [9:0] pos_v,
	input game_over,
	input collision,
	input clk,
	input rst
   );

	reg p1_turn;
	reg cpu_turn;

	// p1
	parameter p1_width = 1;
	parameter p1_height = 64;
	parameter p1_x_left = 64;
	reg [9:0] p1_bottom;
	// cpu
	parameter cpu_width = 1;
	parameter cpu_height = 64;
	parameter cpu_x_left = 576;
	reg [9:0] cpu_bottom;
	//ball
	parameter ball_width = 3;
	parameter ball_height = 3;
	reg [9:0] ball_x_left;
	reg [9:0] ball_bottom;	
	
	always @(posedge clk) begin
		p1_bottom <= p1_posy;
		cpu_bottom <= cpu_posy;
		ball_x_left <= ball_posx;
		ball_bottom <= ball_posy;
	end
	
	
	//addinal intermediate logic signal wires
	wire p1, cpu, ball; // high only when over rectangle
	wire [9:0] x,y; // traditional cartesian coordinates, (left,bottom)=(0,0)
	
	//combinational logic to calculate x,y coordinate system for each compnent
	assign x = pos_h;
	assign y = 480 - pos_v;
	
	assign p1 = ((x >= (p1_x_left) && x < (p1_x_left + p1_width))
								&& (y >= (p1_bottom) && y < (p1_bottom + p1_height)));	
	assign cpu = ((x >= (cpu_x_left) && x < (cpu_x_left + cpu_width))
								&& (y >= (cpu_bottom) && y < (cpu_bottom + cpu_height)));
	assign ball = ((x >= (ball_x_left) && x < (ball_x_left + ball_width))
								&& (y >= (ball_bottom) && y < (ball_bottom + ball_height)));


	initial begin
		p1_turn <= 1;
		cpu_turn <= 0;
	end

		
		
	always @(posedge clk) begin
		red	<= (p1 | ball) & ~cpu & !blank;
		green <= ~p1 & (ball | cpu) & !blank;
		blue  <= (p1 | ball | cpu) & !blank;

		if (game_over) begin
			if (ball_x_left < 360) begin
				red	<= ~(p1 | ball | cpu) | blank;
				green <= (p1 |  ball | cpu) & ~blank;
				blue  <= (p1 | ball | cpu) & ~blank;
			end
			else begin
				red	<= (p1 | ball | cpu) & ~blank;
				green <= ~(p1 | ball | cpu) | blank;
				blue  <= (p1 | ball | cpu) & ~blank;
			end
		end
	end
endmodule
