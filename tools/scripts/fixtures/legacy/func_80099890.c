#include "rr/layout.h"

/* Partial transfer state; field purposes beyond this routine are still unknown. */
typedef struct {
    u8 pad00[0x37];
    u8 flag37;
    u8 pad38[4];
    u8 *packet;
} TransferState;

RR_ASSERT_SIZE(TransferState, 0x40);
RR_ASSERT_OFFSET(TransferState, flag37, 0x37);
RR_ASSERT_OFFSET(TransferState, packet, 0x3C);

extern s32 (*PadTxByteCallback)(TransferState *, s32);
extern s32 _padSioRW2(TransferState *, u8);

s32 _padRecvStage3(TransferState *state) {
    s32 mode = 0;
    s32 result;

    if ((state->packet[0] >> 4) == 8) {
        mode = state->flag37 == 0;
    }
    result = _padSioRW2(state, (u8)PadTxByteCallback(state, mode));
    if (result == 0x5A || result == 0 || result < 0) return result;
    return -4;
}
