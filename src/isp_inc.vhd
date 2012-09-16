-- isp_inc.vhd
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
--   ISP1362 register definitions and bit field constants
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--     VHDL translation of constants, defined in ISP1362 Datasheet, Ch 15
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
package isp_inc is 

--=-=-=-=-=-COMMANDS-=-=-=-=-=--
constant Wr_DcEndpointConfiguration	: std_logic_vector(15 downto 4)	:= x"002";		--20h = control out
constant Rd_DcEndpointConfiguration	: std_logic_vector(15 downto 4)	:= x"003";		--30h = control out
constant Wr_DcAddress				: std_logic_vector(15 downto 0)	:= x"00B6";	--B6h
constant Rd_DcAddress				: std_logic_vector(15 downto 0)	:= x"00B7";	--B7h
constant Wr_DcMode					: std_logic_vector(15 downto 0)	:= x"00B8";	--B8h
constant Rd_DcMode					: std_logic_vector(15 downto 0)	:= x"00B9";	--B9h
constant Wr_DcHardwareConfiguration	: std_logic_vector(15 downto 0)	:= x"00BA";	--BAh
constant Rd_DcHardwareConfiguration	: std_logic_vector(15 downto 0)	:= x"00BB";	--BBh
constant Wr_DcInterruptEnable		: std_logic_vector(15 downto 0)	:= x"00C2";	--C2h
constant Rd_DcInterruptEnable		: std_logic_vector(15 downto 0)	:= x"00C3";	--C3h
constant Wr_DcDMAConfiguration		: std_logic_vector(15 downto 0)	:= x"00F0";	--F0h
constant Rd_DcDMAConfiguration		: std_logic_vector(15 downto 0)	:= x"00F1";	--F1h
constant Wr_DcDMACounter			: std_logic_vector(15 downto 0)	:= x"00F2";	--F2h
constant Rd_DcDMACounter			: std_logic_vector(15 downto 0)	:= x"00F3";	--F3h
constant Reset						: std_logic_vector(15 downto 0)	:= x"00F6";	--F6h
constant Wr_Buffer					: std_logic_vector(15 downto 4)	:= x"000";		--00h = control out (0 illegal)
constant Rd_Buffer					: std_logic_vector(15 downto 4)	:= x"001";		--10h = control out (1 illegal)
constant Rd_ESR						: std_logic_vector(15 downto 4)	:= x"005";		--50h = control out
constant EPSTS_STALL						: std_logic_vector(15 downto 4)	:= x"004";		--40h = control out
constant EPSTS_UNSTALL					: std_logic_vector(15 downto 4)	:= x"008";		--80h = control out
constant Validate					: std_logic_vector(15 downto 4)	:= x"006";		--60h = control out (0 illegal)
constant ClearBuffer				: std_logic_vector(15 downto 4)	:= x"007";		--70h = control out (1 illegal)
constant Rd_DcEndpointStatusImage	: std_logic_vector(15 downto 4)	:= x"00D";		--D0h = control out
constant AcknowledgeSetup			: std_logic_vector(15 downto 0)	:= x"00F4";	--F4h (must ack setups sec12.3.6)
constant Rd_ErrorCode				: std_logic_vector(15 downto 4)	:= x"00A";		--A0h = control out
constant UnlockDevice				: std_logic_vector(15 downto 0)	:= x"00B0";	--B0h
constant Wr_DcScratchRegister		: std_logic_vector(15 downto 0)	:= x"00B2";	--B2h
constant Rd_DcScratchRegister		: std_logic_vector(15 downto 0)	:= x"00B3";	--B3h
constant Rd_DcFrameNumber			: std_logic_vector(15 downto 0)	:= x"00B4";	--B4h
constant Rd_DcChipID				: std_logic_vector(15 downto 0)	:= x"00B5";	--B5h
constant Rd_DcInterrupt				: std_logic_vector(15 downto 0)	:= x"00C0";	--C0h


