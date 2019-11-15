`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:34:36 04/22/2017 
// Design Name: 
// Module Name:    ball_unit 
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
module ball(
	output reg [9:0] x_pos,
	output reg [9:0] y_pos,
	output reg x_vel,
	output reg game_over,
	input collision,
	input clk,
	input rst
	
    );
	 reg y_vel;

	 parameter positive = 1;

	initial begin
		x_pos <= 300;
		y_pos <= 250;
		x_vel <= positive;
		y_vel <= positive;
	end

	always @(posedge clk) begin
		if (rst) begin
			y_vel <= positive;
			x_vel <= positive;
		end
	
		// hits a paddle
		if (collision) begin
			x_vel <= ~x_vel;
		end
		
		// collide with the upper walls
		if (y_pos >= 470 || y_pos <= 10) begin
			y_vel <= ~y_vel;
		end
	end
		
	always @(posedge clk) begin
		if (rst) begin
			game_over <= 0;
		end
		
		// go past the paddles
		else if (x_pos <= 10 || x_pos >= 625) begin
			game_over <= 1;
		end
	end
		
	always @(posedge clk) begin
		if (rst) begin
			x_pos <= 300;
			y_pos <= 250;
		end
		
		else if (game_over) begin
			//do nothing. no position moving
		end

		
		// check for ceiling collisions & adjust appropriately so not stuck
		else if (y_pos >= 470 || y_pos <= 10) begin
			if (y_pos > 320) begin
				y_pos <= y_pos - 1;
			end
			else begin
				y_pos <= y_pos + 1;
			end
		end

			// check for paddle collisions and adjust appropriately
		else if (collision) begin
			if (x_pos > 320) begin
				x_pos <= x_pos - 5;
			end
			else begin
				x_pos <= x_pos + 5;
			end
		end
	

		
		// bottom-left
		else if ((x_vel == !positive) && (y_vel == !positive)) begin
			x_pos <= x_pos - 1;
			y_pos <= y_pos - 1;
		end
		
		//top-left
		else if ((x_vel == !positive) && (y_vel == positive)) begin
			x_pos <= x_pos - 1;
			y_pos <= y_pos + 1;
		end
		
		//bottom-right
		else if ((x_vel == positive) && (y_vel == !positive)) begin
			x_pos <= x_pos + 1;
			y_pos <= y_pos - 1;
		end
		
		//top-right
		else if ((x_vel == positive) && (y_vel == positive)) begin
			x_pos <= x_pos + 1;
			y_pos <= y_pos + 1;
		end
			
	end



		
endmodule
