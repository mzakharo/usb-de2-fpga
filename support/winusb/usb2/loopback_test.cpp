#include "stdafx.h"

#include "libusb.h"
#include <stdio.h>
#include <stdint.h>

#include <Windows.h>
#include <process.h>         // needed for _beginthread()

#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Device vendor and product id.
#define MY_VID 0x04CC
#define MY_PID 0x1A62

// Device configuration and interface id.
#define MY_CONFIG 1
#define MY_INTF 0

// Device endpoint(s)
#define EP_IN 0x82
#define EP_OUT 0x01


//////////////////////////////////////////////////////////////////////////////

#define BUF_SIZE (256)

unsigned char tbuf[BUF_SIZE];
volatile char usb_thread_done = 0;
unsigned char rbuf[BUF_SIZE];



void  usb_thread( void *arg )
{
    
    libusb_device_handle * handle = (libusb_device_handle*)arg;
 
    memset(rbuf, 0, sizeof(rbuf));
    // At some point, get the Tick Count
    DWORD tickAtStart = GetTickCount64();

    int actual_length;
    //wait for all the data to loopback to the device
    int ret = libusb_bulk_transfer(handle, EP_IN, rbuf, sizeof(rbuf), &actual_length, 5000);

    // At some other point get the change of Tick Count
    double elapsedTicks = (double)(GetTickCount64() - tickAtStart);
    if (ret < 0)
    {
        printf("error reading:\n");
    }
    else
        printf("success: bulk read %d bytes, TX+RX Bandwidth = %.2f KB/s\n", actual_length, actual_length * 2 / 1024 / (elapsedTicks / 1000.0));
    
	 usb_thread_done = 1;
}



static void print_endpoint_comp(const struct libusb_ss_endpoint_companion_descriptor* ep_comp)
{
    printf("      USB 3.0 Endpoint Companion:\n");
    printf("        bMaxBurst:           %u\n", ep_comp->bMaxBurst);
    printf("        bmAttributes:        %02xh\n", ep_comp->bmAttributes);
    printf("        wBytesPerInterval:   %u\n", ep_comp->wBytesPerInterval);
}

static void print_endpoint(const struct libusb_endpoint_descriptor* endpoint)
{
    int i, ret;

    printf("      Endpoint:\n");
    printf("        bEndpointAddress:    %02xh\n", endpoint->bEndpointAddress);
    printf("        bmAttributes:        %02xh\n", endpoint->bmAttributes);
    printf("        wMaxPacketSize:      %u\n", endpoint->wMaxPacketSize);
    printf("        bInterval:           %u\n", endpoint->bInterval);
    printf("        bRefresh:            %u\n", endpoint->bRefresh);
    printf("        bSynchAddress:       %u\n", endpoint->bSynchAddress);

    for (i = 0; i < endpoint->extra_length;) {
        if (LIBUSB_DT_SS_ENDPOINT_COMPANION == endpoint->extra[i + 1]) {
            struct libusb_ss_endpoint_companion_descriptor* ep_comp;

            ret = libusb_get_ss_endpoint_companion_descriptor(NULL, endpoint, &ep_comp);
            if (LIBUSB_SUCCESS != ret)
                continue;

            print_endpoint_comp(ep_comp);

            libusb_free_ss_endpoint_companion_descriptor(ep_comp);
        }

        i += endpoint->extra[i];
    }
}

static void print_altsetting(const struct libusb_interface_descriptor* interface)
{
    uint8_t i;

    printf("    Interface:\n");
    printf("      bInterfaceNumber:      %u\n", interface->bInterfaceNumber);
    printf("      bAlternateSetting:     %u\n", interface->bAlternateSetting);
    printf("      bNumEndpoints:         %u\n", interface->bNumEndpoints);
    printf("      bInterfaceClass:       %u\n", interface->bInterfaceClass);
    printf("      bInterfaceSubClass:    %u\n", interface->bInterfaceSubClass);
    printf("      bInterfaceProtocol:    %u\n", interface->bInterfaceProtocol);
    printf("      iInterface:            %u\n", interface->iInterface);

    for (i = 0; i < interface->bNumEndpoints; i++)
        print_endpoint(&interface->endpoint[i]);
}
static void print_interface(const struct libusb_interface* interface)
{
    int i;

    for (i = 0; i < interface->num_altsetting; i++)
        print_altsetting(&interface->altsetting[i]);
}


