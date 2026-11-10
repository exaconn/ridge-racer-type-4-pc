/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
#include "psyq/libpad/padpoll.h"
#include "psyq/libpad/padwait.h"
#include "psyq/libpad/padenab.h"
#include "psyq/libpad/padopen.h"

struct W { int x; };

extern int EnterCriticalSection();
extern int ChangeClearRCnt();
extern int SysDeqIntRP();
extern int ExitCriticalSection();
extern struct W PadMultitapSlotIndex[];
extern void (*PadSaveCommandCallback)();

void _padStopCom() { EnterCriticalSection(); ChangeClearRCnt(3, 1); SysDeqIntRP(2, &PadIntQueueEntry); ExitCriticalSection(); }

s32 _padInitSioMode(PadChannel *a0) {
    struct W *p;
    SerialPortRegisters *hw;
    int v;
    int t;
    int cur;
    PadSio0Base->control = 0x40;
    PadSio0Base->control = 0;
    PadSio0Base->mode = 0xD;
    PadSio0Base->baud = 0x88;
    setRC2wait(a0->responseE8 == 8 ? 0x50 : 0x91);
    hw = PadSio0Base;
    t = 0x1003;
    if (PadActiveChannel != 0) t = 0x3003;
    hw->control = t;
    if ((cur = (&PadMultitapSlotIndex[PadActiveChannel])->x) >= 0) {
        struct W *base;
        if (cur > 0) {
            base = PadMultitapSlotIndex;
        lp:
            v = base[PadActiveChannel].x - 1;
            base[PadActiveChannel].x = v;
            PadPacketDoneCallback(a0->children0C + v);
            if (base[PadActiveChannel].x > 0) goto lp;
        }
        p = &PadMultitapSlotIndex[PadActiveChannel];
        if (p->x == 0) {
            p->x = -1;
            PadPacketDoneCallback(a0);
            PadSaveCommandCallback(a0);
        }
    }
    if (PadSio0Base->status & 0x200) {
        PadSio0Base->control |= 0x10;
        if (PadSio0Base->status & 0x200) {
            while (chkRC2wait() == 0);
            *(unsigned char *)&PadSio0Base->data = 1;
            setRC2wait(0x64);
            if (_padClrIntSio0() == 0) return 0;
            _padWaitRXready();
            PadSio0Base->data;
            setRC2wait(0x1AE);
            goto l1;
        l1b:
            if (chkRC2wait() != 0) return 0;
        l1:
            if (!(I_STAT_Pad->pending & 0x80)) goto l1b;
            *(unsigned char *)&PadSio0Base->data = 0x42;
            setRC2wait(0x3C);
            if (_padClrIntSio0() == 0) return 0;
            _padWaitRXready();
            PadSio0Base->data;
            setRC2wait(0x1AE);
            goto l2;
        l2b:
            if (chkRC2wait() != 0) return 0;
        l2:
            if (!(I_STAT_Pad->pending & 0x80)) goto l2b;
            *(unsigned char *)&PadSio0Base->data = 1;
            setRC2wait(0x3C);
            if (_padClrIntSio0() == 0) return 0;
            _padWaitRXready();
            PadSio0Base->data;
            return 0;
        }
        I_STAT_Pad->pending = ~0x80U;
    }
    if (a0->b50 != 0 && a0->command37 != 0) return 0;
    return 1;
}
