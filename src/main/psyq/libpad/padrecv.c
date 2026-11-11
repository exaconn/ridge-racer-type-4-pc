#include "rr/channels.h"
#include "rr/counters.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
#include "rr_asm.h"
#include "psyq/libpad/padpoll.h"
#include "psyq/libpad/padtm22.h"
#include "psyq/libpad/padrecv.h"

int _padSioRW(PadChannel *a0, int a1);

void _padSioStageStep(PadChannel *channel) {
    s32 *stage=&PadRecvStage;
    ChannelStageCallback callback=PadRecvStageTable[*stage];
    s32 result,value;
    *stage+=1;
    result=callback(channel);
    if(result<0)goto error;
    value=PadRecvStage;
    if(value) {
        if(value==3 && channel->packet[0]==0x80)goto reload;
        setRC2wait(60);
        if(!_padClrIntSio0())PadAdvanceChannelFn(-3);
reload:
        value=PadRecvStage;
        RR_NOP_DEP_ON(value);
    }
    if(value>=5) {
        s32 decremented=value-1;
        {
            s32 *page=(s32 *)0x800B0000U;
            page[-0x4354/4]=decremented;
        }
    }
    goto out;
error:
    PadAdvanceChannelFn(result);
out:;
}

int _padSioRW(PadChannel *a0, int a1) {
    int s1;
    int s2;

    if (a1 < 0) {
        unsigned char tmp = PadSio0Base->data;
        a0->b44 = 0xFF;
        a0->b45 = 1;
        *a0->buffer40 = ~a1;
        s2 = tmp;
        while (!(PadSio0Base->status & 1))
            ;
        while (chkRC2wait() == 0)
            ;
        PadSio0Base->data = ~a1;
    } else {
        {
            int v0 = *a0->packet;
            int v1;
            v1 = 8;
            v0 >>= 4;
            s1 = 0x88;
            if (v0 == v1) {
                if (a0->b44 >= 9) {
                    s1 = 0x22;
                }
            }
        }
        RC2WaitLimit = 0x1AE;
        PadRcnt2Start = ((RootCounterRegisters *)0x1F801120)->count;
        while (!(PadSio0Base->status & 2))
            ;
        s2 = PadSio0Base->data;
        PadSio0Base->baud = s1;
        while (!(I_STAT_Pad->pending & 0x80)) {
            if (chkRC2wait() != 0) {
                return -0x14;
            }
        }
        PadSio0Base->data = a1;
        if (s1 == 0x22) {
            I_STAT_Pad->pending = ~0x80U;
            PadSio0Base->control |= 0x10;
        }
        a0->b45++;
        a0->packet[a0->b44] = s2;
        a0->b44++;
    }
    return s2;
}
