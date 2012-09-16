-- usb_inc.vhd
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
--   USB 2.0 Descriptors, constants, and data structures
-- -----------------------------------------------------------------------
--  Version  : 1.0
--  Date     : Sept 2012
--  Author   : Mikhail Zakharov
--  Web      : http://ca.linkedin.com/in/mzakharo
--  Contact  : m.zakharo@gmail.com
-- -----------------------------------------------------------------------
--  FUNCTION :
--     USB Endpoint Descriptor configuration 
-- -----------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package usb_inc is 
--=-=-=-=-=-TYPE DECLARATIONS-=-=-=-=-=--
subtype nibble		is std_logic_vector(3 downto 0);
subtype byte		is std_logic_vector(7 downto 0);
subtype word		is std_logic_vector(15 downto 0);
subtype	dword		is std_logic_vector(31 downto 0);
subtype buffer64	is std_logic_vector(8*64-1 downto 0);

type deviceDescriptor is
record
	bLength			: byte;
	bDescriptorType	: byte;
	bcdUSB			: word;
	bDeviceClass	: byte;
	bDeviceSubClass	: byte;
	bDeviceProtocol	: byte;
	bMaxPacketSize0	: byte;
	idVendor		: word;
	idProduct		: word;
	bcdDevice		: word;
	iManufacturer	: byte;
	iProduct		: byte;
	iSerialNumber	: byte;
	bNumConfigs		: byte;
end record;
type configurationDescriptor is
record
	bLength			: byte;
	bDescriptorType	: byte;
	wTotalLength	: word;
	bNumInterfaces	: byte;
	bConfigValue	: byte;
	iConfiguration	: byte;
	bmAttributes	: byte;
	bMaxPower		: byte;
end record;
type interfaceDescriptor is
record
	bLength				: byte;
	bDescriptorType		: byte;
	bInterfaceNumber	: byte;
	bAlternateSetting	: byte;
	bNumEndpoints		: byte;
	bInterfaceClass		: byte;
	bInterfaceSubClass	: byte;
	bInterfaceProtocol	: byte;
	iInterface			: byte;
end record;
type endpointDescriptor is
record
	bLength			: byte;
	bDescriptorType	: byte;
	bEndpointAddress: byte;
	bmAttributes	: byte;
	wMaxPacketSize	: word;
	bInterval		: byte;
end record;
type device_request_t is 
  record
    bmRequestType	: byte;
    bRequest		: byte;
    wValue			: word;
    wIndex			: word;
    wLength			: word;
  end record;

--=-=-=-=-=-USB STANDARD REQUESTS-=-=-=-=-=--
constant GET_STATUS			: std_logic_vector(3 downto 0)	:= x"0";
constant CLEAR_FEATURE		: std_logic_vector(3 downto 0)	:= x"1";
constant SET_FEATURE		: std_logic_vector(3 downto 0)	:= x"3";
constant SET_ADDRESS		: std_logic_vector(3 downto 0)	:= x"5";
constant GET_DESCRIPTOR		: std_logic_vector(3 downto 0)	:= x"6";
constant SET_DESCRIPTOR		: std_logic_vector(3 downto 0)	:= x"7";
constant GET_CONFIGURATION	: std_logic_vector(3 downto 0)	:= x"8";
constant SET_CONFIGURATION	: std_logic_vector(3 downto 0)	:= x"9";
constant GET_INTERFACE		: std_logic_vector(3 downto 0)	:= x"A";
constant SET_INTERFACE		: std_logic_vector(3 downto 0)	:= x"B";
constant SYNCH_FRAME_E		: std_logic_vector(3 downto 0)	:= x"C";
constant CFG_DEVICE			: std_logic_vector(3 downto 0) 	:= x"0";
constant CFG_INTERFACE		: std_logic_vector(3 downto 0) 	:= x"1";
constant CFG_ENDPOINT		: std_logic_vector(3 downto 0) 	:= x"2";

--=-=-=-=-=-DESCRIPTOR TYPES-=-=-=-=-=--
constant desc_DEVICE			:  std_logic_vector(3 downto 0)	:= x"1";
constant desc_CONFIGURATION		:  std_logic_vector(3 downto 0)	:= x"2";
constant desc_STRING			:  std_logic_vector(3 downto 0)	:= x"3";
constant desc_INTERFACE			:  std_logic_vector(3 downto 0)	:= x"4";
constant desc_ENDPOINT			:  std_logic_vector(3 downto 0)	:= x"5";
constant desc_DEVICE_QUALIFIER	:  std_logic_vector(3 downto 0)	:= x"6";
constant desc_OTHER_SPEED_CFG	:  std_logic_vector(3 downto 0)	:= x"7";
constant desc_INTERFACE_POWER	:  std_logic_vector(3 downto 0)	:= x"8";				

--=-=-=-=-=-USB Config-=-=-=-=-=--
constant CRD_devDesc	: deviceDescriptor:=(bLength		=> x"12",
											bDescriptorType	=> x"0" & desc_DEVICE,
											bcdUSB			=> x"0200",
											bDeviceClass	=> x"00",
											bDeviceSubClass	=> x"00",
											bDeviceProtocol	=> x"00",
											bMaxPacketSize0	=> x"40",
											idVendor		=> x"04cc",
											idProduct		=> x"1a62",
											bcdDevice		=> x"0100",
											iManufacturer	=> x"00",
											iProduct		=> x"00",
											iSerialNumber	=> x"00",
											bNumConfigs		=>x"01");
											
