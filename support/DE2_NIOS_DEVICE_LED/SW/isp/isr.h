
#ifndef __ISP1362_ISR_H__
#define __ISP1362_ISR_H__

void usb_isr(void);
int Get_Interrupt_number_HEX(int IRQ_NUMBER);
void Get_PIC_Mask(int* PIC_mask1 , int* PIC_mask2 ,int IRQ_NUMBER );


void fn_usb_isr(void);
void Isr_BusReset(void);
void Isr_SuspendChange(void);
void Isr_DmaEot(void);
void Isr_SOF(void);
void Isr_SHORT_PACKET(void);
void Isr_Ep00TxDone(void);
void Isr_Ep00RxDone(void);
void Isr_Ep01Done(void);
void Isr_Ep02Done(void);
void Isr_Ep03Done(void);
void Isr_Ep04Done(void);
void Isr_Ep05Done(void);
void Isr_Ep06Done(void);
void Isr_Ep07Done(void);
void Isr_Ep08Done(void);
void Isr_Ep09Done(void);
void Isr_Ep0ADone(void);
void Isr_Ep0BDone(void);
void Isr_Ep0CDone(void);
void Isr_Ep0DDone(void);
void Isr_Ep0EDone(void);

#endif  //__ISP1362_ISR_H__

