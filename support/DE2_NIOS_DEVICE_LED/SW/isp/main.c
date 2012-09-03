/*
 * main.c
 *
 *  Created on: 2012-07-05
 *      Author: mzakharo
 */
#include <stdio.h>
#include "hal.h"
#include "drv.h"
#include "isr.h"
#include "common.h"

//-------------------------------------------------------------------------
//  Global Variables
D13FLAGS bD13flags;
DEVICE_REQUEST DeviceRequest;

int st_dev = 0;
extern int debug;
int main()
{
	int sent = 0;
	UCHAR buf = 0x11;
	RaiseIRQL();

	Hal4D13_ResetDevice();
	usleep(100);
	check_chip_id();

	configure_isp();

	Hal4D13_AcquireD13(ISP1362_DC_IRQ, (void*) usb_isr);
	LowerIRQL();
	while (1)
	{
		if (bD13flags.DCP_state == USBFSM4DCP_SETUPPROC)
			SetupToken_Handler();
		if ((bD13flags.DCP_state == USBFSM4DCP_REQUESTPROC))
			DeviceRequest_Handler();

		if (st_dev && !sent)
		{
			sent = 1;
			debug = 1;
			Hal4D13_WriteEndpoint(EPINDEX4EP02,&buf, 1);
			debug = 0;
		}
	}
	return 0;
}
