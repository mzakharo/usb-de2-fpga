#include <stdio.h>
#include "common.h"
#include "hal.h"
#include "isr.h"
#include "drv.h"

// ************************************************************************
//  ISR Subroutine
// ************************************************************************
void usb_isr(void)
{
	ULONG i_st;
	RaiseIRQL();
	i_st = Hal4D13_ReadInterruptRegister();
	if (i_st != 0)
	{
		if (i_st & INTSRC_BUSRESET)
			Isr_BusReset();
		if (i_st & INTSRC_EP0OUT)
			Isr_Ep00RxDone();
		if (i_st & INTSRC_EP01)
			Isr_Ep01Done();
		if (i_st & INTSRC_EP02)
			Isr_Ep02Done();

	}
	LowerIRQL();
}
extern int st_dev;
void Isr_BusReset(void)
{
	st_dev = 0;
	printf("BUS RESET\n");
	configure_isp();
}

void Isr_Ep00RxDone(void)
{
	UCHAR ep_last;
	//printf("RX %d\n", bUSBCheck_Device_State.State_bits.FOUR_RESERVED_BITS++);
	ep_last = Hal4D13_GetEndpointStatusWInteruptClear(EPINDEX4EP0_CONTROL_OUT); /* Clear interrupt flag */

	if (ep_last & EPSTS_SETUP)
	{
		bD13flags.DCP_state = USBFSM4DCP_SETUPPROC;
	}
}

#define BUF_SIZE 4
USBCHECK_DEVICE_STATES bUSBCheck_Device_State;
void Isr_Ep01Done(void)
{
	unsigned char bbuf[1];
	UCHAR c;
	c = Hal4D13_GetEndpointStatusWInteruptClear(EPINDEX4EP01);
	Hal4D13_ReadEndpoint(EPINDEX4EP01, (UCHAR *) &bbuf, sizeof(bbuf));
	//printf("Received From Endpoint 1\n");

	/* led stuff */
	if (bbuf[0] == 0xaa)
		bbuf[0] = bUSBCheck_Device_State.State_bits.FOUR_RESERVED_BITS++;
	else if (bbuf[0] == 0x55)
		bUSBCheck_Device_State.State_bits.FOUR_RESERVED_BITS = 0;
	else if (bbuf[0] == 0x99)
		bUSBCheck_Device_State.State_bits.RESET_BITS = 1;
	else
		bUSBCheck_Device_State.State_bits.FOUR_RESERVED_BITS = 0xcc;
	bbuf[0] = bUSBCheck_Device_State.State_bits.FOUR_RESERVED_BITS;
	IOWR(LED_GREEN_BASE,0,bbuf[0]);
	IOWR(LED_RED_BASE,0,bbuf[0]);
	IOWR(SEG7_DISPLAY_BASE,0,bbuf[0]);
	/* led stuff */

	/*loopback */
	//Hal4D13_WriteEndpoint(EPINDEX4EP02, (UCHAR *) &bbuf, sizeof(bbuf));
}

void Isr_Ep02Done(void)
{
	//printf("Received From Endpoint 2\n");
	Hal4D13_GetEndpointStatusWInteruptClear(EPINDEX4EP02);
}
