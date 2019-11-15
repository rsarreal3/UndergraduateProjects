`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:34:56 04/22/2017 
// Design Name: 
// Module Name:    ball_delay_unit 
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
module ball_delay(
     p,
     clk,
	  rst,
	  collision
    );

  input clk;
  input rst;
  input collision;
  output p;
  
  reg 	p;
  reg [32:0] count;
  reg [32:0] PERIOD;
  reg [32:0] PULSE_WIDTH;
  reg prev_collision;
	
  initial //for simulation only
    begin
     count <= 0;
	  PERIOD <= 312500;
    end   
	
	always @(posedge clk) begin
		prev_collision <= collision;
	end
	

  always @ (posedge clk) begin
		if (rst) begin
			PERIOD <= 312500;
			count <= 0;
		end
		
	   if (prev_collision == 0 && collision == 1) begin
			if (PERIOD >= 30000) begin
				PERIOD <= PERIOD - 30000;
			end
		end
			
		 if (count >= (PERIOD - 1))begin
	        count <= 0;
       end
		  		  
      else begin
          count <= count+1;
        end 
		p <= (count == 0);	  
    end
	 
endmodule 

/*	 
 // ball needs to be 312500 slower than the 20ns clock
  parameter PERIOD = 312500;   
  parameter PULSE_WIDTH = 125000; //40% high

  input clk;
  input rst;
  input collision;
  output p;
   
  reg 	p;
  reg [32:0] count;
   

  initial //for simulation only
    begin
     count <= 0;
    end   

  always @ (posedge clk)
    begin
      if (count == (PERIOD - 1))
  		  begin
	        count <= 0;
        end
      else 
        begin
          count <= count+1;       
        end

    end

  always @ (posedge clk)
    begin
       p = count < PULSE_WIDTH;       
    end

endmodule 
*/