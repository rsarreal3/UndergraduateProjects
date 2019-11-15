--
-- Entity: or5
-- Architecture : structural
-- Author: dtran6
-- Created On: 11/13/2017 at 00:36
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity or5 is

  port (
    input1   : in  std_logic;
    input2   : in  std_logic;
    input3   : in  std_logic;
    input4   : in  std_logic;
    input5   : in  std_logic;
    output   : out std_logic);
end or5;

architecture structural of or5 is

begin

  output <= input5 or input4 or input3 or input2 or input1;

end structural;
