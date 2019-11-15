--
-- Entity: nor8
-- Architecture : structural
-- Author: saressa1
-- Created On: 11/13/2017
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity nor8 is

  port (
    input1   : in  std_logic;
    input2   : in  std_logic;
    input3   : in  std_logic;
    input4   : in  std_logic;
    input5   : in  std_logic;
    input6   : in  std_logic;
    input7   : in  std_logic;
    input8   : in  std_logic;
    output   : out std_logic);
end nor8;

architecture structural of nor8 is

  component or8
    port (
      input1   : in  std_logic;
      input2   : in  std_logic;
      input3   : in  std_logic;
      input4   : in  std_logic;
      input5   : in  std_logic;
      input6   : in  std_logic;
      input7   : in  std_logic;
      input8   : in  std_logic;
      output   : out std_logic);
  end component;
  
	component inverter
		port (
			input    : in  std_logic;
			output   : out std_logic);
	end component;
  
  for or8_1: or8 use entity work.or8(structural);
  for inverter_1: inverter use entity work.inverter(structural);
  
  signal temp: std_logic;

begin

  or8_1: or8 port map(input1, input2, input3, input4, input5, input6, input7, input8, temp);
  inverter_1: inverter port map(temp, output);

end structural;