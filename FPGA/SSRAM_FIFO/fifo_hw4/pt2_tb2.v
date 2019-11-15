`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:25:17 03/29/2017
// Design Name:   IPCORE_fifo_block
// Module Name:   C:/Users/saressa1/Documents/CMPE415/Xilinx Projects/fifo_hw4/pt2_tb2.v
// Project Name:  fifo_hw4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: IPCORE_fifo_block
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module pt2_tb2;

	// Inputs
	reg rst;
	reg [7:0] wr_clk;
	reg [7:0] rd_clk;
	reg [15:0] din;
	reg wr_en;
	reg rd_en;
	
	// Outputs
	wire [15:0] dout;
	wire full;
	wire empty;
	
	reg [10:0] f;
	integer i;
	// Instantiate the Unit Under Test (UUT)
	top_fifo_block uut (
		.rst(rst), 
		.wr_clk(wr_clk), 
		.rd_clk(rd_clk), 
		.din(din), 
		.wr_en(wr_en), 
		.rd_en(rd_en), 
		.d_out(dout), 
		.full_out(full), 
		.empty_out(empty)
	);
	always #2 wr_clk = !wr_clk;
	always #2 rd_clk = !rd_clk;
	initial begin
		// Initialize Inputs
		rst = 1;
		wr_clk = 0;
		rd_clk = 0;
		din = 0;
		wr_en = 0;
		rd_en = 0;
		f = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		$monitor("test#%d\t rst = %b\t full = %b\t empty = %b\t rd = %b\t wr = %b\t datain = %d\t dataout = %d\n", f, rst, full, empty, rd_en, wr_en, din, dout);        
		
		#2; f = f+1; rst = 0; wr_en = 1; rd_en = 0; // data_in = 0
		#2; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		
		#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; // data_in = 0
		#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		//----------- 260 writes -------------------------------------------------
		
		for (i = 0 ; i < 260 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; din = din + 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end
	
		
		//----------- 260 reads --------------------------------------------------
	
	
		for (i = 0 ; i < 260 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 0; rd_en = 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end

		// write more!
		for (i = 0 ; i < 10 ; i = i+1) begin
			#6; f = f+1; rst = 0; wr_en = 1; rd_en = 0; din = din + 1;
			#4; f = f+1; rst = 0; wr_en = 0; rd_en = 0;
		end

	end
      
endmodule

