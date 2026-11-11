/* RODATA 0x800120d8 0x14 */
#include "rr/channels.h"
#include "psyq/libpad/padenab.h"
#include "psyq/libpad/padopen.h"
#include "psyq/libpad/padpatch.h"
#include "psyq/libpad/padstat2.h"
#include "psyq/libpad/padctl.h"
#include "rr/channel_init.h"

void PadChkVsync();
void PadStopCom();
int PadChkMtap();
int PadInfoComb();
void PadSetMainMode(s32 selector, s32 first, s32 second);

void PadChkVsync() { _padChkVsync(); }

void PadStartCom() { _padStartCom(); }


void PadStopCom() { _padStopCom(); }

int PadChkMtap(a0) int a0; {
    if (PadMultitapEnabled) {
        int i = a0 >> 4;
        return PadChannelRecords[i].responseE8 == 8;
    }
    return 0;
}

int PadGetState(s32 selector) {
    PadChannel *p = PadPortToChannelFn(selector);
    int a;
    if (p->command37 == 0 && p->previous38 == 0) {
        if (p == p->owner10 || p->flag39 == 0) {
            if (*p->input30 == 0) return p->state49;
        }
    }
    a = p->state49;
    switch (a) {
    case 3: return 1;
    case 2: return 1;
    case 6: return 4;
    }
    return p->state49;
}

int PadInfoMode(a0, a1, a2) int a0, a1, a2; {
    PadChannel *p = PadPortToChannelFn(a0);
    switch (a1) {
    case 1: return p->responseE8;
    case 2: return p->pending;
    case 3: return p->argumentE4;
    case 4:
        if (a2 < 0) return p->modeCountE3;
        if (a2 < p->modeCountE3) return p->modes00[a2];
        return 0;
    case 100: return p->type4C;
    }
    return 0;
}

int PadInfoAct(a0, a1, a2) int a0, a1, a2; {
    PadChannel *p = PadPortToChannelFn(a0);
    unsigned char *q;
    if (a1 < 0) return p->entry_countE9;
    if (a1 < p->entry_countE9) {
        q = (u8 *)&p->entries04[a1];
        switch (a2) {
        case 1: return q[0];
        case 2: return q[1];
        case 3: return q[2];
        case 4: return q[3];
        case 5: return q[4];
        }
    }
    return 0;
}

int PadInfoComb(a0, a1, a2) int a0, a1, a2; {
    PadChannel *s = PadPortToChannelFn(a0);
    ChannelCombEntry8 *e;
    if (a1 < 0) return s->combCountEA;
    if (a1 >= s->combCountEA) return 0;
    e = s->combs08 + a1;
    if (a2 < 0) return e->count00;
    if (a2 >= e->count00) return 0;
    return e->buttons04[a2];
}

s32 PadSetActAlign(s32 selector, u8 *layout) { return _padSetActAlign(PadPortToChannelFn(selector), layout); }

void PadSetMainMode(s32 selector, s32 first, s32 second) {
    _padSetMainMode(PadPortToChannelFn(selector), first & 0xFF, second & 0xFF);
}

void PadSetAct(s32 selector, u8 *outputs, s32 count) { _padSetAct(PadPortToChannelFn(selector), outputs, count); }