constant CRD_cfgDesc	: configurationDescriptor:=(bLength			=>x"09",
													bDescriptorType	=>x"0" & desc_CONFIGURATION,
													wTotalLength	=>std_logic_vector(to_unsigned((9+9+7*2), 16)),
													bNumInterfaces	=>x"01",
													bConfigValue	=>x"01",
													iConfiguration	=>x"00",
													bmAttributes	=>x"C0",
													bMaxPower		=>x"32");

constant CRD_intDesc	: interfaceDescriptor:=(bLength				=>x"09",
												bDescriptorType		=>x"0" & desc_INTERFACE,
												bInterfaceNumber	=>x"00",
												bAlternateSetting	=>x"00",
												bNumEndpoints		=>x"02",
												bInterfaceClass		=>x"00",
												bInterfaceSubClass	=>x"00",
												bInterfaceProtocol	=>x"00",
												iInterface			=>x"00");


constant CRD_endp1Desc	: endpointDescriptor:=(	bLength			=>x"07",
												bDescriptorType	=>x"0" & desc_ENDPOINT,
												bEndpointAddress=>x"01",
												bmAttributes	=>x"02", --USB_ENDPOINT_TYPE_BULK
												wMaxPacketSize	=>x"0002",
												bInterval		=>x"00");
												
constant CRD_endp2Desc	: endpointDescriptor:=(	bLength			=>x"07",
												bDescriptorType	=>x"0" & desc_ENDPOINT,
												bEndpointAddress=>x"82",
												bmAttributes	=>x"02", -- USB_ENDPOINT_TYPE_BULK
												wMaxPacketSize	=>x"0002",
												bInterval		=>x"00");

--=-=-=-=-=-end USB Config-=-=-=-=-=--

function byte_deviceDescriptor( constant d: in deviceDescriptor ) return std_logic_vector;
function byte_configurationDescriptor( constant d: in configurationDescriptor ) return std_logic_vector;
function byte_interfaceDescriptor( constant d: in interfaceDescriptor ) return std_logic_vector;
function byte_endpointDescriptor( constant d: in endpointDescriptor ) return std_logic_vector;

constant CRD_Full_Desc 	: 	std_logic_vector(0 to 8*(9+9+7*2) - 1);

end package;


package body usb_inc is 
function byte_deviceDescriptor( constant d: in deviceDescriptor ) return std_logic_vector is
begin
	return(	d.bLength & 
			d.bDescriptorType & 
			d.bcdUSB(7 downto 0) & 
			d.bcdUSB(15 downto 8) & 
			d.bDeviceClass & 
			d.bDeviceSubClass & 
			d.bDeviceProtocol & 
			d.bMaxPacketSize0 & 
			d.idVendor(7 downto 0) & 
			d.idVendor(15 downto 8) & 
			d.idProduct(7 downto 0) & 
			d.idProduct(15 downto 8) & 
			d.bcdDevice(7 downto 0) & 
			d.bcdDevice(15 downto 8) & 
			d.iManufacturer & 
			d.iProduct & 
			d.iSerialNumber & 
			d.bNumConfigs);
end function byte_deviceDescriptor;

function byte_configurationDescriptor( constant d: in configurationDescriptor ) return std_logic_vector is
begin
	return(	d.bLength & 
			d.bDescriptorType & 
			d.wTotalLength(7 downto 0) & 
			d.wTotalLength(15 downto 8) & 
			d.bNumInterfaces & 
			d.bConfigValue & 
			d.iConfiguration & 
			d.bmAttributes & 
			d.bMaxPower);
end function byte_configurationDescriptor;

function byte_interfaceDescriptor( constant d: in interfaceDescriptor ) return std_logic_vector is
begin
	return(	d.bLength & 
			d.bDescriptorType & 
			d.bInterfaceNumber & 
			d.bAlternateSetting & 
			d.bNumEndpoints & 
			d.bInterfaceClass & 
			d.bInterfaceSubClass & 
			d.bInterfaceProtocol & 
			d.iInterface);
end function byte_interfaceDescriptor;

function byte_endpointDescriptor( constant d: in endpointDescriptor ) return std_logic_vector is
begin
	return(	d.bLength & 
			d.bDescriptorType & 
			d.bEndpointAddress & 
			d.bmAttributes & 
			d.wMaxPacketSize(7 downto 0) & 
			d.wMaxPacketSize(15 downto 8) & 
			d.bInterval);
end function byte_endpointDescriptor;


constant CRD_Full_Desc 	: 	std_logic_vector(0 to 8*(9+9+7*2) - 1):=(
							byte_configurationDescriptor(CRD_cfgDesc) & 
							byte_interfaceDescriptor(CRD_intDesc) & 
							byte_endpointDescriptor(CRD_endp1Desc) & 
							byte_endpointDescriptor(CRD_endp2Desc));	
end usb_inc;
