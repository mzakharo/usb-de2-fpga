#ifndef __INC_HAL4D13_H__
#define __INC_HAL4D13_H__

#include "BASICTYP.h"

#define device_name "ISP1362"
#define ram_size 4096

#define SETUP 0
#define OUT   1
#define IN    2

//#define TRUE  1
//#define FALSE 0

//#define ISP1362_BASE  ISP1362_HC_BASE
//Richard #define ISP1362_IRQ_0 ISP1362_IRQ
//Richard #define ISP1362_IRQ_1 ISP1362_IRQ



/*
#define USB_DC_CMD_READ(value)   IORD(ISP1362_DC_BASE, 2)
#define USB_DC_CMD_WRITE(value)  IOWR(ISP1362_DC_BASE, 2, value)
#define USB_DC_DATA_READ(value)  IORD(ISP1362_DC_BASE, 3)
#define USB_DC_DATA_WRITE(value) IOWR(ISP1362_DC_BASE, 3, value)
*/

#define NONISO_FIFOSIZE_0   0
#define NONISO_FIFOSIZE_2   2
#define NONISO_FIFOSIZE_4   4
#define NONISO_FIFOSIZE_8   8
#define NONISO_FIFOSIZE_16  16
#define NONISO_FIFOSIZE_32  32
#define NONISO_FIFOSIZE_64  64

#define ISO_FIFOSIZE_0      0
#define ISO_FIFOSIZE_16     16
#define ISO_FIFOSIZE_32     32
#define ISO_FIFOSIZE_48     48
#define ISO_FIFOSIZE_64     64
#define ISO_FIFOSIZE_96     96
#define ISO_FIFOSIZE_128    128
#define ISO_FIFOSIZE_160    160
#define ISO_FIFOSIZE_192    192
#define ISO_FIFOSIZE_256    256
#define ISO_FIFOSIZE_320    320
#define ISO_FIFOSIZE_384    384
#define ISO_FIFOSIZE_512    512
#define ISO_FIFOSIZE_640    640
#define ISO_FIFOSIZE_768    768
#define ISO_FIFOSIZE_1023   1023

//#define EP0_FIFO_SIZE       NONISO_FIFOSIZE_64
//#define EP0_PACKET_SIZE     NONISO_FIFOSIZE_64

//richard #define EP0_FIFO_SIZE       NONISO_FIFOSIZE_64
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
#define D13REG_DEVADDR_MASK             0x7F
#define D13REG_DEVADDR_EN               0x80

// BMP for EP_CNFG
#define D13REG_EPCNFG_FIFO_EN           0x80
#define D13REG_EPCNFG_IN_EN             0x40
#define D13REG_EPCNFG_DBLBUF_EN         0x20
#define D13REG_EPCNFG_ISO_EN            0x10
#define D13REG_EPCNFG_ISOSZ_MASK        0x0F
#define D13REG_EPCNFG_NONISOSZ_MASK     0x07
#define D13REG_EPCNFG_RFB_EN            0x08

/*Bulk Endpoint Sizes*/
#define D13REG_EPCNFG_NONISOSZ_8        0x00
#define D13REG_EPCNFG_NONISOSZ_16       0x01
#define D13REG_EPCNFG_NONISOSZ_32       0x02
#define D13REG_EPCNFG_NONISOSZ_64       0x03

/*Isochronous Endpoint Sizes*/
#define D13REG_EPCNFG_ISOSZ_16          0x00
#define D13REG_EPCNFG_ISOSZ_32          0x01
#define D13REG_EPCNFG_ISOSZ_48          0x02
#define D13REG_EPCNFG_ISOSZ_64          0x03
#define D13REG_EPCNFG_ISOSZ_96          0x04
#define D13REG_EPCNFG_ISOSZ_128         0x05
#define D13REG_EPCNFG_ISOSZ_160         0x06
#define D13REG_EPCNFG_ISOSZ_192         0x07
#define D13REG_EPCNFG_ISOSZ_256         0x08
#define D13REG_EPCNFG_ISOSZ_320         0x09
#define D13REG_EPCNFG_ISOSZ_384         0x0A
#define D13REG_EPCNFG_ISOSZ_512         0x0B
#define D13REG_EPCNFG_ISOSZ_640         0x0C
#define D13REG_EPCNFG_ISOSZ_768         0x0D
#define D13REG_EPCNFG_ISOSZ_896         0x0E
#define D13REG_EPCNFG_ISOSZ_1023        0x0F

