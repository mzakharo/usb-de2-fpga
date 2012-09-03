library ieee;
use ieee.std_logic_1164.all;
package ENTITY_NAME_inc is -- component declaration package


type ENTITY_NAME_in_t is record
	en : bit;
end record;
type ENTITY_NAME_out_t is record
	rdy:	 bit;
end record;

component ENTITY_NAME is
port(
	clk	: in	std_logic;
	reset : in std_logic;
	d : in ENTITY_NAME_in_t;
	q : out ENTITY_NAME_out_t);
end component;
end package;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.ENTITY_NAME_inc.all;

entity ENTITY_NAME is
port(
	clk	: in	std_logic;
	reset : in std_logic;
	d : in ENTITY_NAME_in_t;
	q : out ENTITY_NAME_out_t);
end ENTITY_NAME;
architecture handler of ENTITY_NAME is

type not_reset_t is record
	dummy : bit;
end record;

type state_t is (idle );

type reg_t is record
	nr : not_reset_t;
	state : state_t;
end record;
signal r, rin : reg_t;

begin --architecture


comb : process(r,d)
variable v : reg_t;
begin
	--parameters
	v := r; 
	
	--algorithm goes here
	
	rin <= v; --return (v);
end process;

seq : process(reset, clk)
begin
	if (reset= '1') then
		r.state <= idle;
	elsif rising_edge(clk) then	
		r.state <= rin.state; 
	end if;
end process;
--everything in iface does not need to be reset
seq_nrst : process
begin
	wait until rising_edge(clk);	
	r.nr <= rin.nr; 
end process;

end architecture;
