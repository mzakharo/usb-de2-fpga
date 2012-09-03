//#include "isp1362_includes.h"
#include <io.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "BasicTyp.h"
#include "common.h"
#include "HAL4D13.h"
#include "usb_irq.h"
#include "ISP1362_HAL.h"


#ifdef DEBUG_USB_HAL4D13
    #define USB_HAL4D13_DEBUG(x)    DEBUG(x)  
#else
    #define USB_HAL4D13_DEBUG(x)
#endif

/*
// ************************************************************************
// Subroutines -- 1
// ************************************************************************
*/
void Hal4D13_AcquireD13(unsigned char g_host_IRQ_num,void *my_usb_isr)
//void Hal4D13_AcquireD13(unsigned char g_host_IRQ_num)
{
  alt_irq_register(g_host_IRQ_num,NULL,(void*)my_usb_isr);
}

void Hal4D13_ResetDevice(void)
{
  USB_DC_CMD_WRITE(D13CMD_DEV_RESET);
}

USHORT Hal4D13_GetDevConfig(void)
{
  USHORT i; 
  USB_DC_CMD_WRITE(D13CMD_DEV_RD_CNFG);
  i = USB_DC_DATA_READ();
  return i;
}

void Hal4D13_SetDevConfig(USHORT wDevCnfg)
{
  USB_DC_CMD_WRITE(D13CMD_DEV_WR_CNFG);
  USB_DC_DATA_WRITE( wDevCnfg);
}

void Hal4D13_SetMode(UCHAR bMode)
{
  USB_DC_CMD_WRITE( D13CMD_DEV_WR_MODE);
  USB_DC_DATA_WRITE( bMode);
}

UCHAR Hal4D13_GetMode(void)
{
    UCHAR c;   
  USB_DC_CMD_WRITE( D13CMD_DEV_RD_MODE);
  c = (UCHAR)(USB_DC_DATA_READ() & 0x0ff);
    return c;
}

UCHAR Hal4D13_GetAddress(void)
{
    UCHAR c; 
    USB_DC_CMD_WRITE( D13CMD_DEV_RD_ADDR);
    c = (UCHAR)(USB_DC_DATA_READ() & 0x0ff);
    return c;
}

void Hal4D13_SetAddressEnable(UCHAR bAddress, UCHAR bEnable)
{
  USB_DC_CMD_WRITE( D13CMD_DEV_WR_ADDR);
  if(bEnable)
  bAddress |= D13REG_DEVADDR_EN;
  else
  bAddress &= D13REG_DEVADDR_MASK;
  USB_DC_DATA_WRITE( bAddress);
}

void Hal4D13_SoftConnect(UCHAR bEnable)
{
    UCHAR c;
  USB_DC_CMD_WRITE( D13CMD_DEV_RD_MODE);
  c = (UCHAR)(USB_DC_DATA_READ() & 0x0ff);
  if(bEnable)
  c |= D13REG_MODE_SOFTCONNECT;
  else
  c &= ~D13REG_MODE_SOFTCONNECT;  
    USB_DC_CMD_WRITE( D13CMD_DEV_WR_MODE);
  USB_DC_DATA_WRITE( c);
}

void Hal4D13_AcknowledgeSETUP(void)
{
  USB_DC_CMD_WRITE( D13CMD_ACK_SETUP);
}

void Hal4D13_ClearBuffer(UCHAR bEPIndex)
{
  /* Select Endpoint */
    USB_DC_CMD_WRITE( D13CMD_EP_CLEAR_BUF + bEPIndex);
}

void Hal4D13_ValidBuffer(UCHAR bEPIndex)
{
    USB_DC_CMD_WRITE( D13CMD_EP_VALID_BUF + bEPIndex);
}

USHORT Hal4D13_ReadEndpoint(UCHAR bEPIndex, UCHAR *buf, USHORT len)
{
    USHORT i, j, c;
    USB_DC_CMD_WRITE( D13CMD_EP_RD_FIFO + bEPIndex);
    /* read Buffer */
    j = USB_DC_DATA_READ();
    if(j > len)
        j = len;
    i=0;
    while (i<j)//<<
    //for(i=0; i<j; i=i+2, buf++ )
    {
        c = USB_DC_DATA_READ();
        *buf = (UCHAR)c;
        i++;//<<
        if (i >= j) break; //<<
        buf++;
        *buf = (UCHAR)(c>>8);
        i++;//<<
         buf++;
    }
    /* Clear Buffer */
    USB_DC_CMD_WRITE( D13CMD_EP_CLEAR_BUF+bEPIndex);
    return j;

}

