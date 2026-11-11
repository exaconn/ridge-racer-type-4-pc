#include "rr/commands.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
extern s32 SysDeqIntRP(s32,void *);
s32 _cardSioHandler(void) {
    s32 result;
    InterruptRegisters *registers;
    u32 count=CardTxCount+1U;
    CardTxCount=count;
    result=_cardCmdStep();
    registers=I_STAT_Card;
    registers->pending=~0x80U;
    registers->mask|=0x80;
    if(result!=0) {
        s32 flag;
        void *event;
        {
            s32 one=1;
            flag=1;
            if(result==one) {
                s32 retry=CardRetriesLeft;
                CardCommandStatus=0x41;
                if(retry!=0) {
                    _cardCmdReset(CardCommandRequest);
                    CardCommandStatus=2;
                    CardRetriesLeft=CardRetriesLeft-1;
                    flag=1;
                } else {
                    flag=1;
                }
                event=CardIrqQueueEntryPtr;
                /* Stop the SIO transfer through the actual hardware register. */
                CardSio0Regs->control=0;
            } else {
                event=CardIrqQueueEntryPtr;
                /* Stop the SIO transfer through the actual hardware register. */
                CardSio0Regs->control=0;
                CardCommandStatus=0x21;
            }
        }
        CardTxCount=0;
        SysDeqIntRP(flag,event);
    }
    return 0;
}
