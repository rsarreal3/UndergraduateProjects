`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:01:04 09/10/2010 
// Design Name: 
// Module Name:    rotator_oneshot 
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
module rotator_oneshot(
    rot_l_oneshot,
    rot_r_oneshot,
    clk,
    rst,
    rot_a,
    rot_b
    );

    output rot_l_oneshot;
    output rot_r_oneshot;
    input clk;
    input rst;
    input rot_a;
    input rot_b;
    
    wire a = rot_a;
    wire b = rot_b;
    
    reg rot_l_oneshot;
    reg rot_r_oneshot;
    reg a_prev,b_prev;

//NET "rotary_a"     LOC = "K18" | IOSTANDARD = LVTTL | PULLUP;
//NET "rotary_b"     LOC = "G18" | IOSTANDARD = LVTTL | PULLUP;
//NET "rotary_press" LOC = "V16" | IOSTANDARD = LVTTL | PULLDOWN;



   always @ (posedge clk) begin

      if (a==1 && a_prev==0) begin //rising a
        if (b != b_prev) begin    //use b to detect noise repeat of rising a
	        if (b == 1) begin //flag detected direction based on b
	          rot_r_oneshot =1;
            rot_l_oneshot =0;
          end else begin
	          rot_r_oneshot=0;
	          rot_l_oneshot=1;
          end
        end else begin // if false edge then dont flag a rotation
   	      rot_r_oneshot=0; 
          rot_l_oneshot=0;
        end
        b_prev <= b;
     end else if (a==0 && a_prev==1) begin //falling a
        b_prev <= b;
	   end else begin // if rising a not detected don't flag a rotation
 	      rot_r_oneshot=0; 
        rot_l_oneshot=0;
     end // if (a==1 && a_prv==0)
     
     
     a_prev <= a;
     
   end // always @ (posedge clk)
   
   
endmodule