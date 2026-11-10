/* CC1_FLAGS: -mno-split-addresses */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libpad/padstat2.h"
#include "rr/channel_init.h"

void _padPacketDone(void *raw) {
    PadChannel *a0=raw;
    unsigned int t;
    int a1;
    unsigned char b;
    int i;
    t = a0->packet[0];
    if ((t & 0xF0) == 0) {
        a0->input30[0] = 0xFF;
        a0->input30[1] = 0;
        a0->responseE8 = 0;
        a0->pad35 = 0;
        PadResetChannelFn(a0);
        return;
    }
    a1 = a0->responseE8;
    a0->responseE8 = t >> 4;
    if ((t >> 4) == 0xF) {
        a0->responseE8 = a1;
    } else {
        a0->input30[0] = 0;
        a0->input30[1] = a0->packet[0];
        a0->pad35 = *(volatile unsigned char *)&a0->b44;
        for (i = 2; i < a0->b44; i++) a0->input30[i] = a0->packet[i];
    }
    if (a0->packet[1] == 0) {
        if (a0->phase == 1 && a0->update14 == 0) goto l10;
        if (a0->b50 != 0) goto l10;
        goto l11;
    }
l10:
    if (a0->responseE8 != a1) {
l11:
        PadResetChannelFn(a0);
    }
    b = a0->phase;
    a0->retry4A = 0;
    if (b != 0xFF) {
        if (b != 0 && a0->command37 == 0) return;
        if ((unsigned char)(b - 2) < 0xFC) {
            if (a0->packet[0] != 0xF3) { PadResetChannelFn(a0); return; }
        }
        {
            unsigned char sw;
            int one;
            switch (sw = a0->phase, one = 1, sw) {
            case 0:
                a0->state49 = one;
                a0->phase += 1;
                break;
            case 1:
                a0->state47 = 0;
                a0->phase += 1;
                break;
            case 0xFE:
                a0->phase = 0xFF;
                break;
            default: {
                if (a0->complete18 != 0) a0->phase += a0->complete18(a0);
                else a0->phase += _padRecvAtLoadInfo(a0);
                break;
            }
            }
        }
    }
}

void _dirFailAuto(u8 *raw) {
  PadChannel *a0=(PadChannel *)raw;
  int v = a0->phase;
  unsigned int t;
  a0->type4C = a0->type4C + 1;
  if (v != 0) {
    if (v == 1) {
      t = a0->retry4A;
      if (t >= 0xB) {
        a0->state49 = 2;
        a0->phase = 0xFF;
        return;
      }
      a0->retry4A = t + 1;
      return;
    }
    t = a0->retry4A;
    if (t < 0xB) {
      a0->retry4A = t + 1;
      return;
    }
    if (a0->state49 != 0) PadResetChannelFn(a0);
  }
  if (*a0->packet != 0xF3) {
    a0->input30[0] = 0xFF;
    a0->input30[1] = 0;
    a0->responseE8 = 0;
    a0->pad35 = 0;
  }
}
