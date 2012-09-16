-- drv.vhd
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
--   ISP1362 Driver
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--     1. Configures ISP1362 
--	   2. Handles IRQ from ISP1362
--	   3. I/O interface with external HW
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use work.isp_hal.all;
use work.devreq_inc.all;

package isp_drv is 

type io_t is record
	SData : std_logic_vector(15 downto 0);
	RDy : std_logic;
end record;


type isp_drv_in_t is record
	hal : drv_iface_out_t;
	int : std_logic;
	devreq : devreq_out_t;
	io : io_t;
end record;

type isp_drv_out_t is record
	hal : drv_iface_in_t;
	devreq : devreq_in_t;
	io : io_t;
end record;

component drv is
port(
	clk		: in std_logic;
	reset	: in std_logic;
	d : in isp_drv_in_t;
	q : out isp_drv_out_t	);
end component;
end package;

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use ieee.numeric_std.all;

use work.isp_hal.all;
use work.isp_drv.all;
use work.isp_inc.all;
use work.usb_inc.all;

entity drv is
port(
	clk		: in std_logic;
	reset	: in std_logic;
	d : in isp_drv_in_t;
	q : out isp_drv_out_t	);
end drv;

architecture handler of drv is
type cfg_t is (cfg, cfg1, cfg2, cfg3, cfg4, cfg5, cfg6, cfg7, cfg8, cfg9, 
				cfg10, cfg11, cfg12, cfg13, cfg14, cfg15, cfg16, cfg17, 
				cfg18, cfg19, cfg20, cfg21, cfg22, cfg23, cfg24, cfg25, cfg26 ,
				cfg27, cfg28, cfg29, cfg30, cfg31, cfg32, cfg33, cfg34, cfg_complete);
type irq_t is ( reset_irq, wait_irq, serve_irq, serve_irq1, serve_irq2,serve_irq3,
                ep0_out, ep0_out1, ep0_out2, bus_reset, ctrl, ctrl1,
                ctrl2, ctrl3, ctrl4, ctrl5, ctrl6, ctrl7, idev_req, ep1_out, ep1_out1,
				ep1_out2,ep1_out3,ep1_out4,ep1_out5, TxLoads, TxLoads1, TxLoads2, TxLoadDone);
type drv_t is (configure, handle_irq, gdev_req);

type dev_t is ( in_reset, configured);

type txfsm_t is (TxIdle, TxLoad);

