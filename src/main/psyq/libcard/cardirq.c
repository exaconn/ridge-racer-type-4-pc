#include "rr/interrupts.h"
#include "rr/serial.h"
#include "psyq/libcard/cardirq.h"
#include "rr/commands.h"
#include "rr/spu_tick_control.h"

extern int ChangeClearRCnt();
extern int SysEnqIntRP();
extern int SysDeqIntRP();
extern int CardIrqQueueEntry;

int _cardVsyncHandler();

void _cardStartCom(void)
{
    InterruptRegisters *q;
    CardSio0Regs->control = 0x40;
    CardSio0Regs->baud = 0x88;
    CardSio0Regs->mode = 0xD;
    EnterCriticalSection();
    SysDeqIntRP(2, &CardIrqQueueEntry);
    SysEnqIntRP(2, &CardIrqQueueEntry);
    q = I_STAT_Card;
    q->pending = ~1U;
    *(u32 *)&q->mask = q->mask | 1;
    ChangeClearRCnt(3, 0);
    ExitCriticalSection();
}

void _cardStopCom() { EnterCriticalSection(); ChangeClearRCnt(3, 1); SysDeqIntRP(2, &CardIrqQueueEntry); ExitCriticalSection(); }

s32 _cardChkVsyncIrq(void) {
    InterruptRegisters *registers=I_STAT_Card;
    s32 result;
    if(registers->mask&1) {
        if(registers->pending&1) result=1;
        else {
            result=0;
        }
    } else result=0;
    return result;
}

s32 _cardChkSioIrq(void) {
    InterruptRegisters *registers=I_STAT_Card;
    s32 result;
    if(registers->mask&0x80) {
        if(registers->pending&0x80) result=1;
        else {
            result=0;
        }
    } else result=0;
    return result;
}

int _cardVsyncHandler() {
    InterruptRegisters *g;
    SerialPortRegisters *h;
    if (CardTxCount) {
        g = I_STAT_Card;
        CardCommandStatus = 0x11;
        CardTxCount = 0;
        g->pending = ~0x80U;
        g->mask &= ~0x80U;
        h = CardSio0Regs;
        *(u16 *)&h->control = 0;
        SysDeqIntRP(1, CardIrqQueueEntryPtr);
        CardSio0Regs->control = 0x40;
        CardSio0Regs->baud = 0x88;
        CardSio0Regs->mode = 0xD;
        CardSio0Regs->control = 0;
        return 0;
    }
    if (CardCommandStatus & 1) return 0;
    SysDeqIntRP(1, CardIrqQueueEntryPtr);
    SysEnqIntRP(1, CardIrqQueueEntryPtr);
    _cardSioHandler();
    return 0;
}