--From hal.h
constant EPINDEX4EP0_CONTROL_OUT : std_logic_vector(3 downto 0) := x"0";
constant EPINDEX4EP0_CONTROL_IN : std_logic_vector(3 downto 0) := x"1";
constant EPINDEX4EP01 : std_logic_vector(3 downto 0) := x"2";
constant EPINDEX4EP02 : std_logic_vector(3 downto 0) := x"3";
constant EPINDEX4EP03 : std_logic_vector(3 downto 0) := x"4";
constant EPINDEX4EP04 : std_logic_vector(3 downto 0) := x"5";
constant EPINDEX4EP05 : std_logic_vector(3 downto 0) := x"6";
constant EPINDEX4EP06 : std_logic_vector(3 downto 0) := x"7";
constant EPINDEX4EP07 : std_logic_vector(3 downto 0) := x"8";
constant EPINDEX4EP08 : std_logic_vector(3 downto 0) := x"9";
constant EPINDEX4EP09 : std_logic_vector(3 downto 0) := x"A";
constant EPINDEX4EP0A : std_logic_vector(3 downto 0) := x"B";
constant EPINDEX4EP0B : std_logic_vector(3 downto 0) := x"C";
constant EPINDEX4EP0C : std_logic_vector(3 downto 0) := x"D";
constant EPINDEX4EP0D : std_logic_vector(3 downto 0) := x"E";
constant EPINDEX4EP0E : std_logic_vector(3 downto 0) := x"F";


constant DEVADDR_MASK : std_logic_vector(15 downto 0) := x"007F";
constant DEVADDR_EN : std_logic_vector(15 downto 0) := x"0080";

constant EP_DISABLE : std_logic_vector(15 downto 0) := "--------0-------";
constant EPCNFG_FIFO_EN : std_logic_vector(15 downto 0) := x"0080";
constant EPCNFG_IN_EN : std_logic_vector(15 downto 0) := x"0040";
constant EPCNFG_DBLBUF_EN : std_logic_vector(15 downto 0) := x"0020";
constant EPCNFG_ISO_EN : std_logic_vector(15 downto 0) := x"0010";
constant EPCNFG_ISOSZ_MASK : std_logic_vector(15 downto 0) := x"000F";
constant EPCNFG_NONISOSZ_MASK : std_logic_vector(15 downto 0) := x"0007";
constant EPCNFG_RFB_EN : std_logic_vector(15 downto 0) := x"0008";
constant EPCNFG_NONISOSZ_8 : std_logic_vector(15 downto 0) := x"0000";
constant EPCNFG_NONISOSZ_16 : std_logic_vector(15 downto 0) := x"0001";
constant EPCNFG_NONISOSZ_32 : std_logic_vector(15 downto 0) := x"0002";
constant EPCNFG_NONISOSZ_64 : std_logic_vector(15 downto 0) := x"0003";
constant EPCNFG_ISOSZ_16 : std_logic_vector(15 downto 0) := x"0000";
constant EPCNFG_ISOSZ_32 : std_logic_vector(15 downto 0) := x"0001";
constant EPCNFG_ISOSZ_48 : std_logic_vector(15 downto 0) := x"0002";
constant EPCNFG_ISOSZ_64 : std_logic_vector(15 downto 0) := x"0003";
constant EPCNFG_ISOSZ_96 : std_logic_vector(15 downto 0) := x"0004";
constant EPCNFG_ISOSZ_128 : std_logic_vector(15 downto 0) := x"0005";
constant EPCNFG_ISOSZ_160 : std_logic_vector(15 downto 0) := x"0006";
constant EPCNFG_ISOSZ_192 : std_logic_vector(15 downto 0) := x"0007";
constant EPCNFG_ISOSZ_256 : std_logic_vector(15 downto 0) := x"0008";
constant EPCNFG_ISOSZ_320 : std_logic_vector(15 downto 0) := x"0009";
constant EPCNFG_ISOSZ_384 : std_logic_vector(15 downto 0) := x"000A";
constant EPCNFG_ISOSZ_512 : std_logic_vector(15 downto 0) := x"000B";
constant EPCNFG_ISOSZ_640 : std_logic_vector(15 downto 0) := x"000C";
constant EPCNFG_ISOSZ_768 : std_logic_vector(15 downto 0) := x"000D";
constant EPCNFG_ISOSZ_896 : std_logic_vector(15 downto 0) := x"000E";
constant EPCNFG_ISOSZ_1023 : std_logic_vector(15 downto 0) := x"000F";

--constant EPSTS_STALL : std_logic_vector(15 downto 0) := x"0080";
constant EPSTS_DBF1 : std_logic_vector(15 downto 0) := x"0040";
constant EPSTS_DBF0 : std_logic_vector(15 downto 0) := x"0020";
constant EPSTS_OVWR : std_logic_vector(15 downto 0) := x"0008";
constant EPSTS_SETUP : std_logic_vector(15 downto 0) := x"0004";
constant EPSTS_DBFIDX : std_logic_vector(15 downto 0) := x"0002";

