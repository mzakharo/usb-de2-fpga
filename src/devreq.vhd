-- devreq.vhd
-- -----------------------------------------------------------------------
--   USB 2.0 Device Request handler
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--     1. Handles control endpoint requests sent by the USB Host
--	   2. Sends USB Descriptors, defined in usb_inc.vhd to the host
--	   4. Can be used to implement Vendor Specific Device Requests
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use work.usb_inc.all;
use work.isp_hal.all;
package devreq_inc is -- component declaration package

type devreq_in_t is record
	hal : drv_iface_out_t;
	req : device_request_t;
	req_rdy : bit;
end record;
type devreq_out_t is record
	hal : drv_iface_in_t;
	done : bit;
	configured : bit;
end record;

component devreq is
port(
	clk	: in	std_logic;
	reset : in std_logic;
	d : in devreq_in_t;
	q : out devreq_out_t);
end component;
end package;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.devreq_inc.all;
use work.usb_inc.all;
use work.isp_hal.all;
use work.isp_inc.all;

entity devreq is
port(

	clk	: in	std_logic;
	reset : in std_logic;
	d : in devreq_in_t;
	q : out devreq_out_t);
end devreq;
architecture handler of devreq is

type not_reset_t is record
	hal : drv_iface_in_t;
	tx_greater : signed(8 downto 0);
	tx_len : std_logic_vector(7 downto 0);
	descr_len : unsigned(7 downto 0);
	cnt  : unsigned(4 downto 0);
	inline : bit_vector(2 downto 0);	
	tmpcnt : unsigned(4 downto 0);
	neq : bit;
end record;

type state_t is (idle , decode, dev_descr, conf_descr, zero, stall, set_addr, wait_ready);


constant dev_len : integer := byte_deviceDescriptor(CRD_devDesc)'length; --bits
constant conf_len : integer := CRD_Full_Desc'length; --bits

--length of functions
constant fsend_ep_len : integer := 2; 
constant fdev_len : integer := dev_len / 2 / 8;  
constant fconf_len : integer := conf_len /2 /8;
constant fstall_len : integer := 2;
constant fvalid_len : integer := 1;
constant fset_addr_len : integer := 2;

--total number of functions
constant func_states : integer := fsend_ep_len + fdev_len + fconf_len + fstall_len + fvalid_len + fset_addr_len; 


type reg_t is record
	nr : not_reset_t;
	state : state_t;
	shift : bit_vector(func_states downto 0);	
	configured : bit;
end record;
signal r, rin : reg_t;

function swap( constant word : in std_logic_vector(0 to 15) ) return std_logic_vector is
begin		  
	return( word(8 to 15) & word(0 to 7));
end function swap;

begin --architecture

comb : process(r,d)
variable v : reg_t;

constant fsend_ep_start : integer := 0;
alias r_fsend_ep : bit_vector(fsend_ep_len -1 downto 0) is r.shift(fsend_ep_len -1 + fsend_ep_start downto fsend_ep_start);
alias v_fsend_ep : bit_vector(fsend_ep_len- 1 downto 0) is v.shift(fsend_ep_len -1 + fsend_ep_start downto fsend_ep_start);

constant fdev_start : integer := fsend_ep_len + fsend_ep_start;
alias r_fdev_descr : bit_vector(fdev_len-1 downto 0) is r.shift(fdev_len -1 + fdev_start downto fdev_start);
alias v_fdev_descr : bit_vector(fdev_len-1 downto 0) is v.shift(fdev_len -1 + fdev_start downto fdev_start);

constant fconf_start : integer := fdev_len + fdev_start;
alias r_fconf_descr : bit_vector(fconf_len-1 downto 0) is r.shift(fconf_len -1 + fconf_start downto fconf_start);
alias v_fconf_descr : bit_vector(fconf_len-1 downto 0) is v.shift(fconf_len -1 + fconf_start downto fconf_start);

constant fstall_start : integer := fconf_len + fconf_start;
alias r_fstall : bit_vector(fstall_len-1 downto 0) is r.shift(fstall_len -1 + fstall_start downto fstall_start);
alias v_fstall : bit_vector(fstall_len-1 downto 0) is v.shift(fstall_len -1 + fstall_start downto fstall_start);

constant fvalid_start : integer := fstall_len + fstall_start;

