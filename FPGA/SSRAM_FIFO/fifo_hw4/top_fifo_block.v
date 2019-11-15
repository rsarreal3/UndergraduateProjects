`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:32:58 03/30/2017 
// Design Name: 
// Module Name:    top_fifo_block 
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
module top_fifo_block(
	output [15:0] d_out,
	output full_out,
	output empty_out,
	input [15:0] din,
	input rst,
	input [7:0] rd_clk,
	input [7:0] wr_clk,
	input rd_en,
	input wr_en
   );
	
	wire [15:0] dout;
	wire full, empty;
	assign d_out = dout;
	assign full_out = full;
	assign empty_out = empty;
	
	IPCORE_fifo_block uut (
		.rst(rst), 
		.wr_clk(wr_clk), 
		.rd_clk(rd_clk), 
		.din(din), 
		.wr_en(wr_en), 
		.rd_en(rd_en), 
		.dout(dout), 
		.full(full), 
		.empty(empty)
	);

endmodule
