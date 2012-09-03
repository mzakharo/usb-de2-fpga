#ifndef ISP1362_HAL_H_
#define ISP1362_HAL_H_

#include "basic_io.h"
#include <stdio.h>
extern int debug;

/*
 A0: phase selection
    0: data phase
    1: command phase
 A1: bus selection
    0: host (HC)
    1: device (DC)
*/

#define USB_HC_DATA_READ()          IORD(ISP1362_HC_BASE, 0)
#define USB_HC_DATA_WRITE(value)    IOWR(ISP1362_HC_BASE, 0, value)
#define USB_HC_CMD_READ()           IORD(ISP1362_HC_BASE, 1)
#define USB_HC_CMD_WRITE(value)     IOWR(ISP1362_HC_BASE, 1, value)


#define USB_DC_DATA_READ()          IORD(ISP1362_DC_BASE, 0)
#define USB_DC_DATA_WRITE(value)    if(debug)printf("Dx%04X\n", value); IOWR(ISP1362_DC_BASE, 0, value)
#define USB_DC_CMD_READ()           IORD(ISP1362_DC_BASE, 1)
#define USB_DC_CMD_WRITE(value)     if (debug)printf("Cx%04X\n", value); IOWR(ISP1362_DC_BASE, 1, value)

#endif /*ISP1362_DRIVER_H_*/
