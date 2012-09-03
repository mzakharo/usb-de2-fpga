#ifndef __INC_BASICTYPE_H__
#define __INC_BASICTYPE_H__

//??????#define code
//??????#define idata

//??????#define IN
//??????#define OUT

#define IRQL_1
#define IRQL_0

#define FALSE   0
#define TRUE    1

//*************************************************************************
// basic typedefs
//*************************************************************************

#define CHAR    char
#define SHORT   short
#define UCHAR	unsigned char
#define USHORT	unsigned short
#define ULONG	unsigned long
#define BOOL	unsigned char


#define PCHAR   (CHAR *)
#define PUCHAR	(UCHAR *)
#define PUSHORT	(USHORT *)
#define PULONG	(ULONG *)
#define BOOLEAN UCHAR

typedef union _Address {
	USHORT  IoPort;     /* IoData Port */
	UCHAR * pData;      /* Memory Address */
} ADDRESS;

#endif //__INC_BASICTYPE_H__


