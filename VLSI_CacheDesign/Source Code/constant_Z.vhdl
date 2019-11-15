--
-- Entity: constant_Z
-- Architecture : structural
-- Author: dtran6
-- Created On: 11/18/2017
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity constant_Z is

  port (
    tie_Z   : out std_logic);
end constant_Z;

architecture structural of constant_Z is

begin

  tie_Z <= 'Z';

end structural;
