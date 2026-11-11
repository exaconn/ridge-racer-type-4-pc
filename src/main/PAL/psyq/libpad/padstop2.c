/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/channel_init.h"

void _padInitDirSeq(void) {
    ChannelUpdateCallback update;
    u8 *base;
    PadChannelDispatchFn = _padChannelDispatch;
    PadChannelBusyFn = _padChannelBusy;
    update = _padPacketDone;
    /* PAL RAM mapping: PadPacketDoneCallback is base minus 0x4378. */
    base = (u8 *)0x800B0000U;
    ((ChannelUpdateCallback *)base)[-0x4378 / (s32)sizeof(ChannelUpdateCallback)] = update;
}

s32 _padChannelDispatch(void *a0) {
    PadChannel *state=a0;
    /* ASM_BARRIER: Separate the saved channel from the early-call argument. */
    __asm__("" : "+r"(state));
    if(*state->packet==0xF3) {
        if(!state->responseE8)goto Y;
        if(state->phase==0xFF)goto zero;
        if(state->state49==2)PadResetChannelFn(state);
    }
    switch(state->phase) {
    case 1:
        _padCmdParaMode(state,1);
        return 0;
    case 0xFE:goto zero;
    case 0:case 0xFF:return 0;
    default:goto other;
    }
zero:
    a0=state;
Y:
    _padCmdParaMode(a0,0);
    return 0;
other:
    if(!state->update14)goto fallback;
    state->update14(state);
    goto out;
fallback:
    _padSendAtLoadInfo((unsigned char *)state);
out:
    return 0;
}