constant fset_addr_start : integer := fvalid_len + fvalid_start;
alias r_fset_addr : bit_vector(fset_addr_len-1 downto 0) is r.shift(fset_addr_len -1 + fset_addr_start downto fset_addr_start);
alias v_fset_addr : bit_vector(fset_addr_len-1 downto 0) is v.shift(fset_addr_len -1 + fset_addr_start downto fset_addr_start);

type func_data_t	is array(func_states - 1 downto 0) of data_t;
type func_cmd_t		is array(func_states - 1 downto 0) of cmd_t;

variable datas : func_data_t;
variable cmds  : func_cmd_t;


begin
	--parameters
	v := r; 
	
	case r.state is
		when idle => 
			if (d.req_rdy = '1') then
				v.state := decode ;	
			end if;			
			v.configured := '0';
		when decode =>
			--INFO: not checking bRequestType - assuming all requests are USB_STANDARD request types
			if (d.req.bRequest(3 downto 0) = GET_DESCRIPTOR and d.req.wValue(11 downto 8) = desc_DEVICE) then
				v.state := dev_descr;
			elsif (d.req.bRequest(3 downto 0) = GET_DESCRIPTOR and d.req.wValue(11 downto 8) = desc_CONFIGURATION) then
				v.state := conf_descr;		
			elsif (d.req.bRequest(3 downto 0) = SET_CONFIGURATION) then
				v.state := zero;
				v.configured := '1';
			elsif (d.req.bRequest(3 downto 0) = SET_ADDRESS) then
				v.state := set_addr;			
			else
				v.state := stall;
			end if;			
			v.nr.inline := (others => '0');			
			v.nr.descr_len :=to_unsigned(0, 8);	
		
		when dev_descr => v.state := wait_ready;	
			v_fsend_ep(0) := '1';		
			v.nr.descr_len :=to_unsigned((dev_len / 8), 8);	
			v.nr.inline(0) := '1';
			
		when conf_descr => v.state := wait_ready;		
			v.nr.descr_len :=to_unsigned((conf_len / 8), 8);	
			v_fsend_ep(0) := '1';
			v.nr.inline(1) := '1';
			
		when zero => v.state := wait_ready;			
			v_fsend_ep(0) := '1';
			v.nr.inline(2) := '1';
			
		when stall => v.state := wait_ready;
			v_fstall(0) := '1';
			
		when set_addr => v.state := wait_ready;
			v_fset_addr(0) := '1';			
			v.nr.inline(2) := '1';
			
		when wait_ready =>
			if  r.shift(func_states) = '1' then
				v.state := idle;				
			end if;
	end case;
	q.done <= r.shift(func_states);
	q.configured <= r.configured and  r.shift(func_states);
	
			
	--output length to send for desc_DEVICE
	v.nr.tx_greater := ('0' & signed(d.req.wLength(7 downto 0)) - ('0' & signed(r.nr.descr_len)) ); --INFO: max length supported : 0xFF 
	
	if (r.nr.tx_greater(r.nr.tx_greater'high) = '1') then --wLength < sizeof(desc_DEVICE)			
		v.nr.tx_len := d.req.wLength( 7 downto 1) & '0'; -- force even transactions - just to make the code simpler for now.
	else			
		v.nr.tx_len := std_logic_vector(r.nr.descr_len);
	end if;
	
	if (unsigned(to_stdlogicvector(r_fsend_ep))  /= 0) then
		v.nr.tmpcnt := unsigned(r.nr.tx_len(5 downto 1));  --divide by 2 
	else	
		v.nr.tmpcnt := r.nr.cnt;
	end if;
	
	if (r.nr.cnt /= 0) then
		v.nr.neq := '1';
	else
		v.nr.neq := '0';
	end if;	
	
	--function shift register
	if(d.hal.rdy = '1') then
		v_fsend_ep := (r_fsend_ep sll 1);
		v_fsend_ep(0) := v_fsend_ep(0) or r_fset_addr(r_fset_addr'high);
		
		--inline functions 
		v_fdev_descr(v_fdev_descr'low) 		:= r_fsend_ep(r_fsend_ep'high) 	and r.nr.inline(0);
		v_fconf_descr(v_fconf_descr'low) 	:= r_fsend_ep(r_fsend_ep'high) 	and r.nr.inline(1);	
		
		v.nr.cnt := r.nr.tmpcnt - 1; -- descriptor words left to send
		
		for i in v_fdev_descr'high downto 1 loop
			v_fdev_descr(i) := r_fdev_descr(i - 1) and r.nr.neq;
		end loop;
		
		for i in v_fconf_descr'high downto 1 loop
			v_fconf_descr(i) := r_fconf_descr(i - 1) and r.nr.neq;
		end loop;
		
		v_fstall := (r_fstall sll 1);
		
		v_fset_addr := (r_fset_addr sll 1);
		
		v.shift(fvalid_start) := ((r.nr.inline(0) or r.nr.inline(1)) and not(v.nr.neq))
							or 	(r_fsend_ep(r_fsend_ep'high) 	and r.nr.inline(2));
							
	end if;
	
	if(d.hal.rdy = '1') then
		v.shift(func_states) := r.shift(fvalid_start) or r_fstall(r_fstall'high);
	else
		v.shift(func_states) := '0';
	end if;
		
	--initialize temporary outputs
	for i in 0 to (func_states - 1) loop	
		cmds(i)  := (others => '0');
		datas(i) := (others => '0');
	end loop;
	
	--function code start
	
	if (r.shift(fsend_ep_start) = '1') then
		cmds(fsend_ep_start) := otg_wr_cmd;
		datas(fsend_ep_start) := Wr_Buffer & EPINDEX4EP0_CONTROL_IN;
	end if;
	if (r.shift(fsend_ep_start + 1) = '1') then
		cmds(fsend_ep_start + 1) := otg_wr;
		datas(fsend_ep_start+ 1) := x"00" & r.nr.tx_len;
	end if;
	
	for i in 0 to  r_fdev_descr'high  loop	
		if (r.shift(fdev_start + i) = '1' )then		
			cmds(fdev_start + i) := otg_wr;
			datas(fdev_start + i) := swap(byte_deviceDescriptor(CRD_devDesc)( i*16 to (i+1)*16 - 1)) ;
		 end if;			
	end loop;
	
	for i in 0 to  r_fconf_descr'high  loop	
		if (r.shift(fconf_start + i) = '1' )then		
			cmds(fconf_start + i) := otg_wr;
			datas(fconf_start + i) := swap(CRD_Full_Desc( i*16 to (i+1)*16 - 1));
		 end if;			
	end loop;
	
	if (r.shift(fstall_start) = '1') then
		cmds(fstall_start) := otg_wr_cmd;
		datas(fstall_start) := EPSTS_STALL & EPINDEX4EP0_CONTROL_OUT;
	end if;
	if (r.shift(fstall_start + 1) = '1') then
		cmds(fstall_start + 1) := otg_wr_cmd;
		datas(fstall_start+ 1) := EPSTS_STALL & EPINDEX4EP0_CONTROL_IN;
	end if;
	
	if (r.shift(fvalid_start) = '1') then
		cmds(fstall_start + 1) := otg_wr_cmd;
		datas(fstall_start+ 1) := Validate & EPINDEX4EP0_CONTROL_IN;
	end if;
	
	if (r.shift(fset_addr_start) = '1') then
		cmds(fset_addr_start) := otg_wr_cmd;
		datas(fset_addr_start) := Wr_DcAddress;
	end if;
	if (r.shift(fset_addr_start + 1) = '1') then
		cmds(fset_addr_start + 1) := otg_wr;
		datas(fset_addr_start + 1) := x"0080" or d.req.wValue;
	end if;

	--function code end
	
	--assign temporary outputs to real outputs
	v.nr.hal.data := (others => '0');
	v.nr.hal.cmd :=  (others => '0');	
	for i in 0 to (func_states - 1) loop	
		v.nr.hal.cmd  := v.nr.hal.cmd  or cmds(i);
		v.nr.hal.data := v.nr.hal.data or datas(i);
	end loop;
	
	--and finally
	q.hal.cmd <= r.nr.hal.cmd;
	q.hal.data <= r.nr.hal.data;
		
	rin <= v; --return (v);
end process;

seq : process(reset, clk)
begin
	if (reset= '1') then
		r.state <= idle;
		r.shift <= (others => '0');
		r.configured <= '0';
	elsif rising_edge(clk) then	
		r.shift <= rin.shift;
		r.state <= rin.state; 
		r.configured <= rin.configured;
	end if;
end process;
seq_nrst : process
begin
	wait until rising_edge(clk);	
	r.nr <= rin.nr; 
end process;

end architecture;
