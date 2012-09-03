#include <stdio.h>
#include <string.h>
#include "common.h"
#include "usb.h"
#include "chap_9.h"
#include "hal.h"
// *************************************************************************
// USB Device Descriptor
// *************************************************************************

USB_DEVICE_DESCRIPTOR DeviceDescr = //<<
		{
				sizeof(USB_DEVICE_DESCRIPTOR),
				USB_DEVICE_DESCRIPTOR_TYPE,
				0x200,//SWAP(0x200), //Complient to USB2.0 FullSpeed
				0x00,//bDeviceClass,
				0x00,//bDeviceSubClass,
				0x00,//bDeviceProtocol,
				64,//EP0_PACKET_SIZE,
				0x04cc,//SWAP(0x4cc), // Vendor ID for PHILIPS
				0x1a62,//SWAP(0x1a62),// Product ID for ISP 1362
				0x100,//SWAP(0x0100),// Device Release Number 1.0
				0,//STR_INDEX_MANUFACTURER,
				0,//STR_INDEX_PRODUCT,
				0,//STR_INDEX_SERIALNUMBER,
				0x1 //numConfigs

		};

USB_CONFIGURATION_DESCRIPTOR ConfigDescr =
{
		.bLength = 9,
		.bDescriptorType = USB_CONFIGURATION_DESCRIPTOR_TYPE,
		.wTotalLength = sizeof(USB_CONFIGURATION_DESCRIPTOR),
		.bNumInterfaces = 1,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = 0xC0,// USB_CONFIG_SELF_POWERED,
		.MaxPower = 50,
		.iface =
		{
				.bLength = sizeof(USB_INTERFACE_DESCRIPTOR),
				.bDescriptorType = USB_INTERFACE_DESCRIPTOR_TYPE,
				.bInterfaceNumber = 0,
				.bAlternateSetting = 0,
				.bNumEndpoints = 2,
				.bInterfaceClass = 0,
				.bInterfaceSubClass = 0,
				.bInterfaceProtocol = 0,
				.iInterface = 0 },
		.ep1 =
		{
				.bLength = sizeof(USB_ENDPOINT_DESCRIPTOR),
				.bDescriptorType = USB_ENDPOINT_DESCRIPTOR_TYPE,
				.bEndpointAddress = 0x1,//EP1, OUT
				.bmAttributes = USB_ENDPOINT_TYPE_BULK,
				.wMaxPacketSize = 1,
				.bInterval = 0, },
		.ep2 =
		{
				.bLength = sizeof(USB_ENDPOINT_DESCRIPTOR),
				.bDescriptorType = USB_ENDPOINT_DESCRIPTOR_TYPE,
				.bEndpointAddress = 0x2 | USB_ENDPOINT_IN_MASK, // EP2, IN
				.bmAttributes = USB_ENDPOINT_TYPE_BULK,
				.wMaxPacketSize = 1,
				.bInterval = 0, }, };

// *************************************************************************
// USB Protocol Layer
// ************************************************************************

void Chap9_SetAddress(void)
{
	Hal4D13_SetAddressEnable((UCHAR) (DeviceRequest.wValue & DEVICE_ADDRESS_MASK), 1);
	Chap9_SingleTransmitEP0(0, 0);
	printf("Addr %x\n", Hal4D13_GetAddress());
}

void Chap9_GetDescriptor(void)
{
	UCHAR bDescriptor = MSB(DeviceRequest.wValue);
	//UCHAR bDescriptorIndex = LSB(DeviceRequest.wValue);
	switch (bDescriptor)
	{

	case USB_DEVICE_DESCRIPTOR_TYPE:
	{
		printf("USB_DEVICE_DESCR\n");

		Chap9_BurstTransmitEP0((PUCHAR) &DeviceDescr, sizeof(USB_DEVICE_DESCRIPTOR));
		break;
	}
	case USB_CONFIGURATION_DESCRIPTOR_TYPE:
	{
		printf("USB_CONFIGURATION_DESCR\n");
		Chap9_BurstTransmitEP0((PUCHAR) &ConfigDescr, sizeof(USB_CONFIGURATION_DESCRIPTOR));
		break;
	}

	default:
		printf("UNKNOWN DESCR %d\n", bDescriptor);
		Chap9_StallEP0();
		break;
	}
}
extern int st_dev;
void Chap9_SetConfiguration(void)
{
	Chap9_SingleTransmitEP0(0, 0);
	st_dev = 1;
}

// *************************************************************************
// Chap9 support functions
// *************************************************************************

void Chap9_StallEP0(void)
{
	printf("stall\n");
	Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_OUT, EPSTS_STALL);
	Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_IN, EPSTS_STALL);

}

void Chap9_SingleTransmitEP0(PUCHAR buf, USHORT len)
{
	Hal4D13_SingleTransmitEP0(buf, len);
}

void Chap9_BurstTransmitEP0(PUCHAR pData, USHORT len)
{
	short int old = DeviceRequest.wLength;
	if (DeviceRequest.wLength > len)
		DeviceRequest.wLength = len;

	DeviceRequest.wLength &= 0xFE; //force even length
	printf("data %d, old %d, new %d\n", len,old, DeviceRequest.wLength );

	Hal4D13_WriteEndpoint(EPINDEX4EP0_CONTROL_IN, pData, DeviceRequest.wLength);
}

