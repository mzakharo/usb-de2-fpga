library work;
use work.isp_hal.all;
use work.devreq_inc.all;  
use work.usb_inc.all;
library ieee;
use ieee.NUMERIC_STD.all;
use ieee.std_logic_1164.all;
use std.textio.all;
	-- Add your library and packages declaration here ...

entity devreq_tb is
end devreq_tb;

architecture TB_ARCHITECTURE of devreq_tb is
	-- Stimulus signals - signals mapped to the input and inout ports of tested entity
	signal clk : STD_LOGIC;
	signal reset : STD_LOGIC;
	signal otg_data : STD_LOGIC_VECTOR(15 downto 0);
	
	signal hi : isp_hal_in_t;
	signal ho : isp_hal_out_t;

	signal di : devreq_in_t;
	signal do : devreq_out_t;

	--local signals
	signal slowclk_en : bit;	
	
	signal dr : device_request_t;	
	signal req_rdy : bit;	  
	
	signal isp_emu_data : std_logic_vector(15 downto 0);
		
	type IntegerFileType is file of integer;
	file data_out: text;		
	
  --------------------------------------------------------------
  -- clock cycle

  constant period  : time := 20 ns;

  --------------------------------------------------------------

begin





	
	
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
	hi.slowclk_en <= slowclk_en; 

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
		wait until falling_edge(ho.rd_n);
		wait for 22 ns;
		otg_data <= isp_emu_data;
		wait until rising_edge(ho.cs_n);
		wait for 3 ns;				 		   	
	end process;
	
	----------------------------------------------------   
    -- ISP write emulation
    ----------------------------------------------------   
	process	   
	variable buf: LINE;
	begin     
		wait until falling_edge(ho.wr_n);		
		wait until rising_edge(ho.wr_n);
		wait for 3 ns;
		isp_emu_data <= otg_data;
		
		if (ho.addr(0) = '1') then
			write(buf, string'("Cx"));
		else
			write(buf, string'("Dx"));
		end if;
		--write(buf,to_integer(unsigned(otg_data)));	
		write(buf, to_hstring(otg_data));
		--write(buf, string'("This is an example of formatted IO"));
		writeline(data_out, buf);				   
	end process;
	
	--hal	
	hi.drv <= do.hal;		
	h: hal generic map(3) port map (clk, reset, otg_data , hi, ho);

	--devreq 	
	di.hal <= ho.drv;
	di.req <= dr;
	di.req_rdy <= req_rdy;
	d: devreq port map(clk,reset, di , do);
									 
	process
	 	variable fstatus: FILE_OPEN_STATUS;
	begin 		 
		file_open(fstatus, data_out, "myfile.txt", write_mode);
		wait;
	end process;
	
	process	
	variable buf: LINE;	
	begin  														 
		wait until rising_edge(clk);
		dr.bmRequestType <= (others => '0');
		dr.bRequest <= (others =>'0');
		dr.wValue <= (others => '0');		
		dr.wIndex		<= std_logic_vector(to_unsigned(0, 16));
		
		
		dr.bRequest(3 downto 0) <= SET_ADDRESS;
		dr.wValue <= x"0001";
		

		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
				
		dr.bRequest(3 downto 0) <= GET_DESCRIPTOR;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_DEVICE; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(8, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
		dr.bRequest(3 downto 0) <= GET_DESCRIPTOR;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_DEVICE; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(18, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
		dr.bRequest(3 downto 0) <= GET_DESCRIPTOR;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_CONFIGURATION; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(9, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
		dr.bRequest(3 downto 0) <= GET_DESCRIPTOR;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_CONFIGURATION; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(32, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
		
		dr.bRequest(3 downto 0) <= GET_DESCRIPTOR;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_INTERFACE_POWER; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(32, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
	
		dr.bRequest(3 downto 0) <= SET_CONFIGURATION;--SET_CONFIGURATION;-- GET_DESCRIPTOR;
		dr.wValue(11 downto 8) <= desc_INTERFACE_POWER; --desc_CONFIGURATION;-- desc_DEVICE;								   
		dr.wLength		<= std_logic_vector(to_unsigned(0, 16)); 
		
		wait until rising_edge(clk);
		req_rdy <= '1';
		wait until rising_edge(clk);   
		req_rdy <= '0';
		wait until rising_edge(do.done);
		write(buf, string'("Done"));
		writeline(data_out, buf);
		
		
		wait;
	end process;
			  

end TB_ARCHITECTURE;

