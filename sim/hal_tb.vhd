
library ieee;
use ieee.NUMERIC_STD.all;
use ieee.std_logic_1164.all;
use work.isp_hal.all;
	-- Add your library and packages declaration here ...

entity hal_tb is
end hal_tb;

architecture TB_ARCHITECTURE of hal_tb is
	-- Stimulus signals - signals mapped to the input and inout ports of tested entity
	signal clk : STD_LOGIC;
	signal reset : STD_LOGIC;
	signal d : isp_hal_in_t;
	signal otg_data : STD_LOGIC_VECTOR(15 downto 0);
	-- Observed signals - signals mapped to the output ports of tested entity
	signal q : isp_hal_out_t;

	--local signals
	signal slowclk_en : bit := '1';	
	signal write_sense :std_logic; 
	signal write_end_sense : std_logic;
  --------------------------------------------------------------
  -- clock cycle

  constant period  : time := 20 ns;

  --------------------------------------------------------------

begin

	-- Unit Under Test port map
	UUT : hal 
		generic map(3)  
		port map (
			clk => clk,
			reset => reset,
			otg_data => otg_data,
			d => d,
			q => q
		);
	
	
	--d.int <= '1'; -- not used in this tb
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
	d.slowclk_en <= slowclk_en; 

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
    -- read emulation
    ----------------------------------------------------   
	process
	begin 
		otg_data <= (others => 'Z');    
		wait until falling_edge(q.rd_n);
		wait for 22 ns;
		otg_data <= x"BEAD";
		wait until rising_edge(q.cs_n);
		wait for 3 ns;				 		   	
	end process;
	----------------------------------------------------   
    -- write emulation
    ----------------------------------------------------   	 
	write_sense <= to_stdulogic(d.drv.cmd(1)); 
	write_end_sense <= to_stdulogic(q.drv.rdy);
	process	
	begin
		wait for period;
		if (write_sense = '1') then	  
			d.drv.data <= x"FACE";
		else
			d.drv.data <= x"0000"; 
		end if;													 		
		wait until rising_edge(write_end_sense);
		wait until rising_edge(clk);
		d.drv.data <= x"0000";
		wait for period * 4;
	end process;
										   
	--choose what to do
	d.drv.cmd <= otg_rd;  -- otg_wr; -- otg_wr_cmd;
		  

end TB_ARCHITECTURE;

configuration TESTBENCH_FOR_hal of hal_tb is
	for TB_ARCHITECTURE
		for UUT : hal
			use entity work.hal(handler);
		end for;
	end for;
end TESTBENCH_FOR_hal;

