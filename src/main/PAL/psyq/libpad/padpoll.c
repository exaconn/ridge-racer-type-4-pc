/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/counters.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
#include "psyq/libpad/padtm22.h"
#include "psyq/libpad/padpoll.h"

int _padSioRW2(PadChannel *a0, int a1) {
    int s1;
    int s2;
    unsigned v1;
    SerialPortRegisters *hw;
    int c;
    s1 = 0x88;
    c = *a0->packet;
    if ((c >> 4) == 8) {
        if (a0->b44 >= 9) s1 = 0x22;
    }
    do { } while (!(PadSio0Base->status & 2));
    setRC2wait(0x190);
    hw = PadSio0Base;
    s2 = hw->data;
    if (a0->b44 != 0 || (s2 >> 4) != 8) {
        hw->baud = s1;
    } else {
        hw->baud = 0x22;
    }
    while (!(I_STAT_Pad->pending & 0x80)) {
        v1 = ((RootCounterRegisters *)0x1F801120)->count;
        if (v1 < PadRcnt2Start) {
            if (((RootCounterRegisters *)0x1F801128)->count != 0) v1 += ((RootCounterRegisters *)0x1F801128)->count;
            else v1 += 0x10000;
        }
        if (((RootCounterRegisters *)0x1F801124)->count & 0x200) {
            if (v1 - PadRcnt2Start >= RC2WaitLimit) return -2;
        } else {
            if (((v1 - PadRcnt2Start) >> 3) >= RC2WaitLimit) return -2;
        }
    }
    if (a0->responseE8 != 8) {
        if (PadRecvStage == 2) {
            setRC2wait(0x3C);
            do { } while (chkRC2wait() == 0);
        }
    }
    PadSio0Base->data = a1;
    if (PadRecvStage == 3) {
        if (s2 == 0x80) {
            I_STAT_Pad->pending = ~0x80U;
            PadSio0Base->control |= 0x10;
        }
    }
    a0->b45++;
    if (a0->b44 != 0xFF) {
        unsigned char *p;
        int idx;
        idx = *(unsigned char *)&a0->b44;
        p = a0->packet + idx;
        *p = s2;
    }
    a0->b44++;
    return s2;
}

int _padClrIntSio0() {
    I_STAT_Pad->pending = ~0x80U;
    if (PadSio0Base->status & 0x80) {
        do {
            if (chkRC2wait()) return 0;
        } while (PadSio0Base->status & 0x80);
    }
    PadSio0Base->control |= 0x10;
    return 1;
}