USHORT Hal4D13_ReadBulkEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len)
{
    USHORT i, j, c;
    USB_DC_CMD_WRITE( D13CMD_EP_RD_FIFO + bEPIndex);
    j = USB_DC_DATA_READ();
    if(j > len)
        j = len;
    i=0;
    while (i<j) //<<
//  for(i=0; i<j; i=i+2, buf++ )
    {
        c = USB_DC_DATA_READ();
        *buf = (UCHAR)c;
        i++;//<<
        if (i >= j) break; //<<
        buf++;
        *buf = (UCHAR)(c>>8);
    
        i++;//<<
    buf++;
    }
    /* Clear Buffer */
    USB_DC_CMD_WRITE( D13CMD_EP_CLEAR_BUF+bEPIndex);

    return j;
}

USHORT Hal4D13_ReadEndpointWOClearBuffer(UCHAR bEPIndex, UCHAR * buf, USHORT len)
{
    USHORT i, j,c;
    USB_DC_CMD_WRITE( D13CMD_EP_RD_FIFO + bEPIndex);
    /* read Buffer */
    j = USB_DC_DATA_READ();
    if(j > len)
        j = len;
    i=0;
    while (i<j) //<<    
   //for(i<j; i=i+2, buf++ )
    {
         c = USB_DC_DATA_READ();
         *buf = (UCHAR)c;
         USB_HAL4D13_DEBUG(("WOC= %02X ",*buf));//<<        
         i++;//<<
         if (i >= j) break; //<<
         buf++;
         *buf = (UCHAR)(c>>8); 
         USB_HAL4D13_DEBUG(("WOC= %02X ",*buf));//<<
   // richard   if (*buf==0x5) IOWR(LED_RED_BASE, 0 , 0xc1 );
         i++;//<<
     buf++;
    }
    USB_HAL4D13_DEBUG(("(read done)\n"));// richard ,c);
    /* Clear Buffer */
    USB_DC_CMD_WRITE( D13CMD_EP_CLEAR_BUF+bEPIndex);

    return j;
}

USHORT Hal4D13_WriteEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len)
{
    USHORT i,c;
    /* Select Endpoint */
   
    USB_DC_CMD_WRITE( D13CMD_EP_WR_FIFO + bEPIndex);
    USB_DC_DATA_WRITE( len);
    if (len==0) USB_DC_DATA_WRITE(0);
    /* Write Buffer */
  
    //for(i=0; i<len; i=i+2, buf++ )
  i=0;
  while (i < len)
    {
        c = *buf;
        buf++;i++;
    if (i >=len) { USB_DC_DATA_WRITE(c); }//printf("w=%04x  ",c);break; }
        c =(c & 0xff)+ (*buf<<8  & 0xff00);    
    buf++;i++;      
        USB_DC_DATA_WRITE( c);
//    printf("w=%04x  ",c);
    }
//  printf("\nlen=%d i=%d\n",len,i);
  
    /* Validate Buffer */
    USB_DC_CMD_WRITE( D13CMD_EP_VALID_BUF+bEPIndex);
 // printf("\n");
    return i;
}

USHORT Hal4D13_WriteBulkEndpoint(UCHAR bEPIndex, UCHAR * buf, USHORT len)
{
    USHORT i, c;
    /* Select Endpoint */
    USB_DC_CMD_WRITE( D13CMD_EP_WR_FIFO + bEPIndex);
    USB_DC_DATA_WRITE( len);
    if (len==0) USB_DC_DATA_WRITE(0);

    /* Write Buffer */
    for(i=0; i<len; i=i+2, buf++ )
    {
        c = *buf;
        buf++;
        c += (USHORT)(*buf << 8);
        USB_DC_DATA_WRITE(c);
    }
    /* Validate Buffer */
    USB_DC_CMD_WRITE(D13CMD_EP_VALID_BUF+bEPIndex);
    return len;
}

