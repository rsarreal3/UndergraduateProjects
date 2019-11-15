--
-- Entity: nor5
-- Architecture : structural
-- Author: dtran6
-- Created On: 11/17/2017
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity nor5 is

  port (
    input1   : in  std_logic;
    input2   : in  std_logic;
    input3   : in  std_logic;
    input4   : in  std_logic;
    input5   : in  std_logic;
    output   : out std_logic);
end nor5;

architecture structural of nor5 is

  component or5
    port (
      input1   : in  std_logic;
      input2   : in  std_logic;
      input3   : in  std_logic;
      input4   : in  std_logic;
      input5   : in  std_logic;
      output   : out std_logic);
  end component;
  
	component inverter
		port (
			input    : in  std_logic;
			output   : out std_logic);
	end component;

  for or5_1: or5 use entity work.or5(structural);
  for inverter_1: inverter use entity work.inverter(structural);
  
  signal temp: std_logic;
  
begin

  or5_1: or5 port map(input1, input2, input3, input4, input5, temp);
  inverter_1: inverter port map(temp, output);

end structural;
