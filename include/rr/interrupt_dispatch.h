#ifndef RR_INTERRUPT_DISPATCH_H
#define RR_INTERRUPT_DISPATCH_H
#include "rr/library.h"
#include "rr/diagnostics.h"
extern RrLibSavedState D_800A52C8;
extern volatile u16 *D_800A6354,*I_MASK;
extern s32 IntrStuckCount;
extern const char UnexpectedInterruptFmt[],D_80011858[];
void ReturnFromException(void);
void trapIntr(void);
/* I_STAT acknowledgements write zero to the serviced bits. The native protocol
 * fixture supplies the register's write-zero-to-clear behavior here. */
#ifndef RR_IRQ_ACK16
#define RR_IRQ_ACK16(port,value) (*(port)=(u16)(value))
#endif
#endif