ULONG Hal4D13_GetIntEnable(void)
{
    ULONG i;
    USB_DC_CMD_WRITE(D13CMD_DEV_RD_INTEN);
    i = USB_DC_DATA_READ();
    i += (((ULONG)USB_DC_DATA_READ()) << 16);
    return i;
}

void Hal4D13_SetIntEnable(ULONG dIntEn)
{
    USB_DC_CMD_WRITE(D13CMD_DEV_WR_INTEN);
    dIntEn = (dIntEn & 0x0FFFFFF);
    USB_DC_DATA_WRITE((USHORT)dIntEn);
    USB_DC_DATA_WRITE((USHORT)(dIntEn >> 16));
}

ULONG Hal4D13_ReadInterruptRegister(void)
{
    ULONG j,i = 0;
    USB_DC_CMD_WRITE(D13CMD_DEV_INT_SRC);
    i = USB_DC_DATA_READ();
    j = USB_DC_DATA_READ();
    j = ((j<<16) & 0xffff0000 ) + (i & 0xffff);
    return i;
}

UCHAR Hal4D13_GetEndpointStatusWInteruptClear(UCHAR bEPIndex)
{
    UCHAR c;
    USB_DC_CMD_WRITE( D13CMD_EP_RDSTS_CLRINT + bEPIndex);
    c = (UCHAR)(USB_DC_DATA_READ() & 0x0ff);
    return c;
}

UCHAR Hal4D13_GetEndpointStatusWOInteruptClear(UCHAR bEPIndex)
{
    UCHAR c;
    USB_DC_CMD_WRITE( D13CMD_EP_RDSTS + bEPIndex);
    c = (UCHAR)(USB_DC_DATA_READ() &0x0ff);
    return c;
}

void Hal4D13_SetEndpointStatus(UCHAR bEPIndex, UCHAR bStalled)
{
    if(bStalled & 0x80)
    {
        USB_DC_CMD_WRITE( D13CMD_EP_WR_STS + bEPIndex);
    }
    else
    {
    // clear endpoint stall.
        USB_DC_CMD_WRITE( D13CMD_EP_CLR_STALL + bEPIndex);
    }
}

void Hal4D13_SetDMAConfig(USHORT wDMACnfg)
{
    USB_DC_CMD_WRITE(D13CMD_DMA_WR_CNFG);
    USB_DC_DATA_WRITE( wDMACnfg);
}

USHORT Hal4D13_GetDMAConfig(void)
{
    USHORT i;
    USB_DC_CMD_WRITE(D13CMD_DMA_RD_CNFG);
    i = USB_DC_DATA_READ();
    return i;
}

void Hal4D13_SetDMACounter(USHORT wDMACounter)
{
    USB_DC_CMD_WRITE(D13CMD_DMA_WR_COUNT);
    USB_DC_DATA_WRITE( wDMACounter);
}

USHORT Hal4D13_GetDMACounter(void)
{
    USHORT i;
    USB_DC_CMD_WRITE(D13CMD_DMA_RD_COUNT);
    i = USB_DC_DATA_READ();
    return i;
}

USHORT Hal4D13_GetDataFromChipRam(void)
{
    USHORT i;
    USB_DC_CMD_WRITE(D13CMD_DEV_RD_RAM);
    i = USB_DC_DATA_READ();
    return i;
}

void Hal4D13_SetDataToChipRam(USHORT wData)
{
    USB_DC_CMD_WRITE(D13CMD_DEV_WR_RAM);
    USB_DC_DATA_WRITE( wData);
}

USHORT Hal4D13_ReadCurrentFrameNumber(void)
{
    USHORT i;
    USB_DC_CMD_WRITE( D13CMD_RD_FRMNUM);
    i= USB_DC_DATA_READ();
    return i;
}

void Hal4D13_LockDevice(UCHAR bTrue)
{
    USHORT c;
    if(bTrue)
        c = 0;
    else
        c = 0xAA37;
    USB_DC_CMD_WRITE(D13CMD_DEV_LOCK );
    USB_DC_DATA_WRITE(c);
}

USHORT Hal4D13_ReadChipID(void)
{
    USHORT i;
    USB_DC_CMD_WRITE( D13CMD_DEV_RD_CHIPID);
    usleep(1);
    i=USB_DC_DATA_READ();
    return i;
}

