`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    01:10:13 03/29/2017 
// Design Name: 
// Module Name:    fifo_block 
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

module fifo_block(
	output reg [15:0] data_out,
	output reg is_full,
	output reg is_empty,
	input [15:0] data_in,
	input rd_en,
	input wr_en,
	input clk,
	input rst
	);

	reg [15:0] dualport_FIFO_SSRAM [0:255];
	wire [7:0] a_wr, a_rd;
	wire a_isEqual;
	wire rd_blk_en, wr_blk_en;
	wire set_stat, rst_stat;
	reg status;
	wire preFull, preEmpty;
	
	// handle whether or not we can actually read or write (depending on if full/empty)
	assign rd_blk_en = rd_en & ~is_empty;
	assign wr_blk_en = wr_en & ~is_full;
	
	// handle writing & reading
	always @(posedge clk) begin
		if (rd_blk_en) begin
			data_out <= dualport_FIFO_SSRAM[a_rd];
		end
		
		if (wr_blk_en) begin
			dualport_FIFO_SSRAM[a_wr] <= data_in;
		end
	end
	
	// counters:
	grey_code_counter wr(
		.grey_cnt(a_wr),
		.en(wr_blk_en),
		.rst(rst),
		.clk(clk)
	);
	
	grey_code_counter rd(
		.grey_cnt(a_rd),
		.en(rd_blk_en),
		.rst(rst),
		.clk(clk)
	);
		
	assign a_isEqual = (a_rd == a_wr);
	assign set_stat = ((a_wr[6] ~^ a_rd[7]) & (a_wr[7]  ^ a_rd[6]));
	assign rst_stat = ((a_wr[6]  ^ a_rd[7]) & (a_wr[7] ~^ a_rd[6]));
	
	/*Status is high when there is possiblity for the address and the read to become the same*/
	/*Status is low when there is no possibility*/
	always @ (set_stat, rst_stat, rst) begin
		if(rst_stat | rst) begin
			status = 0;
		end
		else if (set_stat) begin
			status = 1;
		end
	end

	// is full?
	assign preFull = status & a_isEqual;
	// is empty?
	assign preEmpty = ~status & a_isEqual;
	
	// UPDATE is_full & is_empty
	always@(posedge clk, posedge preFull) begin
		if (preFull) begin
			is_full <= 1;
		end
		else begin
			is_full <= 0;
		end
	end


	always@(posedge clk, posedge preEmpty) begin
		if (preEmpty) begin
			is_empty <= 1;
		end
		else begin
			is_empty <= 0;
		end
	end		
	
endmodule 

// greycode because we are only changing one bit at a time
// which will cause less error
module grey_code_counter(
	output reg [7:0] grey_cnt,
	input en,
	input rst,
	input clk
	);
	
	reg [7:0] cnt;
	always@(posedge clk) begin
		if (rst) begin
			cnt <= 1;
			grey_cnt <= 0;
		end
		else if (en) begin
			cnt <= cnt + 1;
			grey_cnt <= {cnt[7], cnt[6:0] ^ cnt[7:1]};
		end
	end
endmodule
	
	