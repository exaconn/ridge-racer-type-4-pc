/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
extern s32 SysDeqIntRP(s32,void *);
s32 _cardSioHandler(void) {
    s32 result;
    InterruptRegisters *registers;
    s32 *page;
    u32 count=CardTxCount+1U;
    page=(s32 *)0x800B0000U;
    page[-0x4218/4]=count;
    result=_cardCmdStep();
    registers=I_STAT_Card;
    registers->pending=~0x80U;
    registers->mask|=0x80;
    if(result!=0) {
        /* ASM_BARRIER: Prepare the event argument in the original branch delay slots. */
        register s32 flag __asm__("$4");
        void *event;
        /* ASM_BARRIER: Keep argument preparation after the zero-result gate. */
        __asm__("" : : : "$2", "$4");
        {
            s32 one=1;
            /* ASM_BARRIER: Retain the explicit success-comparison value before the branch. */
            __asm__("" : "+r"(one) : : "$4");
            flag=1;
            if(result==one) {
                s32 retry=CardRetriesLeft;
                /* ASM_BARRIER: Retain argument preparation and retry load before status store. */
                __asm__ volatile("" : "+r"(flag) : "r"(retry));
                CardCommandStatus=0x41;
                flag=1;
                if(retry!=0) {
                    _cardCmdReset(CardCommandRequest);
                    CardCommandStatus=2;
                    CardRetriesLeft=CardRetriesLeft-1;
                    /* ASM_BARRIER: Finish retry bookkeeping before preparing the event argument again. */
                    __asm__("" : : : "$4", "memory");
                    flag=1;
                }
                event=CardIrqQueueEntryPtr;
                /* Ordinary write view preserves the original jump-delay store. */
                *(u16 *)&CardSio0Regs->control=0;
            } else {
                event=CardIrqQueueEntryPtr;
                /* Ordinary write view preserves the original jump-delay store. */
                *(u16 *)&CardSio0Regs->control=0;
                CardCommandStatus=0x21;
            }
        }
        page=(s32 *)0x800B0000U;
        page[-0x4218/4]=0;
        SysDeqIntRP(flag,event);
    }
    return 0;
}
