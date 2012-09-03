#ifndef __INC_HAL_H__
#define __INC_HAL_H__

#include "basictyp.h"

#define device_name "ISP1362"



#define NONISO_FIFOSIZE_0    0x0000
#define NONISO_FIFOSIZE_2    0x0002
#define NONISO_FIFOSIZE_4    0x0004
#define NONISO_FIFOSIZE_8    0x0008
#define NONISO_FIFOSIZE_16   0x0010
#define NONISO_FIFOSIZE_32   0x0020
#define NONISO_FIFOSIZE_64   0x0040
#define ISO_FIFOSIZE_0       0x0000
#define ISO_FIFOSIZE_16      0x0010
#define ISO_FIFOSIZE_32      0x0020
#define ISO_FIFOSIZE_48      0x0030
#define ISO_FIFOSIZE_64      0x0040
#define ISO_FIFOSIZE_96      0x0060
#define ISO_FIFOSIZE_128     0x0080
#define ISO_FIFOSIZE_160     0x00A0
#define ISO_FIFOSIZE_192     0x00C0
#define ISO_FIFOSIZE_256     0x0100
#define ISO_FIFOSIZE_320     0x0140
#define ISO_FIFOSIZE_384     0x0180
#define ISO_FIFOSIZE_512     0x0200
#define ISO_FIFOSIZE_640     0x0280
#define ISO_FIFOSIZE_768     0x0300
#define ISO_FIFOSIZE_1023    0x03FF

#define EP0_PACKET_SIZE     NONISO_FIFOSIZE_64

//  Endpoint Index
#define EPINDEX4EP0_CONTROL_OUT     0x0
#define EPINDEX4EP0_CONTROL_IN      0x1
#define EPINDEX4EP01                0x2
#define EPINDEX4EP02                0x3
#define EPINDEX4EP03                0x4
#define EPINDEX4EP04                0x5
#define EPINDEX4EP05                0x6
#define EPINDEX4EP06                0x7
#define EPINDEX4EP07                0x8
#define EPINDEX4EP08                0x9
#define EPINDEX4EP09                0xA
#define EPINDEX4EP0A                0xB
#define EPINDEX4EP0B                0xC
#define EPINDEX4EP0C                0xD
#define EPINDEX4EP0D                0xE
#define EPINDEX4EP0E                0xF

//  Command Set

#define D13CMD_DEV_RESET            0xF6
#define D13CMD_DEV_RD_CHIPID        0xB5
#define D13CMD_DEV_WR_ADDR          0xB6
#define D13CMD_DEV_RD_ADDR          0xB7
#define D13CMD_DEV_WR_MODE          0xB8
#define D13CMD_DEV_RD_MODE          0xB9
#define D13CMD_DEV_WR_CNFG          0xBA
#define D13CMD_DEV_RD_CNFG          0xBB
#define D13CMD_DEV_INT_SRC          0xC0
#define D13CMD_DEV_WR_INTEN         0xC2
#define D13CMD_DEV_RD_INTEN         0xC3
#define D13CMD_DEV_WR_RAM           0xB2
#define D13CMD_DEV_RD_RAM           0xB3
#define D13CMD_DEV_LOCK             0xB0

#define D13CMD_DMA_RD_COUNT         0xF3
#define D13CMD_DMA_WR_COUNT         0xF2
#define D13CMD_DMA_RD_CNFG          0xF1
#define D13CMD_DMA_WR_CNFG          0xF0

#define D13CMD_ACK_SETUP            0xF4
#define D13CMD_RD_FRMNUM            0xB4

#define D13CMD_EP_WR_FIFO           0x00
#define D13CMD_EP_RD_FIFO           0x10
#define D13CMD_EP_WR_CNFG           0x20
#define D13CMD_EP_RD_CNFG           0x30
#define D13CMD_EP_WR_STS            0x40
#define D13CMD_EP_CLR_STALL         0x80
#define D13CMD_EP_RDSTS_CLRINT      0x50
#define D13CMD_EP_VALID_BUF         0x60
#define D13CMD_EP_CLEAR_BUF         0x70
#define D13CMD_EP_RDSTS             0xD0
#define D13CMD_EP_RD_ERR            0xA0

// BMP for DevADDR
#define DEVADDR_MASK             0x7F
#define DEVADDR_EN               0x80

