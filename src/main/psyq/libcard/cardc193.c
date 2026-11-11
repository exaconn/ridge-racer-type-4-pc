/* RODATA 0x800121e0 0x24 */
#include "rr/commands.h"
#include "rr_asm.h"
s32 _cardGetIdRx(s32 value) {
    switch (CardRxCount) {
    case 2:CardBytesRemaining=8;break;
    case 3:if ((value&255)!=0x5A) return -1;break;
    case 7:*(u32 *)CardCmdResultPtrA=value&255;break;
    case 8:*(u32 *)CardCmdResultPtrA=(*(u32 *)CardCmdResultPtrA<<8)+(value&255);break;
    case 9:*CardCmdResultPtrB=value&255;break;
    case 10:{
        u32 *word=CardCmdResultPtrB;
        RR_NOP_DEP_ON(word);
        *word=(*word<<8)+(value&255);
        break;
    }
    }
    return 0;
}
