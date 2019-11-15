--
-- Entity: two_way_SA_cache
-- Architecture: structural
-- Author: saressa1
-- Created On: 11/13/2017
--

library STD;
library IEEE;
use IEEE.std_logic_1164.all;

entity two_way_SA_cache is
	port (
		CA    : in   std_logic_vector(7 downto 0);
		CD    : in   std_logic_vector(7 downto 0);
		start : in   std_logic;
		rd/wr : in   std_logic;
		MA    : in   std_logic_vector(7 downto 0);
		MD    : in   std_logic_vector(7 downto 0);
		GND   : in   std_logic;
		VDD   : in   std_logic;
		CLK   : in   std_logic;
		RST   : in   std_logic;
		busy  : out  std_logic;
		enable: out  std_logic);
end two_way_SA_cache;

architecture structural of two_way_SA_cache is

component state_machine
	port (
		start    : in  std_logic;
		rd_wr 	 : in  std_logic;
		data_in	 : in  std_logic_vector(7 downto 0);
		address	 : in  std_logic_vector(7 downto 0);
    	hit_miss_0 : in std_logic;
    	hit_miss_1 : in std_logic;
    	rst	     : in  std_logic;
    	clk      : in  std_logic;
		rd_0     : out std_logic;
    	rd_1     : out std_logic;
    	wr_0     : out std_logic;
    	wr_1     : out std_logic;
    	busy     : out std_logic);
end component;


component cache8x4B
	port (
		start    : in  std_logic;
		data_in	 : in  std_logic_vector(7 downto 0);
		address	 : in  std_logic_vector(7 downto 0);
		rd  	 : in  std_logic;
		wr       : in  std_logic;
    	rst	     : in  std_logic;
    	clk      : in  std_logic;
		hit_miss : out std_logic;
		data_out : out std_logic_vector(7 downto 0));
end component;


for state_machine_1: state_machine use entity work.state_machine(structural);
for cache_0, cache_1: cache8x4B use entity work.cache8x4B(structural);

signal hit_miss_0, hit_miss_1 : std_logic;
signal rd_0, rd_1, wr_0, wr_1 : std_logic;

state_machine_1: state_machine port map(start, rd_wr, hit_miss_0, hit_miss, 1, RST, CLK, rd_0, rd_1, wr_0, wr_1, busy);

//cache_0: cache8x4B port_map(start, CD, CA, )
