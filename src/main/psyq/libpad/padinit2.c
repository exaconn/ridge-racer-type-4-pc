#include "rr/layout.h"
#include "psyq/libpad/padpoll.h"
#include "psyq/libpad/padwait.h"
#include "rr/serial.h"
#include "psyq/libpad/padrecv.h"

#include "rr/channel_init.h"

extern s32 PadMultitapActive,PadPacketBytesLeft,D_800ABCA4;
extern s32 (*D_800ABC98)(void),(*D_800ABC94)(void);
s32 _padRecvStage0(PadChannel *channel);
s32 _padRecvStage1(PadChannel *channel);
s32 _padRecvStage2(PadChannel *channel);
s32 _padRecvStage3(PadChannel *channel);
s32 _padTransferPacket(PadChannel *channel);

s32 _padRecvStage0(PadChannel *a0) {
    int r = PadChannelDispatchFn(a0);
    u8 *p = a0->packet;
    PadMultitapActive = r;
    *p = 0;
    return _padSioRW(a0, -2);
}

s32 _padRecvStage1(PadChannel *o) {
    s32 result;
    if (PadActiveChannel == PadFirstChannel) {
        if (D_800ABCA4 != 0) {
            D_800ABC98();
            D_800ABC94();
        }
    }
    if (PadMultitapActive != 0) {
        PadChannelDispatchFn(o->children0C);
        PadChannelDispatchFn(o->children0C + 1);
    }
    if (o->command37 == 0) result = _padSioRW2(o, 0x42);
    else result = _padSioRW2(o, o->command37);
    return result;
}


s32 _padRecvStage2(PadChannel *a0) {
  int a1, v, t;
  if (PadMultitapActive != 0) {
    PadChannelDispatchFn(a0->children0C + 2);
    PadChannelDispatchFn(a0->children0C + 3);
  }
  a1 = 0;
  if (a0->command37 == 0) a1 = PadMultitapEnabled;
  v = _padSioRW2(a0, a1);
  if (v < 0) return v;
  t = (v & 0xF) * 2;
  PadPacketBytesLeft = t;
  if (t == 0) PadPacketBytesLeft = 0x20;
  return 0;
}


s32 _padRecvStage3(PadChannel *state) {
    s32 mode = 0;
    s32 result;

    if ((state->packet[0] >> 4) == 8) {
        mode = state->command37 == 0;
    }
    result = _padSioRW2(state, (u8)PadTxByteCallback(state, mode));
    if (result == 0x5A || result == 0 || result < 0) return result;
    return -4;
}

s32 _padTransferPacket(PadChannel *arg0) {
    s32 i;
    s32 res;
    s32 idx;
    PadChannel *ptr;
    s32 mode;
    s32 b;

    PadAllocGatesFn(arg0);
    mode = 0;
    if (PadMultitapEnabled != 0 && ((b = arg0->packet[0]) >> 4) == 8) {
        mode = (arg0->command37 == 0);
    }

    if (mode != 0) {
        for (i = -1; i < 4; i++) {
            if (--PadPacketBytesLeft <= 0) {
                break;
            }
            if (i >= 0) {
                PadAllocGatesFn(&arg0->children0C[i]);
            }
            res = _padSioRW2(arg0, (u8)PadTxByteCallback(arg0, 1));
            if (res < 0) {
                return res;
            }
            setRC2wait(0x3C);
            if (_padClrIntSio0() == 0) {
                return -3;
            }
        }
    }

    ptr = 0;
    idx = (PadActiveChannel == 0);
    while (PadPacketBytesLeft >= 2) {
        if (PadMultitapSlotIndex[idx] < 0) {
            break;
        }
        if (PadMultitapSlotIndex[idx] > 0) {
            ptr = &PadChannelRecords[idx].children0C[PadMultitapSlotIndex[idx]] - 1;
            PadPacketDoneCallback(ptr);
        }
        switch (PadMultitapSlotIndex[idx]) {
        case 4:
            PadMultitapSlotIndex[idx] = 3;
            break;
        case 3:
            PadPacketDoneCallback(ptr - 1);
            PadMultitapSlotIndex[idx] = 1;
            break;
        case 0:
        case 1:
            ptr = &PadChannelRecords[idx];
            PadPacketDoneCallback(ptr);
            PadSaveCommandCallback(ptr);
            PadMultitapSlotIndex[idx] = -1;
            break;
        }
        res = _padSioRW(arg0, (u8)PadTxByteCallback(arg0, mode));
        if (res < 0) {
            return res;
        }
        setRC2wait(0x3C);
        if (_padClrIntSio0() == 0) {
            return -3;
        }
        PadPacketBytesLeft--;
    }

    while (--PadPacketBytesLeft > 0) {
        res = _padSioRW(arg0, (u8)PadTxByteCallback(arg0, mode));
        if (res < 0) {
            return res;
        }
        if (JOY_DATA->baud != 0x22) {
            setRC2wait(0x3C);
            if (_padClrIntSio0() == 0) {
                res = -3;
                goto end;
            }
        }
    }

    _padWaitRXready();
    arg0->packet[arg0->b44++] = JOY_DATA->data;
    PadAdvanceChannelFn(0);
    res = 0;
end:
    return res;
}

