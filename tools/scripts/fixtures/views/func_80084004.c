/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/spu_tick.h"

void SsSetTickMode(s32 mode)
{
    s32 system=GetVideoMode();
    /* Retain the real active-field address in its configuration phase. */
    register u8 *base __asm__("$2");
    if(mode&0x1000) {
        /* Both word accesses remain inside the complete timer owner. */
        base=(u8 *)&_snd_seq_tick_env+4;
        *(s32 *)base=1;
        *(s32 *)(base-4)=mode&0xFFF;
    } else {
        /* Both word accesses remain inside the complete timer owner. */
        base=(u8 *)&_snd_seq_tick_env+4;
        *(s32 *)base=0;
        *(s32 *)(base-4)=mode;
    }
    if(_snd_seq_tick_env.selector<6) {
        switch(_snd_seq_tick_env.selector) {
        case 4:
            VBLANK_MINUS=50;
            if(system==1) _snd_seq_tick_env.selector=5;
            else _snd_seq_tick_env.selector=50;
            break;
        case 1:
            VBLANK_MINUS=60;
            if(system==0) _snd_seq_tick_env.selector=5;
            else _snd_seq_tick_env.selector=60;
            break;
        case 3:VBLANK_MINUS=120;break;
        case 2:VBLANK_MINUS=240;break;
        case 5:
            if(system==0) VBLANK_MINUS=60;
            else if(system==1) VBLANK_MINUS=50;
            else VBLANK_MINUS=60;
            break;
        case 0:
            if(system==0) VBLANK_MINUS=60;
            else if(system==1) VBLANK_MINUS=50;
            else VBLANK_MINUS=60;
            break;
        default:VBLANK_MINUS=60;
        }
    } else VBLANK_MINUS=_snd_seq_tick_env.selector;
}
