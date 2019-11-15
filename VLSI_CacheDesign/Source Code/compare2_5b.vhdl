--
-- Entity: compare2_5b
-- Architecture : structural
-- Author: dtran6
-- Created On: 11/10/2017
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity compare2_5b is

  port (
    input1   : in  std_logic_vector(4 downto 0);
    input2   : in  std_logic_vector(4 downto 0);
    output   : out std_logic);
end compare2_5b;

architecture structural of compare2_5b is

component xnor2
  port (
    input1    : in  std_logic;
    input2    : in  std_logic;
    output    : out std_logic);
end component;

component and5
  port (
    input1    : in  std_logic;
    input2    : in  std_logic;
    input3    : in  std_logic;
    input4    : in  std_logic;
    input5    : in  std_logic;
    output    : out std_logic);
end component;
  
for xnor2_1, xnor2_2, xnor2_3, xnor2_4, xnor2_5: xnor2 use entity work.xnor2(structural);
for and5_1: and5 use entity work.and5(structural);

signal temp1, temp2, temp3, temp4, temp5: std_logic;

begin

  xnor2_1: xnor2 port map (input1(0), input2(0), temp1);
  xnor2_2: xnor2 port map (input1(1), input2(1), temp2);
  xnor2_3: xnor2 port map (input1(2), input2(2), temp3);
  xnor2_4: xnor2 port map (input1(3), input2(3), temp4);
  xnor2_5: xnor2 port map (input1(4), input2(4), temp5);

  and5_1: and5 port map (temp1, temp2, temp3, temp4, temp5, output);

end structural;
