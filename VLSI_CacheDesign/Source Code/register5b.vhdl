--
-- Entity: register5b
-- Architecture : structural 
-- Author: dtran6
-- Created On: 11/10/17 at 16:16
--
library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity register5b is

  port (
		rd  		 : in  std_logic;
		wr  		 : in  std_logic;
    D		     : in  std_logic_vector(4 downto 0);
    clk      : in  std_logic;
    Q			   : out std_logic_vector(4 downto 0));
end register5b;

architecture structural of register5b is

	component cache_cell
    port (
      rd  		 : in  std_logic;
      wr  		 : in  std_logic;
      D		     : in  std_logic;
      clk      : in  std_logic;
      Q			   : out std_logic);
  end component;
	
	for cache_cell_1, cache_cell_2, cache_cell_3, cache_cell_4, cache_cell_5: cache_cell use entity work.cache_cell(structural);
		
begin

	cache_cell_1: cache_cell port map(rd, wr, D(0), clk, Q(0));
	cache_cell_2: cache_cell port map(rd, wr, D(1), clk, Q(1));
	cache_cell_3: cache_cell port map(rd, wr, D(2), clk, Q(2));
	cache_cell_4: cache_cell port map(rd, wr, D(3), clk, Q(3));
	cache_cell_5: cache_cell port map(rd, wr, D(4), clk, Q(4));
	
end structural;
