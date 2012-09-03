
#ifndef __COMMON_H__
#define __COMMON_H__

#include "BasicTyp.h"
#include "usb_irq.h"

   //*************************************************************************
   // macros
   //*************************************************************************

#define SWAP(x)   (x)
#define MSB(x)    (((x) >> 8) & 0xFF)
#define LSB(x)    ((x) & 0xFF)

#define RaiseIRQL() disable()
#define LowerIRQL() enable()
#define NULL_LENGTH 0xff

   //*************************************************************************
   // structure and union definitions
   //*************************************************************************


#define  DEV_BEHAVIOUR_NOT_SPECIFIED  0 

typedef union _USBCHECK_DEVICE_STATES
{
	struct _USBCHECK_FLAGS
	{
		unsigned char	DEVICE_DEFAULT_STATE	   : 1;
		unsigned char	DEVICE_ADDRESS_STATE 	   : 1;
		unsigned char	DEVICE_CONFIGURATION_STATE : 1;
		unsigned char	DEVICE_INTERFACE_STATE	   : 1;
		unsigned char	FOUR_RESERVED_BITS		   : 8;
		unsigned char	RESET_BITS		           : 8;
	}State_bits;
	
	unsigned char MY_WAY;
}USBCHECK_DEVICE_STATES;



typedef union _D13FLAGS
{
	struct _D13FSM_FLAGS
	{

// Byte 0
		IRQL_1 UCHAR    bus_reset           : 1;
    IRQL_1 UCHAR         suspend             : 1;
    IRQL_1 UCHAR        At_IRQL1		        : 1;
		IRQL_0 UCHAR    configuration		    : 1;
		IRQL_0 UCHAR    remote_wakeup		    : 1;
		IRQL_0 UCHAR    Reserved00          : 3;
// Byte 1
		IRQL_1 UCHAR    DCP_state			      : 4;
		IRQL_1 UCHAR    DCP_IOorMEM         : 1;
		IRQL_1 UCHAR    ISO_state	          : 3;
// Byte 2
		IRQL_1 UCHAR	  setup_dma				        : 1;
		IRQL_1 UCHAR	  control_write_processed	: 1;
		IRQL_1 UCHAR	  dma_disable				      : 1;
		IRQL_1 UCHAR	  dma_state				        : 2;
		IRQL_1 UCHAR      toggle_bit_corrected	  : 1;
		IRQL_1 UCHAR	  control_out_done		    : 1;
		IRQL_0 UCHAR      Reserved20            	: 1;
// Byte 3
		IRQL_1 UCHAR      verbose				      : 1;
		IRQL_1 UCHAR      timer               : 1;
		IRQL_1 UCHAR      dbg                 : 1;
		IRQL_1 UCHAR      Reserved30          : 5;
	} bits;

	ULONG value;

} D13FLAGS;

typedef struct _device_request
{
	UCHAR bmRequestType;
	UCHAR bRequest;
  
	unsigned short int  wValue;
  unsigned short int  wIndex;
	unsigned short int  wLength;
} DEVICE_REQUEST;

// ******************************************************************************

typedef struct _IO_REQUEST 
{
	USHORT	uAddressL;
	UCHAR	bAddressH;
	USHORT	uSize;
	UCHAR	bCommand;
} IO_REQUEST, *PIO_REQUEST;

//*******************************************************************************

#define MAX_CONTROLDATA_SIZE	16
typedef struct _CONTROL_XFER
{
	IRQL_1 BOOLEAN          Abort;      /*Indicate this transfer shall be aborted ASAP*/
	IRQL_1 DEVICE_REQUEST   DeviceRequest;
	IRQL_1 USHORT  		    wLength;
	IRQL_1 USHORT  		    wCount;
	IRQL_1 ADDRESS          Addr;
	IRQL_1 UCHAR            dataBuffer[MAX_CONTROLDATA_SIZE];

} CONTROL_XFER, * PCONTROL_XFER;


// FSM for Default Control Pipe
#define USBFSM4DCP_IDLE             0x0
#define USBFSM4DCP_DATAIN           0x1
#define USBFSM4DCP_DATAOUT          0x2
#define USBFSM4DCP_REQUESTPROC      0x3
#define USBFSM4DCP_HANDSHAKE        0x4
#define USBFSM4DCP_SETUPPROC        0x5
#define USBFSM4DCP_REQUESTPROC1     0x6
#define USBFSM4DCP_STALL            0x7
#define USBFSM4DCP_REQUESTPROC2     0x8
#define USBFSM4DCP_REQUESTPROC3     0x9
#define USBFSM4DCP_CONTROLOUTDONE   0x0A

#endif
