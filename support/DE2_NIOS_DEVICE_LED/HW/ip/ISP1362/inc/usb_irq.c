#include "usb_irq.h"
#include "system.h"
#include "sys/alt_irq.h"

alt_irq_context status;
unsigned char INT_Enabled=1;

void disable(void)
{
  if(INT_Enabled)
  {
    status=alt_irq_disable_all();
    INT_Enabled=0;
  }
}

void enable(void)
{
  if(!INT_Enabled)
  {
    alt_irq_enable_all(status);
    INT_Enabled=1;    
  }
}