// BMP for EP_CNFG
#define EPCNFG_FIFO_EN           0x80
#define EPCNFG_IN_EN             0x40
#define EPCNFG_DBLBUF_EN         0x20
#define EPCNFG_ISO_EN            0x10
#define EPCNFG_ISOSZ_MASK        0x0F
#define EPCNFG_NONISOSZ_MASK     0x07
#define EPCNFG_RFB_EN            0x08

/*Bulk Endpoint Sizes*/
#define EPCNFG_NONISOSZ_8        0x00
#define EPCNFG_NONISOSZ_16       0x01
#define EPCNFG_NONISOSZ_32       0x02
#define EPCNFG_NONISOSZ_64       0x03

/*Isochronous Endpoint Sizes*/
#define EPCNFG_ISOSZ_16          0x00
#define EPCNFG_ISOSZ_32          0x01
#define EPCNFG_ISOSZ_48          0x02
#define EPCNFG_ISOSZ_64          0x03
#define EPCNFG_ISOSZ_96          0x04
#define EPCNFG_ISOSZ_128         0x05
#define EPCNFG_ISOSZ_160         0x06
#define EPCNFG_ISOSZ_192         0x07
#define EPCNFG_ISOSZ_256         0x08
#define EPCNFG_ISOSZ_320         0x09
#define EPCNFG_ISOSZ_384         0x0A
#define EPCNFG_ISOSZ_512         0x0B
#define EPCNFG_ISOSZ_640         0x0C
#define EPCNFG_ISOSZ_768         0x0D
#define EPCNFG_ISOSZ_896         0x0E
#define EPCNFG_ISOSZ_1023        0x0F

// BMP for EP_STATUS
#define EPSTS_STALL              0x80
#define EPSTS_DBF1               0x40
#define EPSTS_DBF0               0x20
#define EPSTS_OVWR               0x08
#define EPSTS_SETUP              0x04
#define EPSTS_DBFIDX             0x02

// BMP for Error_Code
#define ERRCODE_RXTX             0x01
#define ERRCODE_DPID             0x40
#define ERRCODE_OVRLP            0x80
#define ERRCODE_MASK             0x1E
#define ERRCODE_OK               0x00
#define ERRCODE_PIDERR           0x02
#define ERRCODE_PIDNEW           0x04
#define ERRCODE_PKTTYP           0x06
#define ERRCODE_TKCRC            0x08
#define ERRCODE_DATACRC          0x0A
#define ERRCODE_TIMEOUT          0x0C
#define ERRCODE_RSRV0            0x0E
#define ERRCODE_EOP              0x10
#define ERRCODE_NAK              0x12
#define ERRCODE_STALL            0x14
#define ERRCODE_OVRFL            0x16
#define ERRCODE_RSRV1            0x18
#define ERRCODE_BITSTUFF         0x1A
#define ERRCODE_RSRV2            0x1C
#define ERRCODE_DATAPID          0x1E

// BMP for LOCK_CMD
#define LOCK_IOEN                0x01

// BMP for MODE
#define MODE_SOFTCONNECT         0x01
#define MODE_DBG                 0x04
#define MODE_INT_EN              0x08
#define MODE_SUSPND              0x20

// BMP for  CNFG
#define DEVCNFG_INTPOL           0x0001
#define DEVCNFG_INTEDGE           0x0002
#define DEVCNFG_PWROFF           0x0004
#define DEVCNFG_WAKEUPBY_CS      0x0008
#define DEVCNFG_EOTPOL          0x0010
#define DEVCNFG_DMAACKPOL        0x0020
#define DEVCNFG_DMARQPOL         0x0040
#define DEVCNFG_DMAACKONLY      0x0080
#define DEVCNFG_CLOCKDIV_MASK    0x0F00
#define DEVCNFG_CLOCKRUNNING     0x1000
#define DEVCNFG_NOLAZYCLOCK      0x2000
#define DEVCNFG_EXPULLUP         0x4000


// BMP for INTERRUPT SRC
#define INTSRC_BUSRESET          0x000001
#define INTSRC_RESUME            0x000002
#define INTSRC_SUSPEND           0x000004

#define INTSRC_EOT               0x000008
#define INTSRC_SOF               0x000010
#define INTSRC_PSEUDO_SOF        0x000020
#define INTSRC_SHORT_PACKET      0x000040

#define INTSRC_EP0OUT            0x000100
#define INTSRC_EP0IN             0x000200
#define INTSRC_EP01              0x000400
#define INTSRC_EP02              0x000800
#define INTSRC_EP03              0x001000
#define INTSRC_EP04              0x002000
#define INTSRC_EP05              0x004000
#define INTSRC_EP06              0x008000
// #define INTSRC_EP07              0x010000
// #define INTSRC_EP08              0x020000
// #define INTSRC_EP09              0x040000
// #define INTSRC_EP0A              0x080000
// #define INTSRC_EP0B              0x100000
// #define INTSRC_EP0C              0x200000
// #define INTSRC_EP0D              0x400000
// #define INTSRC_EP0E              0x800000

