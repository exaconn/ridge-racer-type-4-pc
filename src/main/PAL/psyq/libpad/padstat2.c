/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/channels.h"
#include "psyq/libpad/padstat2.h"

#include "psyq/libpad/padpatch.h"

void _padSetAct(void *arg0, u8 *arg1, s32 arg2);
void _padSetCmd(void *arg0, s8 arg1, u8 *arg2, s8 arg3);
s32 _padGetActSize(const PadChannel *channel);
s32 _padLoadActInfo(PadChannel *channel,u8 *arena);

void _padSetAct(void *arg0, u8 *arg1, s32 arg2) {
    ((PadChannel *)arg0)->data28 = arg1;
    ((PadChannel *)arg0)->length34 = arg2;
}

void _padSetCmd(void *arg0, s8 arg1, u8 *arg2, s8 arg3) {
    ((PadChannel *)arg0)->command37 = arg1;
    ((PadChannel *)arg0)->data2C = arg2;
    ((PadChannel *)arg0)->length36 = arg3;
}

void _padSendAtLoadInfo(u8 *channel) {
    PadChannel *state=(PadChannel *)channel;
    switch(state->phase) {
    case 2:_padCmd45(state);break;
    case 3:_padCmd4C(state,state->argumentE4);break;
    case 4:_padCmd47(state,state->state47);break;
    }
}

s32 _padRecvAtLoadInfo(void *raw) {
    PadChannel *a0=raw;
    u8 *q;
    int t, n;
    /* ASM_BARRIER: pin b4 to $v1 so the load/add chain takes the original register allocation */
#ifdef __mips__
    register int b __asm__("$3");
#else
    int b;
#endif
    switch (a0->phase) {
    case 2:
        q = a0->packet;
        if (q[7] != 0) goto r0;
        if (a0->modeCountE3 == q[3] && a0->argumentE4 == q[4] && a0->entry_countE9 == q[5] && a0->combCountEA == q[6]) {
            a0->scanSeenEE = 0;
        } else {
            a0->scanSeenEE = 0xFFFF;
        }
        a0->modeCountE3 = a0->packet[3];
        a0->argumentE4 = a0->packet[4];
        a0->pending = 0;
        a0->entry_countE9 = a0->packet[5];
        a0->combCountEA = a0->packet[6];
        a0->scanOffsetEC = 0;
        if (a0->scanSeenEE != 0) goto r0;
        a0->scanIndexEB = 0;
        goto r1;
    case 3:
        if (a0->packet[2] != 0) goto r0;
        if (a0->packet[3] != 0) goto r0;
        t = a0->packet[5] + (a0->packet[4] << 8);
        a0->pending = t;
        if (a0->scanSeenEE == (t & 0xFFFF)) goto eq;
        a0->scanSeenEE = t;
    r0:
        return 0;
    eq:
        a0->scanSeenEE = 0xFFFF;
        a0->scanIndexEB = 0;
        a0->state47 = 0;
        goto r1;
    case 4:
        if (a0->packet[2] != 0) goto r0;
        if (a0->packet[3] != 0) goto r0;
        t = a0->scanOffsetEC;
        b = a0->packet[4];
        t += 8;
        t += (b + 3) & 0x1FC;
        n = a0->state47 + 1;
        a0->state47 = n;
        a0->scanOffsetEC = t;
        if ((unsigned char)n < a0->combCountEA) goto r0;
        if (_padGetActSize(a0) >= 0x81) {
            PadResetChannelFn(a0);
            a0->phase = 0xFE;
            a0->state49 = 2;
            goto r0;
        }
        if (a0->scanSeenEE != a0->scanOffsetEC) {
            a0->scanSeenEE = a0->scanOffsetEC;
            a0->state47 = 0;
            a0->scanOffsetEC = 0;
            return 0;
        }
        a0->scanSeenEE = 0;
        a0->scanIndexEB = 0;
        a0->phase = 0xFF;
        _padLoadActInfo(a0, a0->metadata63);
        a0->phase = 2;
        goto r0;
    default:
        return 1;
    }
r1:
    return 1;
}

s32 _padGetActSize(const PadChannel *channel) {
    s32 modes=channel->modeCountE3;
    s32 actuators=channel->entry_countE9;
    s32 combinations=channel->scanOffsetEC;
    s32 modesBytes=((modes+1)>>1)*4;
    s32 entriesBytes=((actuators*5+3)&0xFFC)+4;
    return modesBytes+entriesBytes+combinations;
}

s32 _padLoadActInfo(PadChannel *channel,u8 *arena) {
    s32 result;
    long modesAddress;
    u8 *combinations;
    if (arena == 0 || channel->entries04 != 0 || PadChannelBusyFn(channel) != 0)
        return 0;
    result=1;
    channel->state49=4;
    /* The arena may begin at an unaligned byte in the channel record. */
    if (__alignof__(ChannelCombEntry8)==4)
        modesAddress=((long)arena+3)>>2<<2;
    else
        modesAddress=((long)arena+__alignof__(ChannelCombEntry8)-1)
            & -__alignof__(ChannelCombEntry8);
    channel->modes00=(u16 *)modesAddress;
    channel->entries04=(ChannelGateEntry5 *)((u8 *)channel->modes00
        + ((channel->modeCountE3+1)>>1<<2));
    combinations=(u8 *)channel->entries04
        + ((channel->entry_countE9*5+3)&0xFFC);
    if (__alignof__(ChannelCombEntry8)>4)
        combinations=(u8 *)(((long)combinations+__alignof__(ChannelCombEntry8)-1)
            & -__alignof__(ChannelCombEntry8));
    channel->combs08=(ChannelCombEntry8 *)combinations;
    channel->phase=1;
    channel->update14=_padSendActInfoCmd;
    channel->state47=0;
    channel->complete18=_padRecvActInfoReply;
    return result;
}
