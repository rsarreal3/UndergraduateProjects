`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   02:19:57 03/29/2017
// Design Name:   fifo_block
// Module Name:   C:/Users/saressa1/Documents/CMPE415/Xilinx Projects/fifo_hw4/pt2_tb.v
// Project Name:  fifo_hw4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: fifo_block
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module pt2_tb;

	// Inputs
	reg [15:0] data_in;
	reg rd_en;
	reg wr_en;
	reg clk;
	reg rst;

	// Outputs
	wire [15:0] data_out;
	wire is_full;
	wire is_empty;
	
	reg [10:0] f;
	integer i;

	// Instantiate the Unit Under Test (UUT)
	fifo_block uut (
		.data_out(data_out), 
		.is_full(is_full), 
		.is_empty(is_empty), 
		.data_in(data_in), 
		.rd_en(rd_en), 
		.wr_en(wr_en), 
		.clk(clk), 
		.rst(rst)
	);

	always #2 clk = !clk;
	initial begin
		// Initialize Inputs
		data_in = 0;
		rd_en = 0;
		wr_en = 0;
		clk = 0;
		rst = 1;
		f = 0;

		// Wait 100 ns for global reset to finish
		#10;
		$monitor("test#%d\t rst = %b\t full = %b\t empty = %b\t rd = %b\t wr = %b\t datain = %d\t dataout = %d\n", f, rst, is_full, is_empty, rd_en, wr_en, data_in, data_out);        
		
		#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; // data_in = 0
		#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		//----------- 260 writes -------------------------------------------------
	
		for (i = 0 ; i < 260 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; data_in = data_in + 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end
		
	
		//----------- 260 reads --------------------------------------------------
	
		for (i = 0 ; i < 260 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 0; rd_en = 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end

		
		for (i = 0 ; i < 10 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; data_in = data_in + 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end

	end
      
endmodule