// BMP for DMA Cnfg
#define DMACNFG_BURSTLEN_MASK        0x0003
#define DMACNFG_ENABLE               0x0008
#define DMACNFG_EPINDX_MASK          0x00F0
#define DMACNFG_NONISOPKTSZ_MASK     0x1F00
#define DMACNFG_NONISOPKTSZ_EN       0x2000
#define DMACNFG_EOTBY_SHORTPKT_EN    0x4000
#define DMACNFG_COUNTER_EN           0x8000

#define DMACNFG_BURSTLEN0            0x0000
#define DMACNFG_BURSTLEN4            0x0001
#define DMACNFG_BURSTLEN8            0x0002
#define DMACNFG_BURSTLEN16           0x0003



/* 1362 Commands*/
#define HCCMD_RD_CNFG_REG 0x20
#define HCCMD_WR_CNFG_REG 0xA0
/* 1362 Commands*/

// Member Functions
void Hal4D13_ResetDevice(void);
void Hal4D13_SetDataToChipRam(USHORT wData);
void Hal4D13_SetDevConfig(USHORT wDevCnfg);
void Hal4D13_SetDMAConfig(USHORT wDMAConfig);
void Hal4D13_SetDMACounter(USHORT wDMACounter);

USHORT Hal4D13_GetDMACounter(void);
USHORT Hal4D13_GetDevConfig(void);
USHORT Hal4D13_GetDMAConfig(void);
USHORT Hal4D13_ReadChipID(void);
USHORT Hal4D13_GetDataFromChipRam(void);

ULONG Hal4D13_GetIntEnable(void);
ULONG Hal4D13_ReadInterruptRegister(void);


void Hal4D13_LockDevice(UCHAR bTrue);
UCHAR Hal4D13_GetErrorCode(UCHAR bEPIndex);
UCHAR Hal4D13_GetMode(void);
void Hal4D13_SetMode(UCHAR bMode);

UCHAR Hal4D13_GetEndpointConfig(UCHAR bEPIndex);
void Hal4D13_SetEndpointConfig(UCHAR bEPConfig, UCHAR bEPIndex);

void Hal4D13_SetAddressEnable(UCHAR bAddress, UCHAR bEnable);
UCHAR Hal4D13_GetAddress(void);

void Hal4D13_SoftConnect(UCHAR bEnable);

void Hal4D13_SetIntEnable(ULONG dIntEn);

UCHAR Hal4D13_GetEndpointStatusWInteruptClear(UCHAR bEPIndex);
UCHAR Hal4D13_GetEndpointStatusWOInteruptClear(UCHAR bEPIndex);
void Hal4D13_SetEndpointStatus(UCHAR bEPIndex, UCHAR bStalled);

void Hal4D13_AcknowledgeSETUP(void );
void Hal4D13_ClearBuffer(UCHAR bEPIndex);
void Hal4D13_ValidBuffer(UCHAR bEPIndex);

USHORT Hal4D13_ReadEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len);
USHORT Hal4D13_ReadEndpointWOClearBuffer(UCHAR bEPIndex, UCHAR * buf, USHORT len);
USHORT Hal4D13_WriteEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len);

USHORT Hal4D13_WriteBulkEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len);
USHORT Hal4D13_ReadBulkEndpoint(UCHAR bEPIndex, UCHAR  * buf, USHORT len);

USHORT Hal4D13_ReadCurrentFrameNumber(void);


void Hal4D13_StallEP0InControlWrite(void);
void Hal4D13_StallEP0InControlRead(void);
void Hal4D13_SingleTransmitEP0(UCHAR * buf, USHORT len);
void Hal4D13_UnconfigDevice(void);
void Hal4D13_ConfigDevice(void);
void Hal4D13_RegAccess(void);

BOOLEAN Hal4D13_IsSetupPktInvalid(void);
void Hal4D13_AcquireD13(unsigned char g_host_IRQ_num,void *my_usb_isr);
void Hal4D13_ReleaseD13(int IRQ_NUMBER);


void Write_HC_Config_Reg(unsigned short Config);
unsigned short Read_HC_Config_Reg(void);

#endif //__INC_HAL_H__