// BMP for EP_STATUS
#define D13REG_EPSTS_STALL              0x80
#define D13REG_EPSTS_DBF1               0x40
#define D13REG_EPSTS_DBF0               0x20
#define D13REG_EPSTS_OVWR               0x08
#define D13REG_EPSTS_SETUP              0x04
#define D13REG_EPSTS_DBFIDX             0x02

// BMP for Error_Code
#define D13REG_ERRCODE_RXTX             0x01
#define D13REG_ERRCODE_DPID             0x40
#define D13REG_ERRCODE_OVRLP            0x80
#define D13REG_ERRCODE_MASK             0x1E
#define D13REG_ERRCODE_OK               0x00
#define D13REG_ERRCODE_PIDERR           0x02
#define D13REG_ERRCODE_PIDNEW           0x04
#define D13REG_ERRCODE_PKTTYP           0x06
#define D13REG_ERRCODE_TKCRC            0x08
#define D13REG_ERRCODE_DATACRC          0x0A
#define D13REG_ERRCODE_TIMEOUT          0x0C
#define D13REG_ERRCODE_RSRV0            0x0E
#define D13REG_ERRCODE_EOP              0x10
#define D13REG_ERRCODE_NAK              0x12
#define D13REG_ERRCODE_STALL            0x14
#define D13REG_ERRCODE_OVRFL            0x16
#define D13REG_ERRCODE_RSRV1            0x18
#define D13REG_ERRCODE_BITSTUFF         0x1A
#define D13REG_ERRCODE_RSRV2            0x1C
#define D13REG_ERRCODE_DATAPID          0x1E

// BMP for LOCK_CMD
#define D13REG_LOCK_IOEN                0x01

// BMP for MODE
#define D13REG_MODE_SOFTCONNECT         0x01
#define D13REG_MODE_OFFGOODLNK          0x02
#define D13REG_MODE_DBG                 0x04
#define D13REG_MODE_INT_EN              0x08
#define D13REG_MODE_SUSPND              0x20
#define D13REG_MODE_DMA16               0x80

// BMP for  CNFG
#define D13REG_DEVCNFG_INTPOL           0x0001
#define D13REG_DEVCNFG_INTEDGE           0x0002
#define D13REG_DEVCNFG_PWROFF           0x0004
#define D13REG_DEVCNFG_WAKEUPBY_CS      0x0008
#define D13REG_DEVCNFG_EOTPOL          0x0010
#define D13REG_DEVCNFG_DMAACKPOL        0x0020
#define D13REG_DEVCNFG_DMARQPOL         0x0040
#define D13REG_DEVCNFG_DMAACKONLY      0x0080
#define D13REG_DEVCNFG_CLOCKDIV_MASK    0x0F00
#define D13REG_DEVCNFG_CLOCKRUNNING     0x1000
#define D13REG_DEVCNFG_NOLAZYCLOCK      0x2000
#define D13REG_DEVCNFG_EXPULLUP         0x4000


#define D13REG_DEVCNFG_CLOCKDIV_480M    0x0000
#define D13REG_DEVCNFG_CLOCKDIV_240M    0x0100
#define D13REG_DEVCNFG_CLOCKDIV_160M    0x0200
#define D13REG_DEVCNFG_CLOCKDIV_120M    0x0300
#define D13REG_DEVCNFG_CLOCKDIV_96M     0x0400
#define D13REG_DEVCNFG_CLOCKDIV_80M     0x0500
#define D13REG_DEVCNFG_CLOCKDIV_69M     0x0600
#define D13REG_DEVCNFG_CLOCKDIV_60M     0x0700
#define D13REG_DEVCNFG_CLOCKDIV_54M     0x0800
#define D13REG_DEVCNFG_CLOCKDIV_48M     0x0900
#define D13REG_DEVCNFG_CLOCKDIV_44M     0x0A00
#define D13REG_DEVCNFG_CLOCKDIV_40M     0x0B00
#define D13REG_DEVCNFG_CLOCKDIV_37M     0x0C00
#define D13REG_DEVCNFG_CLOCKDIV_34M     0x0D00
#define D13REG_DEVCNFG_CLOCKDIV_32M     0x0E00
#define D13REG_DEVCNFG_CLOCKDIV_30M     0x0F00

