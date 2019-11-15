--
-- Entity: nand3
-- Architecture : structural
-- Author: saressa1
-- Created On: 11/1/2017
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity nand3 is

  port (
    input1   : in  std_logic;
    input2   : in  std_logic;
    input3   : in  std_logic;
    output   : out std_logic);
end nand3;

architecture structural of nand3 is

component and3
  port (
    input1   : in  std_logic;
    input2   : in  std_logic;
    input3   : in  std_logic;
    output   : out std_logic);
end component;

component inverter
  port (
    input    : in  std_logic;
    output   : out std_logic);
end component;

for and3_1: and3 use entity work.and3(structural);
for inverter_1: inverter use entity work.inverter(structural);

signal temp: std_logic;

begin

  and3_1: and3 port map (input1, input2, input3, temp);

  inverter_1: inverter port map (temp, output);

end structural;