type not_reset_t is record
	hdata_out : data_t;	
	hcmd : cmd_t;
	hcmd_cfg : cmd_t;
	hdata_out_cfg : data_t;
	hcmd_irq : cmd_t;
	hdata_out_irq : data_t;
	int : std_logic;
	DcInterrupt : data_t;
	DeviceReq	: device_request_t;
	req_rdy : bit;
	Din : std_logic_vector(io_t.SData'high downto 0);
end record;

type state_t is record
	cfg : cfg_t;
	global : drv_t;
	irq : irq_t;
	dev : dev_t;
	TxFSM : txfsm_t;
end record;

type reg_t is record
	st : state_t;
	nr : not_reset_t;
	RxRdy : std_logic;
end record;
signal r, rin : reg_t;

alias hdatah : byte is d.hal.data(15 downto 8);
alias hdatal : byte is d.hal.data(7 downto 0);

procedure 	hal_handler( variable v : inout reg_t ; constant hcmd : in cmd_t ; 
					constant data : in data_t; constant next_st : in cfg_t) is
begin
		v.nr.hdata_out_cfg :=data ;
		v.nr.hcmd_cfg := hcmd ;		
		if (d.hal.rdy = '1') then
			v.st.cfg := next_st;
		end if;	
end procedure 	hal_handler;

procedure hal_out_cmd( variable v : inout reg_t ; constant data : in data_t  ; 
					constant next_st : in cfg_t) is
begin
	hal_handler(v, otg_wr_cmd, data, next_st);
end procedure hal_out_cmd;

procedure hal_out( variable v :  inout reg_t ;  constant data : in data_t  ; 
					constant next_st : in cfg_t) is
begin
	hal_handler(v, otg_wr, data, next_st);
end procedure hal_out;

procedure hal_in( variable v :  inout reg_t ; constant next_st : in cfg_t) is
begin
	hal_handler(v,otg_rd, dont_care_data_t, next_st);
end procedure hal_in;
procedure 	hal_handler_irq( variable v : inout reg_t ; constant hcmd : in cmd_t ; 
					constant data : in data_t; constant next_st : in irq_t) is
begin
		v.nr.hdata_out_irq :=data ;	
		v.nr.hcmd_irq := hcmd ;		
		if (d.hal.rdy = '1') then
			v.st.irq := next_st;
		end if;		
end procedure 	hal_handler_irq;

procedure hal_out_cmd_irq( variable v : inout reg_t ; constant data : in data_t  ; 
					constant next_st : irq_t) is
begin
	hal_handler_irq(v, otg_wr_cmd, data, next_st);
end procedure hal_out_cmd_irq;

procedure hal_out_irq( variable v :  inout reg_t ;  constant data : in data_t  ; 
					constant next_st : in irq_t) is
begin
	hal_handler_irq(v, otg_wr, data, next_st);
end procedure hal_out_irq;

procedure hal_in_irq( variable v :  inout reg_t ; constant next_st : in irq_t) is
begin
	hal_handler_irq(v,otg_rd, dont_care_data_t, next_st);
end procedure hal_in_irq;


begin --architecture

comb : process(r,d)
variable v : reg_t;

begin
	v := r; --parameters		
		
	v.nr.hdata_out_cfg := dont_care_data_t;	
	v.nr.hcmd_cfg := otg_idle;
	
	case r.st.cfg is	
		when cfg => hal_out_cmd(v, Wr_DcMode, 												cfg1);
		when cfg1 => hal_out(v, MODE_INT_EN or MODE_SOFTCONNECT, 							cfg2);		
		when cfg2 =>  hal_out_cmd(v, Wr_DcHardwareConfiguration, 							cfg3);			
		when cfg3 => hal_out(v,DEVCNFG_NOLAZYCLOCK, 										cfg4);
		when cfg4 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0_CONTROL_OUT, 	cfg5);
		when cfg5 => hal_out(v, EPCNFG_FIFO_EN or EPCNFG_NONISOSZ_64, 						cfg6);
		when cfg6 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0_CONTROL_IN, 	cfg7);
		when cfg7 => hal_out(v, EPCNFG_FIFO_EN or EPCNFG_IN_EN or EPCNFG_NONISOSZ_64, 		cfg8);
		when cfg8 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP01,				cfg9);
		when cfg9 => hal_out(v, EPCNFG_FIFO_EN or EPCNFG_DBLBUF_EN or EPCNFG_NONISOSZ_64,	cfg10);
		when cfg10 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP02,				cfg11);
		when cfg11 => hal_out(v, EPCNFG_FIFO_EN or EPCNFG_DBLBUF_EN or EPCNFG_IN_EN or EPCNFG_NONISOSZ_64,	cfg12);
		when cfg12 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP03,				cfg13);
		when cfg13 => hal_out(v, EP_DISABLE, 												cfg14);
		when cfg14 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP04,				cfg15);
		when cfg15 => hal_out(v, EP_DISABLE, 												cfg16);
		when cfg16 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP05,				cfg17);
		when cfg17 => hal_out(v, EP_DISABLE, 												cfg18);
		when cfg18 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP06,				cfg19);                  
		when cfg19 => hal_out(v, EP_DISABLE, 												cfg20);                    
		when cfg20 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP07,				cfg21);                        
		when cfg21 => hal_out(v, EP_DISABLE, 												cfg22);                        
		when cfg22 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0A,				cfg23);                        
		when cfg23 => hal_out(v, EP_DISABLE, 												cfg24);                        
		when cfg24 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0B,				cfg25);                        
		when cfg25 => hal_out(v, EP_DISABLE, 												cfg26);                        
		when cfg26 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0C,				cfg27);                        
		when cfg27 => hal_out(v, EP_DISABLE, 												cfg28);                        
		when cfg28 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0D,				cfg29);                        
		when cfg29 => hal_out(v, EP_DISABLE, 												cfg30);                        
		when cfg30 => hal_out_cmd(v, Wr_DcEndpointConfiguration & EPINDEX4EP0E,				cfg31);
		when cfg31 => hal_out(v, EP_DISABLE,												cfg32); 
		when cfg32 => hal_out_cmd(v, Wr_DcInterruptEnable, 									cfg33);
		when cfg33 => hal_out(v, INTSRC_EP0OUT or INTSRC_EP01  or INTSRC_BUSRESET, cfg34);
		when cfg34 => hal_out(v, (others => '0'),  									 cfg_complete); --ignore upper bytes
		when cfg_complete => 
			if (r.st.irq = bus_reset) then
				v.st.cfg := cfg;
			end if;
		when others => v.st.cfg := r.st.cfg; --hang
	end case;	
		
	v.nr.hdata_out_irq := dont_care_data_t;	
	v.nr.hcmd_irq := otg_idle;

	
	v.RxRdy := '0';
	
	case r.st.irq is
		when reset_irq =>
			v.st.irq := wait_irq;
		when wait_irq =>
			if (r.st.TxFsm = TxLoad and r.st.dev = configured) then
				v.st.irq := TxLoads;				
			elsif (r.nr.int = '1') then
				v.st.irq := serve_irq;
			end if;
		when serve_irq => hal_out_cmd_irq(v, Rd_DcInterrupt , serve_irq1);
		when serve_irq1 =>  hal_in_irq (v,serve_irq2);
            v.nr.DcInterrupt := d.hal.data;
		when serve_irq2 => hal_in_irq (v,serve_irq3); 
        when serve_irq3 =>
        	if (r.nr.DcInterrupt(0) = '1') then		 --BUS RESET =1
                v.st.irq := bus_reset;
			elsif (r.nr.DcInterrupt(8) = '1') then	 --EP0_OUT =1 
				v.st.irq := ep0_out;
			elsif (r.nr.DcInterrupt(10) = '1') then	 --EP1_OUT =1 
				v.st.irq := ep1_out;
			else
				v.st.irq := wait_irq;
			end if;
        when bus_reset =>    
			v.st.irq := reset_irq;
		--ENDPOINT 0 Handler (no handshake)
        when ep0_out => hal_out_cmd_irq (v,Rd_ESR & EPINDEX4EP0_CONTROL_OUT, ep0_out1);
		when ep0_out1 => hal_in_irq(v, ep0_out2);
		when ep0_out2 => 
			if (d.hal.data(3 downto 2) = "01") then --OVERWRITE =0, SETUPT =1
				v.st.irq := ctrl;
			else
				v.st.irq := wait_irq;
			end if;
		--ENDPOINT 1 Handler (no handshake)	(RX)
		when ep1_out => hal_out_cmd_irq (v,Rd_ESR & EPINDEX4EP01, 		ep1_out1);
		when ep1_out1 => hal_in_irq(v, 									ep1_out2); --ignore status info
		when ep1_out2 => hal_out_cmd_irq(v,Rd_Buffer & EPINDEX4EP01, 	ep1_out3);
		when ep1_out3 => hal_in_irq(v, 									ep1_out4); --ignore length
		when ep1_out4 => hal_in_irq(v, 									ep1_out5);
			v.RxRdy := To_StdULogic(d.hal.rdy); 
		when ep1_out5 => hal_out_cmd_irq(v,ClearBuffer & EPINDEX4EP01, 	wait_irq);
		--ENDPOINT 2 Handler (no handshake) (TX)
		when TxLoads => hal_out_cmd_irq (v, Wr_Buffer & EPINDEX4EP02, TxLoads1);
		when TxLoads1 => hal_out_irq(v, x"0002", TxLoads2); -- Tx 2 Bytes
		when TxLoads2 => hal_out_irq(v, r.nr.Din, TxLoadDone);
		when txLoadDone => hal_out_cmd_irq(v,Validate & EPINDEX4EP02, wait_irq);
		--CONTROL Packet Handler
		when ctrl =>  hal_out_cmd_irq(v,Rd_Buffer & EPINDEX4EP0_CONTROL_OUT, 	ctrl1);
		when ctrl1 => hal_in_irq(v, 											ctrl2); --ignore length
		when ctrl2 => hal_in_irq(v, 											ctrl3);
			v.nr.DeviceReq.bmRequestType := hdatal;
			v.nr.DeviceReq.bRequest := hdatah;
		when ctrl3 => hal_in_irq(v, 											ctrl4);
			v.nr.DeviceReq.wValue := d.hal.data;
		when ctrl4 => hal_in_irq(v, 											ctrl5);
			v.nr.DeviceReq.wIndex := d.hal.data;
		when ctrl5 => hal_in_irq(v, 											ctrl6);
			v.nr.DeviceReq.wLength:= d.hal.data;
		when ctrl6 => hal_out_cmd_irq(v, AcknowledgeSetup, 						ctrl7);
		when ctrl7 => hal_out_cmd_irq(v, ClearBuffer & EPINDEX4EP0_CONTROL_OUT, idev_req);
		when idev_req => 
			if (d.devreq.done = '1') then				
				v.st.irq := wait_irq;
			end if;		
        when others =>	v.st.irq := r.st.irq; --hang	
	end case;	
	--outputs
	q.io.RDy <= r.RxRdy;
	q.io.SData <=  d.hal.data;
	
		
	q.devreq.req <= r.nr.DeviceReq;
	q.devreq.req_rdy <= r.nr.req_rdy;
	q.devreq.hal <= d.hal;
	
	v.nr.hcmd := otg_idle;
	v.nr.hdata_out := dont_care_data_t;
	
	v.nr.req_rdy := '0';	
	v.nr.int := '0';
	case r.st.global is 
	when configure =>
		v.nr.hcmd := r.nr.hcmd_cfg;
		v.nr.hdata_out := r.nr.hdata_out_cfg; 
		if (r.st.cfg = cfg_complete) then
			v.st.global := handle_irq;
		end if;
	when handle_irq =>
		v.nr.int := not(d.int);
		v.nr.hcmd := r.nr.hcmd_irq;
		v.nr.hdata_out := r.nr.hdata_out_irq; 		
        if (r.st.irq = bus_reset) then
            v.st.global := configure;
		elsif (r.st.irq = idev_req) then
			v.st.global := gdev_req;			
			v.nr.req_rdy := '1';
        end if;
	when gdev_req =>
		v.nr.hcmd := d.devreq.hal.cmd;
		v.nr.hdata_out := d.devreq.hal.data; 	
		if (d.devreq.done = '1') then
			v.st.global := handle_irq;
		end if;
	end case;
	
	q.hal.cmd <= r.nr.hcmd;
	q.hal.data <= r.nr.hdata_out;
	
	
	case r.st.dev is
	when in_reset =>
		if (d.devreq.configured = '1') then
			v.st.dev := configured;
		end if;
	when configured =>		
		if (r.st.cfg = cfg) then
			v.st.dev := in_reset;
		end if;	
	end case;	
	
	case r.st.TxFSM is
	when TxIdle =>
		if d.io.RDy ='1' then
			v.nr.Din := d.io.SData;    -- Latch input data immediately.
			v.st.TxFSM := TxLoad;		
		end if;
	when TxLoad =>
		if (r.st.irq = TxLoadDone) then
			v.st.TxFSM := TxIdle;
		end if;		
	end case;	
	
	rin <= v; --return (v);
end process;

seq : process(reset, clk)
begin
	if (reset= '1') then
		r.st.global <= configure;
		r.st.cfg <= cfg;
		r.st.irq <= reset_irq;
		r.st.dev <= in_reset;
		r.st.TxFSM <= TxIdle;
		r.RxRdy <= '0';
	elsif rising_edge(clk) then	
		r.st <= rin.st;
		r.RxRdy <= rin.RxRdy;
	end if;
end process;
seq_nrst : process
begin
	wait until rising_edge(clk);	
	r.nr <= rin.nr; 
end process;
end architecture;


