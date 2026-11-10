/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
s32 SsUtKeyOffV(s32 a0) {
    int r;
    int h = a0;
    int selected = a0;
    if (_snd_ev_flag == 1) { r = -1; goto out; }
    _snd_ev_flag = 1;
    if ((selected & 0xFFFF) >= 0x18U) { 
        int *page;
        r=-1;
        page=(int *)0x80110000U;
page[-0xFA4/4]=0;
        goto out; }
    _svm_voice_cursor = h;
    /* ASM_BARRIER: Keep the selector in a1 until the reset call. */
    __asm__("" : : "r"(selected));
    _SsVmKeyOffNow(0);
    _snd_ev_flag = 0;
    r = 0;
out:
    return r;
}