// BMP for INTERRUPT SRC
#define D13REG_INTSRC_BUSRESET          0x000001
#define D13REG_INTSRC_RESUME            0x000002
#define D13REG_INTSRC_SUSPEND           0x000004

#define D13REG_INTSRC_EOT               0x000008
#define D13REG_INTSRC_SOF               0x000010
#define D13REG_INTSRC_PSEUDO_SOF        0x000020
#define D13REG_INTSRC_SHORT_PACKET      0x000040

#define D13REG_INTSRC_EP0OUT            0x000100
#define D13REG_INTSRC_EP0IN             0x000200
#define D13REG_INTSRC_EP01              0x000400
#define D13REG_INTSRC_EP02              0x000800
#define D13REG_INTSRC_EP03              0x001000
#define D13REG_INTSRC_EP04              0x002000
#define D13REG_INTSRC_EP05              0x004000
#define D13REG_INTSRC_EP06              0x008000
#define D13REG_INTSRC_EP07              0x010000
#define D13REG_INTSRC_EP08              0x020000
#define D13REG_INTSRC_EP09              0x040000
#define D13REG_INTSRC_EP0A              0x080000
#define D13REG_INTSRC_EP0B              0x100000
#define D13REG_INTSRC_EP0C              0x200000
#define D13REG_INTSRC_EP0D              0x400000
#define D13REG_INTSRC_EP0E              0x800000

// BMP for DMA Cnfg
#define D13REG_DMACNFG_BURSTLEN_MASK        0x0003
#define D13REG_DMACNFG_ENABLE               0x0008
#define D13REG_DMACNFG_EPINDX_MASK          0x00F0
#define D13REG_DMACNFG_NONISOPKTSZ_MASK     0x1F00
#define D13REG_DMACNFG_NONISOPKTSZ_EN       0x2000
#define D13REG_DMACNFG_EOTBY_SHORTPKT_EN    0x4000
#define D13REG_DMACNFG_COUNTER_EN           0x8000

#define D13REG_DMACNFG_BURSTLEN0            0x0000
#define D13REG_DMACNFG_BURSTLEN4            0x0001
#define D13REG_DMACNFG_BURSTLEN8            0x0002
#define D13REG_DMACNFG_BURSTLEN16           0x0003

#define HcControl           0x01
#define HcCmdStatus         0x02
#define HcIntStatus         0x03
#define HcIntEnable         0x04
#define HcIntDisable        0x05
#define HcFmItv             0x0D
#define HcFmNo              0x0F
#define HcRhA               0x12
#define HcRhB               0x13
#define HcRhStatus      0x14
#define HcRhP1              0x15
#define HcRhP2              0x16
#define HcIntDone       0x17
#define HcIntSkip       0x18
#define HcIntLast       0x19
#define HcIntActive         0x1A
#define HcATLDone       0x1B
#define HcATLSkip       0x1C
#define HcATLLast       0x1D
#define HcATLActive         0x1E
#define HcHWCfg             0x20
#define HcDMACfg        0x21
#define HcTransferCnt       0x22
#define HcUpInt             0x24
#define HcUpIntEnable       0x25
#define HcChipID            0x27
#define HcScratch           0x28
#define HcBufStatus         0x2C
#define HcPTLLen        0x30
#define HcDirAddrLen        0x32
#define HcINTLen        0x33
#define HcATLLen            0x34
#define HcPTL0_Port     0x40
#define HcPTL1_Port     0x42
#define HcInt_Port      0x43
#define HcATL_Port          0x44
#define HcDirAddr_Port      0x45
#define HcPTLTogRate        0x47
#define HcATLThrsCnt        0x51
#define HcATLTimeOut        0x52
#define HcIntBlkSize        0x53
#define HcATLBlkSize        0x54
#define OTGControl      0x62
#define OTGStatus           0x67
#define OTGInterrupt            0x68
#define OTGInterruptEnable      0x69
#define OTGTimerControl         0x6A
#define HcReset             0xA9

//  Port Offset for ISP1362
// richard #define HC_DATA_PORT      0
// richard #define HC_COMMAND_PORT   1
// richard #define D13_DATA_PORT     0  //2
// richard #define D13_COMMAND_PORT  1  //3

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

#endif //__INC_HAL4D13_H__





