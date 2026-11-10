/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/layout.h"
#include "psyq/libpad/padpatch.h"

#include "psyq/libpad/padmeta.h"

extern u8 *PadPatchScanPtr;
void _padSendActInfoCmd(void *raw);
s32 _padRecvActInfoReply(void *raw);
void _padActAlignUpdate(void *raw);
s32 _padActAlignComplete(void *raw);

void _padSendActInfoCmd(void *raw) {
    PadChannel *s=raw;
    switch (s->phase) {
    case 2: _padCmd4C(s, s->state47); break;
    case 3: _padCmd46(s, s->state47); break;
    case 4:
        if (s->flag48 == 0) _padCmd47(s, s->state47);
        else _padCmd4B(s);
        break;
    }
}

s32 _padRecvActInfoReply(void *raw) {
    PadChannel *s=raw;
    u8 *q;
    ChannelGateEntry5 *p5;
    ChannelCombEntry8 *e8;
    unsigned char *src;
    int i;
    int t;

    switch (s->phase) {
    case 2:
        if (s->packet[2] != 0) return 0;
        if (s->packet[3] != 0) return 0;
        s->modes00[s->state47] = s->packet[5] + (s->packet[4] << 8);
        if (s->scanSeenEE != s->modes00[s->state47]) {
            s->scanSeenEE = s->modes00[s->state47];
            return 0;
        }
        s->scanSeenEE = 0;
        s->scanIndexEB = 0;
        if (++s->state47 < s->modeCountE3) return 0;
        s->state47 = 0;
        break;

    case 3:
        q = s->packet;
        if (q[2] != 0) return 0;
        if (q[3] != 0) return 0;
        p5 = &s->entries04[s->state47];
        if (p5->opaque00[0] == q[4] && p5->opaque00[1] == (*(unsigned char *)&q[5] & 0x7F) && p5->all_bits02 == q[6] && p5->cost03 == q[7] && p5->opaque04 == ((t = q[5]) >> 7)) {
            s->scanSeenEE = 0;
        } else {
            s->scanSeenEE = 0xFFFF;
        }
        p5->opaque00[0] = s->packet[4];
        p5->opaque00[1] = s->packet[5] & 0x7F;
        p5->all_bits02 = s->packet[6];
        p5->cost03 = s->packet[7];
        t = s->packet[5];
        p5->opaque04 = t >> 7;
        if (s->scanSeenEE != 0) return 0;
        s->scanIndexEB = 0;
        if (++s->state47 < s->entry_countE9) return 0;
        s->state47 = 0;
        s->flag48 = 0;
        break;

    case 4:
        q = s->packet;
        if (q[2] != 0) {
            s->flag48 = 0;
            return 0;
        }
        e8 = &s->combs08[s->state47];
        if (s->flag48 == 0) {
            i = 3;
            e8->count00 = s->flag48 = q[4];
            src = &s->packet[5];
            if (s->state47 == 0) {
                PadPatchScanPtr = e8->buttons04 = (unsigned char *)(s->combs08 + s->combCountEA);
            } else {
                PadPatchScanPtr = e8->buttons04 = e8[-1].buttons04 + ((e8[-1].count00 + 3) & 0x1FC);
            }
        } else {
            i = 5;
            src = &q[3];
        }
        while (i-- != 0) {
            if (s->flag48 == 0) break;
            if (PadPatchScanPtr >= PadActInfoLimit(s)) {
                s->state47 = 0;
                s->flag48 = 0;
                return 0;
            }
            if (*PadPatchScanPtr != *src) {
                s->scanSeenEE = 0xFFFF;
            }
            {
                unsigned char **pp = &PadPatchScanPtr;
                unsigned char *p = *pp;
                *p = *src++;
                *pp = p + 1;
            }
            s->flag48--;
        }
        if (s->flag48 == 0) {
            if (s->scanSeenEE != 0) {
                s->scanSeenEE = 0;
                s->flag48 = 0;
                return 0;
            }
            if (++s->state47 >= s->combCountEA) {
                s->state49 = 6;
                s->phase = 0xFE;
                s->scanIndexEB = 0;
                return 0;
            }
            s->flag48 = 0;
            s->scanIndexEB = 0;
            return 0;
        }
        return 0;

    default:
        return 1;
    }
    return 1;
}

s32 _padSetActAlign(void *raw, u8 *layout) {
    PadChannel *channel=raw;
    s32 result;
    if (PadChannelBusyFn(raw) == 0) {
        result=1;
        channel->phase=1;
        channel->update14=_padActAlignUpdate;
        channel->layout20=layout;
        channel->complete18=_padActAlignComplete;
        return result;
    }
    return 0;
}

void _padActAlignUpdate(void *raw) {
    PadChannel *channel=raw;
    channel->command37=0x4D;
    channel->length36=6;
    channel->data2C=channel->layout20;
}

s32 _padActAlignComplete(void *raw)
{
    PadChannel *state=raw;
    s32 group, minimum, remaining, i;
    u8 *slots;

    /* Groups with fewer slots than their minimum are marked unassigned. */
    group = 0;
    if (state->entry_countE9 != 0) {
        do {
            slots = state->layout20;
            remaining = 0;
            for (i = 5; i >= 0; i--) {
                if (*slots++ == group) remaining++;
            }
            minimum = state->entries04[group].all_bits02;
            slots = state->layout20;
            if (minimum == 0) minimum = 1;
            i = 0;
            do {
                if (*slots++ == group) {
                    if (remaining < minimum) {
                        state->output54.fields.payload[i] = 0xFF;
                        remaining--;
                    } else {
                        state->output54.fields.payload[i] = group;
                    }
                }
                i++;
            } while (i < 6);
            group++;
        } while (group < state->entry_countE9);
    }
    state->phase = 0xFE;
    return 0;
}