UCHAR Hal4D13_GetErrorCode(UCHAR bEPIndex)
{
    UCHAR   c;
    USB_DC_CMD_WRITE( D13CMD_EP_RD_ERR+bEPIndex);
    c = (UCHAR)(USB_DC_DATA_READ()&0x0ff);
    return c;
}

UCHAR Hal4D13_GetEndpointConfig(UCHAR bEPIndex)
{
    UCHAR   c;
    USB_DC_CMD_WRITE( D13CMD_EP_RD_CNFG+bEPIndex);
    c = (UCHAR)(USB_DC_DATA_READ() & 0x0ff);
    return c;
}

void Hal4D13_SetEndpointConfig(UCHAR bEPConfig, UCHAR bEPIndex)
{
    USB_DC_CMD_WRITE( (USHORT)(D13CMD_EP_WR_CNFG+bEPIndex));
    USB_DC_DATA_WRITE((USHORT)bEPConfig);
}

/*
// ************************************************************************
// Subroutines -- 3
// ************************************************************************
*/
void Hal4D13_UnconfigDevice(void)
{
}

void Hal4D13_ConfigDevice(void)
{
}


void Hal4D13_SingleTransmitEP0(UCHAR * buf, USHORT len)
{
    if( len <= EP0_PACKET_SIZE) {
        Hal4D13_WriteEndpoint(EPINDEX4EP0_CONTROL_IN, buf, len);
    }
}

void Hal4D13_RegAccess(void)
{
    USHORT   c;
    USHORT  i;
    ULONG   l;

    Hal4D13_ResetDevice();
    printf("Reseting\n");

    i = Hal4D13_ReadChipID();
    printf(" ChipId = %hx\n",i);

    i = Hal4D13_GetDataFromChipRam();
    printf(" Chip RAM = %hx\n",i);

    printf("Writing 0x55AA,");
    Hal4D13_SetDataToChipRam(0x55AA);
    i = Hal4D13_GetDataFromChipRam();
    printf(" Chip RAM = %hx\n",i);

    l = Hal4D13_GetIntEnable();
    printf("Hal4D13_GetIntEnable = %lx\n",l);

    l = Hal4D13_ReadInterruptRegister();
    printf("Hal4D13_ReadInterruptRegister = %lx\n",l);

    i = Hal4D13_GetDevConfig();
    printf("Hal4D13_GetDevConfig = %hx\n",i);

    i = Hal4D13_GetDMAConfig();
    printf("Hal4D13_GetDMAConfig = %hx\n",i);

    i = Hal4D13_GetDMACounter();
    printf("Hal4D13_GetDMACounter = %hx\n",i);

    i = Hal4D13_ReadCurrentFrameNumber();
    printf("Hal4D13_ReadCurrentFrameNumber = %hx\n",i);

    c = Hal4D13_GetMode();
    printf("Hal4D13_GetMode = %x\n",c);

    c = Hal4D13_GetEndpointStatusWOInteruptClear(EPINDEX4EP0_CONTROL_OUT);
    printf("Hal4D13_GetEndpointStatus EP0 = %x\n",c);

    c = Hal4D13_GetAddress();
//  printf("Hal4D13_GetAddress = %x\n",c);
}

void Hal4D13_StallEP0InControlWrite(void)
{
    Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_IN, D13REG_EPSTS_STALL);
    Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_OUT, D13REG_EPSTS_STALL);
}

void Hal4D13_StallEP0InControlRead(void)
{
    Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_OUT, D13REG_EPSTS_STALL);
    Hal4D13_SetEndpointStatus(EPINDEX4EP0_CONTROL_IN, D13REG_EPSTS_STALL);
}

BOOLEAN Hal4D13_IsSetupPktInvalid(void)
{
    USHORT SetupPacketInvalid;
    SetupPacketInvalid = Hal4D13_GetEndpointStatusWOInteruptClear(EPINDEX4EP0_CONTROL_OUT)&D13REG_EPSTS_OVWR;
    return SetupPacketInvalid;
}

void Write_HC_Config_Reg(unsigned short Config)
{
    USB_HC_CMD_WRITE(HCCMD_WR_CNFG_REG );
    USB_HC_DATA_WRITE(Config);
}

unsigned short Read_HC_Config_Reg(void)
{
    USB_HC_CMD_WRITE(HCCMD_RD_CNFG_REG );
    return USB_HC_DATA_READ();
}

