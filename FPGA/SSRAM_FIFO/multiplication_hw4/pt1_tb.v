`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   00:25:51 03/28/2017
// Design Name:   top_multiplication
// Module Name:   C:/Users/Ressa/Documents/CMPE415/Xilinx Projects/multiplication_hw4/pt1_tb.v
// Project Name:  multiplication_hw4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: top_multiplication
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module pt1_tb;

	// Inputs
	reg [15:0] a_din;
	reg [15:0] b_din;
	reg clk;
	reg rst;
	reg go;

	// Outputs
	wire done;
	wire [2:0] current_state;
	wire [6:0] ab_cnt;
	wire [31:0] p_out;
	

	// Instantiate the Unit Under Test (UUT)
	top_multiplication uut (
		.done(done),  // out
		.current_state(current_state), //out 
		.p_out(p_out),
		.ab_cnt(ab_cnt),
		.a_din(a_din), //in
		.b_din(b_din), //in
		.clk(clk), //in
		.rst(rst), //in
		.go(go) //in
	);

	always #1 clk = !clk;

	
	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 1;
		go = 0;
		a_din = 0;
		b_din = 0;
		// Wait 100 ns for global reset to finish
		#100;
       
		// Add stimulus here
		$monitor("go= %b\t done = %b\t rst = %b\t i = x  = %d\t p(i) = x^2 = %d\n", go, done, rst, ab_cnt, p_out);
		#10; rst = 1'b0; go = 1'b1;
		#10; rst = 1'b0; go = 1'b0;
	end
      
		
endmodule