static void print_configuration(struct libusb_config_descriptor* config)
{
    uint8_t i;

    printf("  Configuration:\n");
    printf("    wTotalLength:            %u\n", config->wTotalLength);
    printf("    bNumInterfaces:          %u\n", config->bNumInterfaces);
    printf("    bConfigurationValue:     %u\n", config->bConfigurationValue);
    printf("    iConfiguration:          %u\n", config->iConfiguration);
    printf("    bmAttributes:            %02xh\n", config->bmAttributes);
    printf("    MaxPower:                %u\n", config->MaxPower);

    for (i = 0; i < config->bNumInterfaces; i++)
        print_interface(&config->interface[i]);
}

static void print_2_0_ext_cap(struct libusb_usb_2_0_extension_descriptor* usb_2_0_ext_cap)
{
    printf("    USB 2.0 Extension Capabilities:\n");
    printf("      bDevCapabilityType:    %u\n", usb_2_0_ext_cap->bDevCapabilityType);
    printf("      bmAttributes:          %08xh\n", usb_2_0_ext_cap->bmAttributes);
}

static void print_ss_usb_cap(struct libusb_ss_usb_device_capability_descriptor* ss_usb_cap)
{
    printf("    USB 3.0 Capabilities:\n");
    printf("      bDevCapabilityType:    %u\n", ss_usb_cap->bDevCapabilityType);
    printf("      bmAttributes:          %02xh\n", ss_usb_cap->bmAttributes);
    printf("      wSpeedSupported:       %u\n", ss_usb_cap->wSpeedSupported);
    printf("      bFunctionalitySupport: %u\n", ss_usb_cap->bFunctionalitySupport);
    printf("      bU1devExitLat:         %u\n", ss_usb_cap->bU1DevExitLat);
    printf("      bU2devExitLat:         %u\n", ss_usb_cap->bU2DevExitLat);
}

