library work;
use work.isp_hal.all;
use work.isp_drv.all;
library ieee;
use ieee.NUMERIC_STD.all;
use ieee.std_logic_1164.all;

	-- Add your library and packages declaration here ...

entity drv_tb is
end drv_tb;

architecture TB_ARCHITECTURE of drv_tb is
	-- Stimulus signals - signals mapped to the input and inout ports of tested entity
	signal clk : STD_LOGIC;
	signal reset : STD_LOGIC;
	signal otg_data : STD_LOGIC_VECTOR(15 downto 0);
	
	signal otg_i : isp_hal_in_t;
	signal otg_o : isp_hal_out_t;

	signal drv_i : isp_drv_in_t;
	signal drv_o : isp_drv_out_t;

	--local signals
	signal slowclk_en : bit;	
	
	signal isp_emu_data : std_logic_vector(15 downto 0);
	
	
  --------------------------------------------------------------
  -- clock cycle

  constant period  : time := 20 ns;

  --------------------------------------------------------------

begin


	--I/O
	otg_i.drv <= drv_o.hal;	
	h: hal generic map(3) port map (clk, reset, otg_data , otg_i, otg_o);

	--driver 
	drv_i.hal <= otg_o.drv;
	d: drv port map(clk,reset, drv_i , drv_o);


	
	
	----------------------------------------------------
    -- clock
    process
    begin
      clk <= '0';
      wait for period/2;
      clk <= '1';
      wait for period/2;
    end process;
	--produces 25MHz clock enable for OTG 
	p_slowclk_en: process
	begin
		wait until rising_edge(clk);
		slowclk_en <= not(slowclk_en);
	end process;  
	otg_i.slowclk_en <= slowclk_en; 

	----------------------------------------------------
    -- resets
    process
    begin
      reset <= '1';
      wait for period;
      reset <= '0';
      wait;
    end process;    
    ----------------------------------------------------   
    -- ISP read emulation
    ----------------------------------------------------   
	process
	begin 
		otg_data <= (others => 'Z');    
		wait until falling_edge(otg_o.rd_n);
		wait for 22 ns;
		otg_data <= isp_emu_data;
		wait until rising_edge(otg_o.cs_n);
		wait for 3 ns;				 		   	
	end process;
	
	----------------------------------------------------   
    -- ISP write emulation
    ----------------------------------------------------   
	process
	begin     
		wait until falling_edge(otg_o.wr_n);		
		wait until rising_edge(otg_o.wr_n);
		wait for 3 ns;
		isp_emu_data <= otg_data;	
	end process;
	
	
			  

end TB_ARCHITECTURE;

