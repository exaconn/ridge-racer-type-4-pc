/* GCC_VERSION: 2.7.2 */
/* RODATA 0x800121e0 0x24 */
#include "rr/commands.h"
#include "rr_asm.h"
s32 _cardGetIdRx(s32 value) {
    /* ASM_BARRIER: Share the original result pointer register between accumulation cases. */
    register u32 *word __asm__("$2");
    switch (CardRxCount) {
    case 2:D_80114730=8;break;
    case 3:if ((value&255)!=0x5A) return -1;break;
    case 7:*(u32 *)D_8011471C=value&255;break;
    case 8:word=D_8011471C;goto append;
    case 9:*D_80114720=value&255;break;
    case 10:word=D_80114720;
        /* ASM_BARRIER: Place the pointer load gap before the shared accumulation entry. */
        RR_NOP_DEP_ON(word);
    append:*word=(*word<<8)+(value&255);break;
    }
    return 0;
}
