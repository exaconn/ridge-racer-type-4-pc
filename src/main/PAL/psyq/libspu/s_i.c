/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_tick_control.h"
#include "psyq/libspu/s_i.h"
#include "rr/spu_common.h"

/* Anchors originate in the complete timer object. Signed native-width
 * offset differences keep every scalar access within that object. */
#define FIELD(p,type,member) (*(type *)((p)+(RR_FIELD_OFFSET(SpuTickState20,member)-RR_FIELD_OFFSET(SpuTickState20,flags))))
#define FLAG(p,member) (*(s8 *)((p)+RR_FIELD_OFFSET(SpuTickFlags4,member)))

extern void (*D_800A4D44[])();
extern s32 D_800A4D4C;
extern void (*D_800A4D40)(void);

void SsSetMVol(short a, short b);
void SsStart2(void);

void SsSetMVol(short a, short b) {
    SpuCommonAttributes28 s;
    s.mask = 3; s.master.left = a * 0x81; s.master.right = b * 0x81;
    SpuSetCommonAttr(&s);
}

void _SsStart(s32 direct)
{
    s32 wait=999;
    u32 counter;
    s32 period;
    SpuTickCallback callback;
    s32 interrupt;
    u8 *setup;
    u8 *state;
    do {wait--;} while(wait>=0);
    counter=0xF2000002;
    period=0x44E8;
    setup=(u8 *)&_snd_seq_tick_env+RR_FIELD_OFFSET(SpuTickState20,flags);
    FLAG(setup,interrupt)=6;
    FLAG(setup,direct)=0;
    FLAG(setup,divided)=0;
    FIELD(setup,SpuTickCallback,previous)=0;
    switch(FIELD(setup,s32,selector)) {
    case 0:FLAG(setup,interrupt)=127;return;
    case 5:
        FLAG(setup,interrupt)=0;
        if(direct==0) FLAG(setup,direct)=1;
        else {counter=0xF2000003;period=1;}
        break;
    case 2:break;
    case 3:period=0x89D0;break;
    default:
        {
        register u8 *active=(u8 *)&_snd_seq_tick_env+RR_FIELD_OFFSET(SpuTickState20,active);
        if(*(s32 *)active!=0) return;
        {
            register s32 frequency=((s32 *)active)[-1];
            register u8 *parent=active-4;
            if(frequency<70) {
                register s32 quotient=0x204CC0/frequency;
                parent[RR_FIELD_OFFSET(SpuTickState20,flags)+1]++;
                /* ASM_BARRIER: Publish the divided flag before assigning the period. */
                period=quotient;
            } else { register s32 quotient __asm__("$2")=0x409980/frequency;  period=quotient; }
        }
        }
    }
    state=(u8 *)&_snd_seq_tick_env+RR_FIELD_OFFSET(SpuTickState20,flags);
    if(FLAG(state,direct)) {
        EnterCriticalSection();
        VSyncCallback(FIELD(state,SpuTickCallback,callback));
    } else {
        EnterCriticalSection();
        ResetRCnt(counter);
        SetRCnt(counter,(u16)period,0x1000);
        interrupt=FLAG(state,interrupt);
        if(interrupt==0) {
            interrupt=0;
            FIELD(state,SpuTickCallback,previous)=InterruptCallback(interrupt,0);
            interrupt=FLAG(state,interrupt);
            callback=_SsTickCallback;
        } else {
            callback=_SsTickCallbackDivided;
            if(FLAG(state,divided)==0) callback=FIELD(state,SpuTickCallback,callback);
        }
        InterruptCallback(interrupt,callback);
    }
    ExitCriticalSection();
}

void SsStart(void) { _SsStart(1); }

void SsStart2(void) { _SsStart(0); }

void _SsTickCallback(void) {
 void (**p)() = D_800A4D44;
 if (p[0] != 0) p[0]();
 p[-1]();
}

void _SsTickCallbackDivided(void) {
    if (D_800A4D4C == 0) {
        u8 *page;
        s32 one = 1;
        /* PAL mapping: page + 0x4D4C is &D_800A4D4C. */
        page = (u8 *)0x800A0000U;
        *(s32 *)(page + 0x4D4C) = one;
    } else {
        u8 *page;
        void (*callback)(void) = D_800A4D40;
        /* PAL mapping: page + 0x4D4C is &D_800A4D4C. */
        page = (u8 *)0x800A0000U;
        *(s32 *)(page + 0x4D4C) = 0;
        callback();
    }
}
