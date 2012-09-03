#ifndef __COMMON_H__
#define __COMMON_H__

#include "basictyp.h"
#include "basic_io.h"
#include "usb_irq.h"

//*************************************************************************
// macros
//*************************************************************************

#define MSB(x)    (((x) >> 8) & 0xFF)

#define RaiseIRQL() disable()
#define LowerIRQL() enable()

//*************************************************************************
// structure and union definitions
//*************************************************************************


typedef union _USBCHECK_DEVICE_STATES
{
	struct _USBCHECK_FLAGS
	{
		unsigned char DEVICE_ADDRESS_STATE :1;
		unsigned char DEVICE_CONFIGURATION_STATE :1;
		unsigned char DEVICE_INTERFACE_STATE :1;
		unsigned char FOUR_RESERVED_BITS :8;
		unsigned char RESET_BITS :8;
	} State_bits;

	unsigned char MY_WAY;
} USBCHECK_DEVICE_STATES;

typedef struct _D13FLAGS
{
	volatile UCHAR DCP_state;
} D13FLAGS;

typedef struct _device_request
{
	volatile UCHAR bmRequestType;
	volatile UCHAR bRequest;

	volatile unsigned short int wValue;
	volatile unsigned short int wIndex;
	volatile unsigned short int wLength;
} DEVICE_REQUEST;

//*******************************************************************************
extern D13FLAGS bD13flags;
extern DEVICE_REQUEST DeviceRequest;


// FSM for Default Control Pipe
#define USBFSM4DCP_IDLE             0x0
#define USBFSM4DCP_REQUESTPROC      0x3
#define USBFSM4DCP_SETUPPROC        0x5

#endif
