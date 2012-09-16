-- hal.vhd
-- -----------------------------------------------------------------------
-- This file is part of "ISP1362 Inteface for DE2"
--
--   "ISP1362 Inteface for DE2" is free software: you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation, version 3
--
--    "ISP1362 Inteface for DE2" is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
--
--   You should have received a copy of the GNU General Public License
--   along with "ISP1362 Inteface for DE2".  If not, see <http://www.gnu.org/licenses/>.
-- -----------------------------------------------------------------------
--   ISP1362 low-level interface 
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--     1. Implements ISP1362 I/O Timing 
--     2. More info: ISP1362 Datasheet, Chapter 19, Sec 1.2
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
package isp_hal is -- component declaration package

subtype cmd_t	is bit_vector(1 downto 0);
subtype data_t is std_logic_vector(15 downto 0);
constant dont_care_data_t : data_t := (others => '-');

constant otg_idle			: cmd_t	:= "00";
constant otg_rd			: cmd_t	:= "01";
constant otg_wr			: cmd_t	:= "10";
constant otg_wr_cmd		: cmd_t	:= "11";

type drv_iface_in_t is record
	cmd : cmd_t;
	data : data_t;	
end record;

type isp_hal_in_t is record
	drv : drv_iface_in_t;
	slowclk_en : bit;
end record;

type drv_iface_out_t is record
	data : data_t;
	rdy:	 bit;
end record;


type isp_hal_out_t is record
	drv : drv_iface_out_t;	
	rst_n	: 		std_logic;								--ISP1362 Reset pin
	addr	: 		std_logic_vector(1 downto 0);			--ISP1362 Address 2 Bits[peripheral,command]
	cs_n	: 		std_logic;									--ISP1362 Chip Select 
	rd_n	: 		std_logic;									--ISP1362 Write 
	wr_n	: 		std_logic;									--ISP1362 Read 	
	-- not used
	dack0_n	: 		std_logic;								--ISP1362 DMA Acknowledge 1 
	dack1_n	: 		std_logic;								--ISP1362 DMA Acknowledge 2
end record;

component hal is
GENERIC( 
				delay :INTEGER := 275  --for testbench
		); 
port(
	clk	: in	std_logic;
	reset : in std_logic;
	otg_data : inout std_logic_vector(15 downto 0);
	d : in isp_hal_in_t;
	q : out isp_hal_out_t);
end component;
end package;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.isp_hal.all;

entity hal is
GENERIC( 
				delay :INTEGER := 275 
		); 
port(
	clk	: in	std_logic;
	reset : in std_logic;
	otg_data : inout std_logic_vector(15 downto 0);
	d : in isp_hal_in_t;
	q : out isp_hal_out_t	);
end hal;
architecture handler of hal is

type iface_t is record
	wr : bit;
	rd : bit;
	data_out: data_t;
	rdy : bit;
end record;

type state_t is (reset_st,idle_st,write_st, writeA_st, writeB_st, idleA_st,  idleB_st, read_st, readA_st);

type reg_t is record
	reset : std_logic;
	go : bit;
	count : unsigned(8 downto 0);
	state : state_t;
	iface : iface_t;	
	rdy_out : bit;
end record;
signal r, rin : reg_t;

begin --architecture

--ISP1362 config
q.dack0_n	<= '1';	--must be set (datahseet)
q.dack1_n	<= '1';	--must be set (datasheet)
q.addr(1)	<= '1';	--always talking to the peripheral

comb : process(r,d, otg_data)
variable v : reg_t;
begin
	--parameters
	v := r; 
	
	--resets the ISP1362 by driving otg_reset low for 11us
	--assumes that slow clk enable is toggling at a rate of 25MHz
	if (r.count = delay ) then 
		v.reset := '1';
	end if;	
	--POR pulse lasts 800 ns
	if (r.count = delay + 20) then
		v.go := '1';
	end if;
	v.count := r.count + 1; 
	--end reset   
	
	v.iface.rdy := '0';
	--non registered outputs
	q.cs_n <= '1';
	q.rd_n <= '1'; 
	q.wr_n <= '1';	
	
	--handle bidir port
	otg_data <= (others => 'Z');	 --to device
	
	
	
	--I/O with the ISP1362 FSM
	case r.state is
		when reset_st =>
			if (r.go = '1') then
				v.state := idle_st;
			end if;		
	    when idle_st =>				-- 240 ns cmd / 200 ns data
			if (d.drv.cmd(0) = '1') then
			    v.state := read_st; 
			end if;
			if (d.drv.cmd(1) = '1' ) then
			    v.state := write_st; 
			end if;			
	    when write_st =>	--this will take 40 ns
	    	q.wr_n <= '0';
			q.cs_n <= '0';
			otg_data <= d.drv.data;
			v.state := writeA_st;
		when writeA_st =>   -- 80 ns
			-- data hold time after wr high (3 ns min)
			otg_data <= d.drv.data;	
			v.iface.rdy := '1';		 --signal early so that new data can be setup 			
			if (d.drv.cmd(0) = '1') then
			--In the command to data phase, the minimum value of the write command 
			--to the read data or write data cycle time must be 205 ns	(src Datasheet)
				v.state := writeB_st;
			else
				v.state := idleA_st;
			end if;
		when writeB_st => v.state := idleA_st;  -- 120 ns cmd /  80 ns data
		when idleA_st => v.state := idleB_st;   -- 160 ns cmd / 120 ns data
		when idleB_st => v.state := idle_st;    -- 200 ns cmd / 160 ns data 		
	    when read_st =>	 --read	 will take 40 ns
			q.rd_n <= '0';
			q.cs_n <= '0';
			v.state := readA_st;  
			v.iface.data_out := otg_data;	 --to interface				
		when readA_st => -- 80 ns		
			v.state := idleA_st;  		
			v.iface.rdy := '1';				
		when others => v.state := reset_st;			
	end case;
	
	v.rdy_out :=  v.iface.rdy and not(r.iface.rdy) ; --generate pulse of one cycle

	
	--outputs
	q.drv.rdy <= r.rdy_out;
	q.drv.data  <= r.iface.data_out;
	
	q.rst_n <= r.reset;
	q.addr(0) <= to_stdulogic(d.drv.cmd(0) and d.drv.cmd(1));
	
	rin <= v; --return (v);
end process;

seq : process(reset, clk)
begin
	if (reset= '1') then
		r.reset <= '0';
		r.go <= '0';
		r.count <= (others => '0');
		r.state <= reset_st;
		r.rdy_out <= '0';
	elsif rising_edge(clk) then	
		r.reset <= rin.reset; 
		r.go <= rin.go;
		r.rdy_out <= rin.rdy_out;
		if (d.slowclk_en = '1') then --slow clock	
			r.count <= rin.count; 
			r.state <= rin.state;
		end if;
	end if;
end process;
--everything in iface does not need to be reset
--since it is driven by the logic/FSM
seq_nrst : process
begin
	wait until rising_edge(clk);	
	r.iface <= rin.iface; 
end process;
end architecture;
