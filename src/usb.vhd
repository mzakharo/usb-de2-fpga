-- usb.vhd
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
--   ISP1362 Inteface for DE2 - VHDL Model
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--    A standalone (NIOS II free) interface in VHDL to the ISP1362 USB device
--     for Altera DE2 Development board.
--
--   Typical Area :  
--     ~ 352  LEs (Cyclone II)
--   VHDL synthesis tool tested
--   	QuartusII 11 & 12
--
--   Design notes :
--
--   1. 2 Bulk endpoints (IN/OUT)
--
--   2. Transfer rate: 180 KB/s with 2 Bytes/USB Transaction.
--		ISP1362 supports up to 1MB/s with 64 Bytes/USB Transaction
--
--   3. Hardware flow control: ISP1362 can detect overflow of internal buffers. 
--		Currently overflows must be avoided by the host software application.
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use work.isp_hal.all;
use work.isp_drv.all;
use work.devreq_inc.all;


entity usb is
port(
	--clock
	CLOCK_50	: in	std_logic;
	--DE2 board config 
	OTG_FSPEED	: out		std_logic;						--USB Full Speed 
	OTG_LSPEED	: out		std_logic;						--USB Low Speed 
	--ISP1368	
	OTG_DATA	: inout		std_logic_vector(15 downto 0);	--ISP1362 Data bus 16 bits
	OTG_INT1	: in		std_logic;									--ISP1362 Interrupt 2 (Peripheral Interrupts) 
	OTG_RST_N	: out		std_logic;								--ISP1362 Reset pin
	OTG_ADDR	: out		std_logic_vector(1 downto 0);			--ISP1362 Address 2 Bits[peripheral,command]
	OTG_CS_N	: out		std_logic;									--ISP1362 Chip Select 
	OTG_RD_N	: out		std_logic;									--ISP1362 Write 
	OTG_WR_N	: out		std_logic;									--ISP1362 Read 	
	-- not used
	OTG_DACK0_N	: out		std_logic;								--ISP1362 DMA Acknowledge 1 
	OTG_DACK1_N	: out		std_logic;								--ISP1362 DMA Acknowledge 2
	--I/O...
	KEY									: in	std_logic_vector(3 downto 0);
	SW										: in	std_logic_vector(17 downto 0);
	LEDR									: out	std_logic_vector(17 downto 0);
	LEDG									: out	std_logic_vector(8 downto 0);
	HEX0,HEX1,HEX2,HEX3,HEX4,HEX5,HEX6,HEX7	: out	std_logic_vector(6 downto 0)
	);
end usb;

architecture handler of usb is

--DE2 config signals
signal clk 		: std_logic;
signal slowclk_en : bit;
signal reset, reset_synch 	: std_logic;

--USB stuff
signal hal_i : isp_hal_in_t;
signal hal_o : isp_hal_out_t;
signal drv_i : isp_drv_in_t;
signal drv_o : isp_drv_out_t;


begin --architecture

--DE2 USB config
OTG_FSPEED	<='0';						-- 0 = Enable, Z = Disable 
OTG_LSPEED	<='Z';						-- 0 = Enable, Z = Disable 


--DE2 clock and reset config
clk <= CLOCK_50;


p_reset: process
begin
	wait until rising_edge(clk);
	reset_synch <= not(KEY(0));
	reset <= reset_synch;
end process;
															   
--produces 25MHz clock enable pulse for HAL 
--assumes 50MHz clk, if this assumption changes,
--this code needs to be modified
p_slowclk_en: process
begin
	wait until rising_edge(clk);
	slowclk_en <= not(slowclk_en);
end process;


-- hal port map
hal_i.slowclk_en <= slowclk_en;
OTG_RST_N <= hal_o.rst_n ;	
OTG_ADDR <= hal_o.addr;
OTG_CS_N <= hal_o.cs_n;	
OTG_RD_N <= hal_o.rd_n;	
OTG_WR_N <= hal_o.wr_n;	
OTG_DACK0_N <= hal_o.dack0_n;	
OTG_DACK1_N <= hal_o.dack1_n;

--hal
hal_i.drv <= drv_o.hal;	
h: hal port map (clk, reset, OTG_DATA , hal_i, hal_o);

--driver port map
drv_i.int <= OTG_INT1;
--driver 
drv_i.hal <= hal_o.drv;
d: drv port map(clk,reset, drv_i , drv_o);

--device request handler
dvrq: devreq port map(clk, reset, drv_o.devreq, drv_i.devreq);

--loopback (for testing)
 drv_i.io <= drv_o.io;
 
 --debug and test LEDs
 LEDR(drv_o.io.Sdata'high downto 0) <= drv_o.io.Sdata;

end handler;
