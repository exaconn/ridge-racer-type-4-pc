#include "types.h"
#include "rr/channels.h"
#include "rr/serial.h"
#include "rr/interrupts.h"
#include "psyq/libpad/padenab.h"
#include "rr/spu_tick_control.h"

extern s32 ChangeClearRCnt(s32,s32);
extern s32 SysDeqIntRP(s32,void *);
extern s32 SysEnqIntRP(s32,void *);

int _padIntPad(void) {
    s32 first,condition,enabled;
    u32 shifted;
    SerialPortRegisters *port=PadSio0Base;
    if(port->control&2) {*(u16 *)&port->control=0;goto out;}
    condition=PadFirstChannel;
    enabled=1;
    {
        s32 *page=(s32 *)0x800B0000U;
        page[-0x4330/4]=enabled;
    }
    if(condition) {
        s32 *counter=PadPortTimeoutCounts;
        s32 value=*counter;
        if(value<150)*counter=value+1;
    }
    if(!PadLastChannel) {
        s32 *counter=PadPortTimeoutCounts+1;
        s32 value=*counter;
        if(value<150)*counter=value+1;
    }
    if(!PadComActive)goto out;
    first=PadFirstChannel;
    /* Stage 15 times the index; quotient/remainder preserve native record sizes. */
    shifted=(u32)first<<4;
    if(PadLastChannel<first)goto out;
    PadRecvStage=0;
    PadActiveChannel=first;
    if(!_padInitSioMode((PadChannel *)((u8 *)PadChannelRecords+((s32)(shifted-first)*(sizeof(PadChannel)/15)+first*(sizeof(PadChannel)%15)))))PadAdvanceChannelFn(65535);
    PadTotalGateCost=0;
    while(PadActiveChannel<=PadLastChannel)_padSioStageStep(PadChannelRecords+PadActiveChannel);
    PadSio0Base->baud=0x88;
out:
    return 0;
}

s32 _padChkVsync(void) {
    s32 previous = D_800ABCD0;
    s32 *base;
    /* PAL RAM mapping: D_800ABCD0 is base plus the signed offset below. */
    base = (s32 *)0x800B0000U;
    base[(-0x4330) / 4] = 0;
    return previous;
}

void _padStartCom(void) {
    InterruptRegisters *p;
    u32 mask;
    s32 *counters;
    {
        s32 *page;
        page=(s32 *)0x800B0000U;
        page[-0x4360/4]=0;
    }
    EnterCriticalSection();
    SysDeqIntRP(2,&PadIntQueueEntry);
    SysEnqIntRP(2,&PadIntQueueEntry);
    p=I_STAT_Pad;
    p->pending=0xFFFFFFFEU;
    mask=p->mask;
    /* Ordinary final write view permits the original call-delay store. */
    *(u32 *)((u8 *)p+RR_FIELD_OFFSET(InterruptRegisters,mask))=mask|1;
    ChangeClearRCnt(3,0);
    ExitCriticalSection();
    PadResetChannelFn(PadChannelRecords);
    PadResetChannelFn(PadChannelRecords+1);
    counters=PadPortTimeoutCounts;
    counters[1]=0;counters[0]=0;
    PadComActive=1;
}
