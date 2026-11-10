#ifndef RR_PSYQ_LIBPAD_PADMETA_H
#define RR_PSYQ_LIBPAD_PADMETA_H

#include "rr/channels.h"

/* A native combination descriptor grows by its pointer width; padding before
 * the descriptor array also belongs to storage, not to the protocol budget. */
static __inline__ u8 *PadActInfoLimit(PadChannel *channel) {
    if (sizeof(ChannelCombEntry8)==PAD_COMB_WIRE_BYTES) {
        return &channel->modeCountE3;
    } else {
        s32 modesBytes=((channel->modeCountE3+1)>>1)*4;
        s32 entriesBytes=(channel->entry_countE9*5+3)&0xFFC;
        u8 *packedCombinations=(u8 *)channel->modes00+modesBytes+entriesBytes;
        long alignmentBytes=(u8 *)channel->combs08-packedCombinations;
        return channel->metadata63+PAD_ACT_INFO_BYTES+alignmentBytes
            + channel->combCountEA*(sizeof(ChannelCombEntry8)-PAD_COMB_WIRE_BYTES);
    }
}

#endif