constant ERRCODE_RXTX : std_logic_vector(15 downto 0) := x"0001";
constant ERRCODE_DPID : std_logic_vector(15 downto 0) := x"0040";
constant ERRCODE_OVRLP : std_logic_vector(15 downto 0) := x"0080";
constant ERRCODE_MASK : std_logic_vector(15 downto 0) := x"001E";
constant ERRCODE_OK : std_logic_vector(15 downto 0) := x"0000";
constant ERRCODE_PIDERR : std_logic_vector(15 downto 0) := x"0002";
constant ERRCODE_PIDNEW : std_logic_vector(15 downto 0) := x"0004";
constant ERRCODE_PKTTYP : std_logic_vector(15 downto 0) := x"0006";
constant ERRCODE_TKCRC : std_logic_vector(15 downto 0) := x"0008";
constant ERRCODE_DATACRC : std_logic_vector(15 downto 0) := x"000A";
constant ERRCODE_TIMEOUT : std_logic_vector(15 downto 0) := x"000C";
constant ERRCODE_RSRV0 : std_logic_vector(15 downto 0) := x"000E";
constant ERRCODE_EOP : std_logic_vector(15 downto 0) := x"0010";
constant ERRCODE_NAK : std_logic_vector(15 downto 0) := x"0012";
constant ERRCODE_STALL : std_logic_vector(15 downto 0) := x"0014";
constant ERRCODE_OVRFL : std_logic_vector(15 downto 0) := x"0016";
constant ERRCODE_RSRV1 : std_logic_vector(15 downto 0) := x"0018";
constant ERRCODE_BITSTUFF : std_logic_vector(15 downto 0) := x"001A";
constant ERRCODE_RSRV2 : std_logic_vector(15 downto 0) := x"001C";
constant ERRCODE_DATAPID : std_logic_vector(15 downto 0) := x"001E";

constant LOCK_IOEN : std_logic_vector(15 downto 0) := x"0001";
constant MODE_SOFTCONNECT : std_logic_vector(15 downto 0) := x"0001";
constant MODE_DBG : std_logic_vector(15 downto 0) := x"0004";
constant MODE_INT_EN : std_logic_vector(15 downto 0) := x"0008";
constant MODE_SUSPND : std_logic_vector(15 downto 0) := x"0020";

constant DEVCNFG_INTPOL : std_logic_vector(15 downto 0) := x"0001";
constant DEVCNFG_INTEDGE : std_logic_vector(15 downto 0) := x"0002";
constant DEVCNFG_PWROFF : std_logic_vector(15 downto 0) := x"0004";
constant DEVCNFG_WAKEUPBY_CS : std_logic_vector(15 downto 0) := x"0008";
constant DEVCNFG_EOTPOL : std_logic_vector(15 downto 0) := x"0010";
constant DEVCNFG_DMAACKPOL : std_logic_vector(15 downto 0) := x"0020";
constant DEVCNFG_DMARQPOL : std_logic_vector(15 downto 0) := x"0040";
constant DEVCNFG_DMAACKONLY : std_logic_vector(15 downto 0) := x"0080";
constant DEVCNFG_CLOCKDIV_MASK : std_logic_vector(15 downto 0) := x"0F00";
constant DEVCNFG_CLOCKRUNNING : std_logic_vector(15 downto 0) := x"1000";
constant DEVCNFG_NOLAZYCLOCK : std_logic_vector(15 downto 0) := x"2000";
constant DEVCNFG_EXPULLUP : std_logic_vector(15 downto 0) := x"4000";


constant INTSRC_BUSRESET : std_logic_vector(15 downto 0) := x"0001";
constant INTSRC_RESUME : std_logic_vector(15 downto 0) := x"0002";
constant INTSRC_SUSPEND : std_logic_vector(15 downto 0) := x"0004";
constant INTSRC_EOT : std_logic_vector(15 downto 0) := x"0008";
constant INTSRC_SOF : std_logic_vector(15 downto 0) := x"0010";
constant INTSRC_PSEUDO_SOF : std_logic_vector(15 downto 0) := x"0020";
constant INTSRC_SHORT_PACKET : std_logic_vector(15 downto 0) := x"0040";
constant INTSRC_EP0OUT : std_logic_vector(15 downto 0) := x"0100";
constant INTSRC_EP0IN : std_logic_vector(15 downto 0) := x"0200";
constant INTSRC_EP01 : std_logic_vector(15 downto 0) := x"0400";
constant INTSRC_EP02 : std_logic_vector(15 downto 0) := x"0800";
constant INTSRC_EP03 : std_logic_vector(15 downto 0) := x"1000";
constant INTSRC_EP04 : std_logic_vector(15 downto 0) := x"2000";
constant INTSRC_EP05 : std_logic_vector(15 downto 0) := x"4000";
constant INTSRC_EP06 : std_logic_vector(15 downto 0) := x"8000";



end package;