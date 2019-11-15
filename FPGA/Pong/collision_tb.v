`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:10:56 04/25/2017
// Design Name:   collision_detector
// Module Name:   C:/Users/Ressa/Documents/CMPE415/Xilinx Projects/HW6_pong/collision_tb.v
// Project Name:  HW6_pong
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: collision_detector
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module collision_tb;

	// Inputs
	reg [9:0] ball_posx;
	reg [9:0] ball_posy;
	reg [9:0] p1_posy;
	reg [9:0] cpu_posy;
	reg ball_x_vel;
	reg clk;

	// Outputs
	wire collision;

	// Instantiate the Unit Under Test (UUT)
	collision_detector uut (
		.ball_posx(ball_posx), 
		.ball_posy(ball_posy), 
		.p1_posy(p1_posy), 
		.cpu_posy(cpu_posy), 
		.ball_x_vel(ball_x_vel), 
		.clk(clk), 
		.collision(collision)
	);
	
	always #5 clk = ~clk;

	initial begin
		// Initialize Inputs
		ball_posx = 0;
		ball_posy = 0;
		p1_posy = 0;
		cpu_posy = 0;
		ball_x_vel = 0;
		clk = 0;

		// Wait 100 ns for global reset to finish
		#100;
      
		// Add stimulus here
		$monitor("ball_pos(%d, %d) p1[%d, %d] cpu[%d, %d] collision: %d", ball_posx, ball_posy, p1_posy, p1_posy+64, cpu_posy, cpu_posy+64, collision);
		
		ball_posx = 320; ball_posy = 240; p1_posy = 80; cpu_posy = 240;
		#20;
		ball_posx = 64; ball_posy = 240; p1_posy = 240; cpu_posy = 240;
		#20;
		ball_posx = 64; ball_posy = 0; p1_posy = 400; cpu_posy = 240;
		#20;
		ball_posx = 576; ball_posy = 240; p1_posy = 80; cpu_posy = 240;
		#20;
		ball_posx = 576; ball_posy = 0; p1_posy = 80; cpu_posy = 400;
		#20;
	end
      
endmodule