static void print_bos(libusb_device_handle* handle)
{
    struct libusb_bos_descriptor* bos;
    uint8_t i;
    int ret;

    ret = libusb_get_bos_descriptor(handle, &bos);
    if (ret < 0)
        return;

    printf("  Binary Object Store (BOS):\n");
    printf("    wTotalLength:            %u\n", bos->wTotalLength);
    printf("    bNumDeviceCaps:          %u\n", bos->bNumDeviceCaps);

    for (i = 0; i < bos->bNumDeviceCaps; i++) {
        struct libusb_bos_dev_capability_descriptor* dev_cap = bos->dev_capability[i];

        if (dev_cap->bDevCapabilityType == LIBUSB_BT_USB_2_0_EXTENSION) {
            struct libusb_usb_2_0_extension_descriptor* usb_2_0_extension;

            ret = libusb_get_usb_2_0_extension_descriptor(NULL, dev_cap, &usb_2_0_extension);
            if (ret < 0)
                return;

            print_2_0_ext_cap(usb_2_0_extension);
            libusb_free_usb_2_0_extension_descriptor(usb_2_0_extension);
        }
        else if (dev_cap->bDevCapabilityType == LIBUSB_BT_SS_USB_DEVICE_CAPABILITY) {
            struct libusb_ss_usb_device_capability_descriptor* ss_dev_cap;

            ret = libusb_get_ss_usb_device_capability_descriptor(NULL, dev_cap, &ss_dev_cap);
            if (ret < 0)
                return;

            print_ss_usb_cap(ss_dev_cap);
            libusb_free_ss_usb_device_capability_descriptor(ss_dev_cap);
        }
    }

    libusb_free_bos_descriptor(bos);
}
int _tmain(int argc, _TCHAR* argv[])
{
    libusb_device** devs;
    libusb_device *dev = NULL; /* the device handle */
	setbuf(stdout, NULL); //force prints to come out without delay
    int ret, i = 0;
    ssize_t cnt;
    uint8_t path[8];
    int retcode = 0;

    ret = libusb_init(NULL);
    if (ret < 0)
    {
        printf("could not initialize WinUSB driver, err = %d\n", ret);
        return ret;
    }
    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0) {
        printf("error getting device list count, err= %l\n", (int) cnt);
        libusb_exit(NULL);
        return (int)cnt;
    }

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            fprintf(stderr, "failed to get device descriptor");
            break;
        }

        if (desc.idVendor == MY_VID && desc.idProduct == MY_PID) {
            printf("%04x:%04x (bus %d, device %d)",
                desc.idVendor, desc.idProduct,
                libusb_get_bus_number(dev), libusb_get_device_address(dev));
        
       
            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0) {
                printf(" path: %d", path[0]);
                for (int j = 1; j < r; j++)
                    printf(".%d", path[j]);
            }
            printf("\n");
            break;
        }

    }
    if (dev == NULL) {
        printf("device %04X:%04X not found\n", MY_VID, MY_PID);
        return -1;
    }

    libusb_device_handle* handle = NULL;
    struct libusb_device_descriptor desc;
    char verbose = 1;
    libusb_open(dev, &handle);
	if (!(handle))
    {
        printf("error opening the handle");
        return 1;
    }
    else
        printf("success: handle to %04X:%04X opened\n", MY_VID, MY_PID);
    ret = libusb_get_device_descriptor(dev, &desc);
    if (ret < 0) {
        fprintf(stderr, "failed to get device descriptor");
        return -1;
    }

    unsigned char string[256];
    if (handle) {
        if (desc.iManufacturer) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
            if (ret > 0)
                printf("  Manufacturer:              %s\n", (char*)string);
        }

        if (desc.iProduct) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, sizeof(string));
            if (ret > 0)
                printf("  Product:                   %s\n", (char*)string);
        }

        if (desc.iSerialNumber && verbose) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, sizeof(string));
            if (ret > 0)
                printf("  Serial Number:             %s\n", (char*)string);
        }
    }

    if (verbose) {
        for (i = 0; i < desc.bNumConfigurations; i++) {
            struct libusb_config_descriptor* config;

            ret = libusb_get_config_descriptor(dev, i, &config);
            if (LIBUSB_SUCCESS != ret) {
                printf("  Couldn't retrieve descriptors\n");
                continue;
            }

            print_configuration(config);

            libusb_free_config_descriptor(config);
        }

        if (handle && desc.bcdUSB >= 0x0201)
            print_bos(handle);
    }



    if (libusb_set_configuration(handle, MY_CONFIG) < 0)
    {
        printf("error setting config #%d\n", MY_CONFIG);
        retcode = 2;
        goto end;
    }
    else
        printf("success: set configuration #%d\n", MY_CONFIG);
	
    if (libusb_claim_interface(handle, 0) < 0)
    {
        printf("error claiming interface #%d:\n", MY_INTF);
        retcode = 3;
        goto end;
    }
    else
        printf("success: claim_interface #%d\n", MY_INTF);
	
   
  

    for (int i = 0; i < sizeof(tbuf); i++)
        tbuf[i] = i;

    int actual_length;
    ret = libusb_bulk_transfer(handle, EP_OUT, tbuf, sizeof(tbuf), &actual_length, 10000);
    if (ret < 0)
        printf("error writing:\n");
    else
        printf("Success! :Wrote %d bytes\n", actual_length);

    //wait for all the data to loopback to the device
    ret = libusb_bulk_transfer(handle, EP_IN, rbuf, sizeof(rbuf), &actual_length, 5000);

    if (ret < 0)
    {
        printf("error reading:\n");
    }

#if 0
    Sleep(500);
    usb_thread((void*)handle);

    while (!usb_thread_done) {
        Sleep(100);
    }
#endif
		
	for (int i = 0; i < sizeof(rbuf); i++)
	{
		//printf("0x%02X\n",rbuf[0] & 0xFF);
		if ((rbuf[i] & 0xFF) != (tbuf[i] & 0xFF))
		{
			printf("Error data corrupt!%d\n",i);
			break;
		}
	}	

end:
    if (handle)
        libusb_close(handle);

    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);
	return retcode;
}

