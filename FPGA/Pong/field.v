`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:58:13 04/22/2017 
// Design Name: 
// Module Name:    field 
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
module field(
    output reg red,
    output reg green,
    output reg blue
    );
	parameter FIElD_WIDTH = 640;
	parameter FIELD_HEIGHT = 480;
	parameter X_LEFT = 320;
	parameter Y_BOTTOM = 240;
	
	//addinal intermediate logic signal wires
	wire flag_on_rect; // high only when over rectangle
	wire [9:0] x,y; // traditional cartesian coordinates, (left,bottom)=(0,0)
	
	//combinational logic to calculate x,y coordinate system
	assign x = pos_h;
	assign y = 480 - pos_v;
	
	//combinational logic to decide if beam is over a rectangle
	assign flag_on_rect = x >= (X_LEFT)				&&
								 x <  (X_LEFT + WIDTH)  &&
								 y >= (Y_BOTTOM)			&&
								 y <  (Y_BOTTOM + HEIGHT);
								  
	// combinational logic and registers that load on rising clock edge
	always @(posedge clk) begin

					
	end
endmodule


				

/*


	parameter WIDTH = 640;
	parameter HEIGHT = 480;
	parameter X_LEFT = 320;
	parameter Y_BOTTOM = 240;
	
	//addinal intermediate logic signal wires
	wire flag_on_rect; // high only when over rectangle
	wire [9:0] x,y; // traditional cartesian coordinates, (left,bottom)=(0,0)
	
	//combinational logic to calculate x,y coordinate system
	assign x = pos_h;
	assign y = 480 - pos_v;
	
	//combinational logic to decide if beam is over a rectangle
	assign flag_on_rect = x >= (X_LEFT)				&&
								 x <  (X_LEFT + WIDTH)  &&
								 y >= (Y_BOTTOM)			&&
								 y <  (Y_BOTTOM + HEIGHT);
								  
	// combinational logic and registers that load on rising clock edge
	always @(posedge clk)
		if ((sw0 & sw1) | (sw0 & sw2) | (sw1 & sw2)) begin
			red	<= ~flag_on_rect & blank;
			green <= ~flag_on_rect & blank;
			blue  <= ~flag_on_rect & blank;
		end

		else if (sw0 == 1) begin
			red	<= flag_on_rect & ~blank;
			green <= flag_on_rect & ~blank;
			blue  <= ~flag_on_rect & blank;
			
		end
		else if (sw1 == 1) begin
			red	<= flag_on_rect & ~blank;
			green <= ~flag_on_rect & blank;
			blue  <= flag_on_rect & ~blank;
		end 
		
		else if (sw2 == 1) begin
			red	<= ~flag_on_rect & blank;
			green <= flag_on_rect & ~blank;
			blue  <= flag_on_rect & ~blank;
		end
		

		else begin
			red	<= ~flag_on_rect & blank;
			green <= ~flag_on_rect & blank;
			blue  <= ~flag_on_rect & blank;
		end			

endmodule
*/

