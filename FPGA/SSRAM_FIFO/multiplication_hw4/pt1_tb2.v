`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   00:08:26 03/29/2017
// Design Name:   top_multiplication_IPCORE
// Module Name:   C:/Users/saressa1/Documents/CMPE415/Xilinx Projects/multiplication_hw4/pt1_tb2.v
// Project Name:  multiplication_hw4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: top_multiplication_IPCORE
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module pt1_tb2;

	reg [15:0] a_din;
	reg [15:0] b_din;
	reg clk;
	reg rst;
	reg go;

	// Outputs
	wire done;
	wire [31:0] p_out;
	wire [2:0] current_state;
	wire [6:0] ab_cnt;
	wire [6:0] p_cnt;

	// Instantiate the Unit Under Test (UUT)
	top_multiplication_IPCORE uut (
		.done(done), 
		.p_out(p_out), 
		.current_state(current_state), 
		.ab_cnt(ab_cnt), 
		.p_cnt(p_cnt), 
		.a_din(a_din), 
		.b_din(b_din), 
		.clk(clk), 
		.rst(rst), 
		.go(go)
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
		#100; rst = 1'b0; go = 1'b1;
		#10; rst = 1'b0; go = 1'b0;
	end
endmodule

