
#ifndef __CHAP_9_H__
#define __CHAP_9_H__


// *************************************************************************
// basic #defines
// *************************************************************************






// *************************************************************************
// USB Protocol Layer
// *************************************************************************

// *************************************************************************
// USB standard device requests
// *************************************************************************

void Chap9_GetStatus(void);
void Chap9_ClearFeature(void);
void Chap9_SetFeature(void);
void Chap9_SetAddress(void);
void Chap9_GetDescriptor(void);
void Chap9_GetConfiguration(void);
void Chap9_SetConfiguration(void);
void Chap9_GetInterface(void);
void Chap9_SetInterface(void);

   // *************************************************************************
   // Chap9 support functions
   // *************************************************************************

void Chap9_StallEP0(void);
void Chap9_SingleTransmitEP0(UCHAR * buf, USHORT len);
void Chap9_BurstTransmitEP0(UCHAR * pData, USHORT len);
#endif
