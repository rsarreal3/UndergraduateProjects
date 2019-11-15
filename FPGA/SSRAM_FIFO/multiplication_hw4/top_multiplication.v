`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:42:01 03/27/2017 
// Design Name: 
// Module Name:    top_multiplication 
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
module top_multiplication(
    output reg done,
	 output [31:0] p_out,
	 output reg [2:0] current_state,
	 output reg [6:0] ab_cnt,
	 output reg [6:0] p_cnt,
	 input [15:0] a_din,
	 input [15:0] b_din,
    input clk,
    input rst,
	 input go
    );


////////////////////////////////////////////

	// Outputs
	reg p_wr;
	reg ab_wr;
	wire [31:0] product;
	wire [15:0] a_dout, b_dout;
	reg [15:0] a_out, b_out;
	wire [31:0] p_dout;
	
	///////////////////////////////////////////
	// memory handling //
	SSRAM_16w_a SSRAM_a(
		.clk(clk), 
		.address(ab_cnt), 
		.wr_en(ab_wr), 
		.data_in(a_din),
		.data_out(a_dout)
	);
	SSRAM_16w_b SSRAM_b(
		.clk(clk), 
		.address(ab_cnt), 
		.wr_en(ab_wr), 
		.data_in(b_din),
		.data_out(b_dout)
	);
	SSRAM_32w SSRAM_p(
		.clk(clk), 
		.address(p_cnt), 
		.wr_en(p_wr), 
		.data_in(product),
		.data_out(p_dout)
	);


	///////////////////////////////////////////
//	 reg [6:0] cnt;
	 reg cnt_rst;
	 reg cnt_end;
	 reg ab_cnt_en, p_cnt_en;

	 parameter [2:0] idle = 3'b000;
	 parameter [2:0] get_ab = 3'b001;
	 parameter [2:0] get_ab_last = 3'b010;
	 parameter [2:0] store_prod = 3'b011;
	 parameter [2:0] finish = 3'b100;
	 reg [2:0] next_state;
	 
	 always @(posedge clk or posedge rst) begin
		if(rst) begin
			current_state <= idle;
		end
		else begin
			current_state <= next_state;
		end
	 end
			
	reg is_first, is_last;
	 always @ * begin //state machine
		ab_wr = 1'b0; // we dont need to write because data is initialized in RAM
		case (current_state)
			idle:
				begin
					if (!go) begin // if not beginning yet
						next_state = idle;
					end
					else begin
						next_state = get_ab;
					end
					p_wr = 1'b0;
					cnt_rst = 1'b1;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
					done = 1'b0;
					is_first = 1'b1;
					is_last = 1'b0;
				end
			get_ab:
				begin
					next_state = store_prod;
					p_wr = 1'b0;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b1;
					if (is_first) begin
						p_cnt_en = 1'b0;
						is_first = 1'b0;
					end
					else begin
						p_cnt_en = 1'b1;
					end
				end
			get_ab_last:
				begin
					next_state = store_prod;
					p_wr = 1'b0;
					is_last = 1'b1;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b1;
				end
			store_prod:
				begin
					if (cnt_end && !is_last) begin
						next_state = get_ab_last;
					end
					else if (is_last) begin
						next_state = finish;
					end
					else begin
						next_state = get_ab;
					end
					p_wr = 1'b1;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
				end
			finish: 
				begin
					next_state = idle;
					done = 1'b1;
					p_wr = 1'b0;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
				end
		endcase
	end
	
	
	// handle counter	s
	always @(posedge clk) begin
		if (cnt_rst) begin
			ab_cnt <= 0;
		end
		else if (ab_cnt_en) begin
			ab_cnt <= ab_cnt + 1;
		end
		
		if (ab_cnt == 7'b1111111) begin
			cnt_end <= 1'b1;
		end
		else begin
			cnt_end <= 1'b0;
		end
	end


	always @(posedge clk) begin
		if (cnt_rst) begin
			p_cnt <= 0;
		end
		else if (p_cnt_en) begin
			p_cnt <= p_cnt + 1;
		end
	end

	assign p_out = p_dout;

	always @(posedge clk) begin
		a_out <= a_dout;
		b_out <= b_dout;
	end

	// handle multiplication
	assign  product = {{16{b_out[15]}},a_out} * {{16{b_out[15]}},b_dout};
	initial $monitor("done = %b",done);
	
endmodule


//-------------------------------------------------------------------

module SSRAM_16w_a(
	output reg [15:0] data_out,
 	input clk,
	input [6:0] address,
	input wr_en,
	input [15:0] data_in
	);
	
	reg [15:0] SSRAM [0:127];
	
	initial begin
		$readmemh("SSRAM_a.txt", SSRAM);
	end
	
	// write to the memory
	always @(posedge clk) begin 	
		if (wr_en) begin
			SSRAM[address] <= data_in;
		end
		else begin
			data_out <= SSRAM[address];
		end
	end
	
endmodule






//-------------------------------------------------------------------

module SSRAM_16w_b(
	output reg [15:0] data_out,
 	input clk,
	input [6:0] address,
	input wr_en,
	input [15:0] data_in
	);
	
	reg [15:0] SSRAM [0:127];
	
	initial begin
		$readmemh("SSRAM_b.txt", SSRAM);
	end
	
	// write to the memory
	always @(posedge clk) begin 	
		if (wr_en) begin
			SSRAM[address] <= data_in;
		end
		else begin
			data_out <= SSRAM[address];
		end
	end
	
endmodule




//-------------------------------------------------------------------


module SSRAM_32w(
	output reg [31:0] data_out,
 	input clk,
	input [6:0] address,
	input wr_en,
	input [31:0] data_in
	);
	
	reg [31:0] SSRAM [0:127];
	
	// write to the memory
	
	always @(posedge clk) begin 	
		if (wr_en) begin
			SSRAM[address] <= data_in;
		end
		else begin
			data_out <= SSRAM[address];
		end
	end
	
endmodule



//-------------------------------------------------------------------

module top_multiplication_IPCORE(
     output reg done,
	 output [31:0] p_out,
	 output reg [2:0] current_state,
	 output reg [6:0] ab_cnt,
	 output reg [6:0] p_cnt,
	 input [15:0] a_din,
	 input [15:0] b_din,
    input clk,
    input rst,
	 input go
    );



////////////////////////////////////////////

	// Outputs
	reg p_wr;
	reg ab_wr;
	wire [31:0] product;
	wire [15:0] a_dout, b_dout;
	reg [15:0] a_out, b_out;
	wire [31:0] p_dout;
	
	///////////////////////////////////////////
	// memory handling //
IPCORE_SSRAM_a a(
  .clka(clk), // input clka
  .wea(ab_wr), // input [0 : 0] wea
  .addra(ab_cnt), // input [6 : 0] addra
  .dina(a_din), // input [15 : 0] dina
  .douta(a_dout) // output [15 : 0] douta
);

IPCORE_SSRAM_a b(
  .clka(clk), // input clka
  .wea(ab_wr), // input [0 : 0] wea
  .addra(ab_cnt), // input [6 : 0] addra
  .dina(b_din), // input [15 : 0] dina
  .douta(b_dout) // output [15 : 0] douta
);


IPCORE_SSRAM_32w p(
  .clka(clk), // input clka
  .wea(p_wr), // input [0 : 0] wea
  .addra(p_cnt), // input [6 : 0] addra
  .dina(product), // input [31 : 0] dina
  .douta(p_dout) // output [31 : 0] douta
);
	///////////////////////////////////////////
	 reg cnt_rst;
	 reg cnt_end;
	 reg ab_cnt_en, p_cnt_en;

	 parameter [2:0] idle = 3'b000;
	 parameter [2:0] get_ab = 3'b001;
	 parameter [2:0] get_ab_last = 3'b010;
	 parameter [2:0] store_prod = 3'b011;
	 parameter [2:0] finish = 3'b100;
	 reg [2:0] next_state;
	 
	 always @(posedge clk or posedge rst) begin
		if(rst) begin
			current_state <= idle;
		end
		else begin
			current_state <= next_state;
		end
	 end
			
	reg is_first, is_last;
	 always @ * begin //state machine
		ab_wr = 1'b0; // we dont need to write because data is initialized in RAM
		case (current_state)
			idle:
				begin
					if (!go) begin // if not beginning yet
						next_state = idle;
					end
					else begin
						next_state = get_ab;
					end
					p_wr = 1'b1;
					cnt_rst = 1'b1;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
					done = 1'b0;
					is_first = 1'b1;
					is_last = 1'b0;
				end
			get_ab:
				begin
					next_state = store_prod;
					p_wr = 1'b0;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b1;
					if (is_first) begin
						p_cnt_en = 1'b0;
						is_first = 1'b0;
					end
					else begin
						p_cnt_en = 1'b1;
					end
				end
			get_ab_last:
				begin
					next_state = store_prod;
					p_wr = 1'b0;
					is_last = 1'b1;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b1;
				end
			store_prod:
				begin
					if (cnt_end && !is_last) begin
						next_state = get_ab_last;
					end
					else if (is_last) begin
						next_state = finish;
					end
					else begin
						next_state = get_ab;
					end
					p_wr = 1'b1;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
				end
			finish: 
				begin
					next_state = idle;
					done = 1'b1;
					p_wr = 1'b0;
					cnt_rst = 1'b0;
					ab_cnt_en = 1'b0;
					p_cnt_en = 1'b0;
				end
		endcase
	end
	
	
	// handle counter	s
	always @(posedge clk) begin
		if (cnt_rst) begin
			ab_cnt = 0;
		end
		else if (ab_cnt_en) begin
			ab_cnt = ab_cnt + 1;
		end
		
		if (ab_cnt == 7'b1111111) begin
			cnt_end = 1'b1;
		end
		else begin
			cnt_end = 1'b0;
		end
	end


	always @(posedge clk) begin
		if (cnt_rst) begin
			p_cnt = 0;
		end
		else if (p_cnt_en) begin
			p_cnt = p_cnt + 1;
		end
	end

	assign p_out = p_dout;

	always @(posedge clk) begin
		a_out <= a_dout;
		b_out <= b_dout;
	end

	// handle multiplication
	assign  product = {{16{b_out[15]}},a_out} * {{16{b_out[15]}},b_dout};
	initial $monitor("done = %b",done);
	
endmodule

