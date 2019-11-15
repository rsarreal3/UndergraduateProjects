`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:33:13 04/22/2017 
// Design Name: 
// Module Name:    collision_detector 
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
module collision_detector(
	input [9:0] ball_posx,
	input [9:0] ball_posy,
	input [9:0] p1_posy,
	input [9:0] cpu_posy,
	input ball_x_vel,
	input clk,
	output reg  collision
    );


	parameter p1_posx = 64;
	parameter cpu_posx = 576;
	parameter paddle_height = 64;

	always @(posedge clk) begin
		if (ball_posx == p1_posx && ball_x_vel == 0) begin
			if ((ball_posy >= p1_posy-5) && (ball_posy < p1_posy + 70)) begin		
				collision <= 1;
			end
			
		end
		else if  (ball_posx == cpu_posx && ball_x_vel == 1) begin
			if ((ball_posy >= cpu_posy-5) && (ball_posy < cpu_posy + 70)) begin		
				collision <= 1;
			end
		end
		
		else begin
			collision <= 0;
		end
	end

endmodule 
