#include "common.h"
#include "drv.h"
#include "usb.h"
#include "hal.h"
#include "chap_9.h"
#include <stdio.h>

#define reserved Chap9_StallEP0
void ML_AcknowledgeSETUP(void);
//*************************************************************************
// USB protocol function pointer arrays
//*************************************************************************

#define MAX_STANDARD_REQUEST    0x0D
void (*StandardDeviceRequest[])(void) =
{
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		Chap9_SetAddress,
		Chap9_GetDescriptor,
		reserved,
		reserved,
		Chap9_SetConfiguration,
		reserved,
		reserved,
		reserved };

CHAR * _NAME_USB_REQUEST_TYPE[] =
{
		"Standard",
		"Class",
		"Vendor",
		"Reserved" };

CHAR * _NAME_USB_STANDARD_REQUEST[] =
{
		"GET_STATUS",
		"CLEAR_FEATURE",
		"RESERVED",
		"SET_FEATURE",
		"RESERVED",
		"SET_ADDRESS",
		"GET_DESCRIPTOR",
		"SET_DESCRIPTOR",
		"GET_CONFIGURATION",
		"SET_CONFIGURATION",
		"GET_INTERFACE",
		"SET_INTERFACE",
		"SYNC_FRAME" };

//*************************************************************************
// Class device requests
//*************************************************************************

#define MAX_CLASS_REQUEST    0x01
void (*ClassDeviceRequest[])(void) =
{
		reserved };

CHAR * _NAME_USB_CLASS_REQUEST[] =
{
		" ML_Reserved" };

//*************************************************************************
// Vendor Device Request
//*************************************************************************

#define MAX_VENDOR_REQUEST    0x0f

void benchmark(void)
{
	unsigned char c = 42;
	Chap9_SingleTransmitEP0(&c, 1);
	printf("benchmark %d\n", DeviceRequest.wValue);
}

void (*VendorDeviceRequest[])(void) =
{
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		reserved,
		benchmark,
		reserved };

CHAR * _NAME_USB_VENDOR_REQUEST[] =
{
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"RESERVED",
		"BENCHMARK",
		"RESERVED", };


void ML_AcknowledgeSETUP(void)
{
	Hal4D13_AcknowledgeSETUP();
	Hal4D13_ClearBuffer(EPINDEX4EP0_CONTROL_OUT);
}
int debug = 0;
void DeviceRequest_Handler(void)
{
	UCHAR type, req;
	RaiseIRQL();
	bD13flags.DCP_state = USBFSM4DCP_IDLE;
	debug = 1;

	type = DeviceRequest.bmRequestType & USB_REQUEST_TYPE_MASK;
	req = DeviceRequest.bRequest & USB_REQUEST_MASK;

	printf("req type = %s", _NAME_USB_REQUEST_TYPE[type >> 5]);

	if ((type == USB_STANDARD_REQUEST) && (req < MAX_STANDARD_REQUEST))
	{
		printf(" req: %s\n", _NAME_USB_STANDARD_REQUEST[req]);
		(*StandardDeviceRequest[req])();
	}
	else if ((type == USB_CLASS_REQUEST) && (req < MAX_CLASS_REQUEST))
	{
		printf("Not implemented req: 0x%X\n", req);
		(*ClassDeviceRequest[req])();
	}
	else if ((type == USB_VENDOR_REQUEST) && (req < MAX_VENDOR_REQUEST))
	{
		printf(" req: %d\n", req);
		(*VendorDeviceRequest[req])();
	}
	else
	{
		putchar('\n');
		Chap9_StallEP0();
	}
	debug = 0;
	LowerIRQL();
}
void SetupToken_Handler(void)
{
	unsigned short j;
	RaiseIRQL();
	bD13flags.DCP_state = USBFSM4DCP_IDLE;

	j = Hal4D13_ReadEndpointWOClearBuffer(EPINDEX4EP0_CONTROL_OUT, (UCHAR *) &DeviceRequest, sizeof(DeviceRequest));

	if (j == sizeof(DeviceRequest))
	{
		/* Set command  without Data stage*/
		ML_AcknowledgeSETUP();
		bD13flags.DCP_state = USBFSM4DCP_REQUESTPROC;

	}
	else
	{
		printf("wrong setup command\n");
		Chap9_StallEP0();
	}
	LowerIRQL();
}
void configure_isp(void)
{
	printf("configure %s\n", device_name);
	bD13flags.DCP_state = USBFSM4DCP_IDLE;

	//Configure device
	Hal4D13_SetDevConfig(DEVCNFG_NOLAZYCLOCK);
	Hal4D13_SetMode(MODE_INT_EN | MODE_SOFTCONNECT);

	/*Control Endpoint*/
	Hal4D13_SetEndpointConfig(EPCNFG_FIFO_EN | EPCNFG_NONISOSZ_64, EPINDEX4EP0_CONTROL_OUT);
	Hal4D13_SetEndpointConfig(EPCNFG_FIFO_EN | EPCNFG_IN_EN | EPCNFG_NONISOSZ_64, EPINDEX4EP0_CONTROL_IN);
	/*Control Endpoint*/

	/*port1 is OUT*/
	Hal4D13_SetEndpointConfig(EPCNFG_FIFO_EN | EPCNFG_DBLBUF_EN | EPCNFG_NONISOSZ_64, EPINDEX4EP01);
	/*port2 is IN*/
	Hal4D13_SetEndpointConfig(EPCNFG_FIFO_EN | EPCNFG_DBLBUF_EN | EPCNFG_IN_EN | EPCNFG_NONISOSZ_64, EPINDEX4EP02);

	/*DISABLED*/
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP03);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP04);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP05);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP06);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP07);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP08);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP09);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP0A);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP0B);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP0C);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP0D);
	Hal4D13_SetEndpointConfig(0, EPINDEX4EP0E);
	/*DISABLED*/

	/*Set interrupt configuration*/
	Hal4D13_SetIntEnable(INTSRC_EP0OUT | /*INTSRC_EP0IN |*/INTSRC_EP01 |/* INTSRC_EP02 | */INTSRC_BUSRESET);

}

void check_chip_id(void)
{
	unsigned short CHIP_ID;
	unsigned char HIGH_ID;

	CHIP_ID = Hal4D13_ReadChipID();
	HIGH_ID = (unsigned char) (CHIP_ID >> 8);

	switch (HIGH_ID)
	{
	case 0x36:
	{
		printf("CHIP ID =0x%04x\n\n", CHIP_ID);
		break;
	}
	default:
	{
		printf("UNKNOWN CHIP ID =0x%04x\n\n", CHIP_ID);
		break;
	}
	}
}
