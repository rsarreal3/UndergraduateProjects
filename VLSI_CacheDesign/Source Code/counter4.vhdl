--
-- Entity: counter4
-- Architecture : structural 
-- Author: dtran6
-- Created On: 11/1/17 at 1:58
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity counter4 is

  port (
    rst			 : in  std_logic;
    clk      : in  std_logic;
    count    : out std_logic_vector(1 downto 0));
end counter4;

architecture structural of counter4 is
  
	component dff	
		port ( 
			D        : in  std_logic;
      clk      : in  std_logic;
      Q        : out std_logic;
      not_Q    : out std_logic);
	end component;
	
	component and2
		port (
			input1   : in  std_logic;
			input2   : in  std_logic;
			output   : out std_logic);
	end component;
	
	component xor2
		port (
			input1   : in  std_logic;
			input2   : in  std_logic;
			output   : out std_logic);
	end component;
	
	component inverter
	  port (
    input    : in  std_logic;
    output   : out std_logic);
	end component;

	for dff_1, dff_2: dff use entity work.dff(structural);
	for and2_1, and2_2: and2 use entity work.and2(structural);
	for xor2_1: xor2 use entity work.xor2(structural);
	for inverter_1: inverter use entity work.inverter(structural);
	
	signal not_rst: std_logic;
	signal new_count1_value, new_count0_value: std_logic;
	signal inc_count1_value: std_logic;
	signal count1_value, count0_value: std_logic;
	signal not_count1_value, not_count0_value: std_logic;
	signal not_rst_and_count1, not_rst_and_count0: std_logic;
	
begin

	dff_1: dff port map(new_count0_value, clk, count0_value, not_count0_value);
	dff_2: dff port map(new_count1_value, clk, count1_value, not_count1_value);
	inverter_1: inverter port map(rst, not_rst);
	and2_1: and2 port map(not_rst, not_count0_value, new_count0_value);
	and2_2: and2 port map(not_rst, inc_count1_value, new_count1_value);
	xor2_1: xor2 port map(count1_value, count0_value, inc_count1_value);
	
end structural;
