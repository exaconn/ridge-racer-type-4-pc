/* RODATA 0x800102cc 0x24 */
#include "types.h"
#include "m2c_macros.h"
#include "rr/frame_loop.h"
#include "game/data/cdfile2.h"
#include "game/system/frmenv.h"
#include "game/system/pad.h"
#include "game/effect/spfade.h"
#include "psyq/libgpu/gpuenv.h"
#include "game/system/frmtask.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/sound/sndslot.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/course_choices.h"
#include "rr/menu_background.h"
#include "rr/driver_control.h"
#include "rr/overlay668_playback_step.h"
#include "rr/overlay668_stream.h"

extern s16 D_800AC5D0;
extern s32 D_8010BF84;
extern short *D_8010BF80;
extern int D_800F3A60;
extern int func_80052800();
extern int func_800528A8();
extern int func_80052A30();
extern int func_800436E8();
extern int func_8002AF8C();
extern int func_800207D4();
extern int func_80052B8C();
extern int func_8001BAF8();
extern int func_8001E1F0();
extern void *func_80093580(void *);

void func_8001E344(void);
void func_8001E390(void);

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A26 (*(s16 (*))&D_800F3A26)
#define D_800F4C6A (*(s16 (*))&D_800F4C6A)

void func_8001E344(void) {
    D_8010BF84 = 0;
    D_800AC5D0 = -1;
    D_800F4C6A = 0;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

#undef D_800F3A26
#undef D_800F4C6A

void func_8001E378(void) {
    D_8010BF84 = 0;
    D_800AC5D0 = -1;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(int (*))&D_800ACC14)
#define D_800F4C6A (*(short (*))&D_800F4C6A)
#define D_800F3A26 (*(short (*))&D_800F3A26)

void func_8001E390(void) {
    short *p;
    short *q;
    short *r;
    volatile int *g;
    int *s_;
    int *d_;
    int v;

    switch (D_8010BF84) {
    case 0:
        v = func_8001223C(0, 0x15);
        if (func_8001292C(v, D_800ACC18)) {
            D_8010BF84 = 1;
        }
        return;
    case 1:
        if (func_80012A58()) {
            D_8010BF84 = 2;
        }
        return;
    case 2:
        func_80052800();
        func_800528A8(0, 0x24);
        func_80052A30(0);
        func_800436E8(D_800ACC18);
        D_8010BF84 = 7;
        q = (short *)D_800ACC18;
        D_8010BF80 = q;
        D_800ACC18 = (int)q + 0x10;
        q[1] = 0;
        q[2] = 0;
        q[3] = 0;
        q[4] = 0;
        q[5] = 0;
        q[6] = 8;
        q[7] = 0;
        q[0] = 0;
    case 7:
        r = D_8010BF80;
        if (r[0] < 0x40) {
            if (r[0] == 8) {
                r[4] = 4;
            } else if (r[0] == 0x20) {
                r[5] = 4;
            }
            goto common;
        }
        D_8010BF84 = 8;
    case 8:
        func_8002AF8C(1, 1, 0);
        if (D_800AC5D0 > 0) {
            D_800AC5D0--;
            func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
            if (D_800AC5D0 == 0) {
                D_800F4C6A = 1;
                D_800F3A26 = 3;
                D_800F4D8A = 0;
            }
        } else if (D_800F3A60) {
            D_800AC5D0 = 0x40;
            func_80052B8C(0, 0x40);
        }
    case 3: case 4: case 5: case 6:
    default:
    common:
        p = D_8010BF80;
        func_8001B754(p + 1, p + 4, 0x80);
        p = D_8010BF80;
        func_8001B754(p + 2, p + 5, 0x80);
        p = D_8010BF80;
        func_8001B754(p + 3, p + 6, 0x80);
        func_8001BAF8(D_8010BF80[1], 0x2F);
        func_8001BAF8(D_8010BF80[2], 0x30);
        p = D_8010BF80;
        func_8001E1F0(p[3]);
        g = (int *)0x1F800000;
        *(int *)(g[0] + 4) = 0xE100022A;
        s_ = (int *)g[1];
        d_ = (int *)g[0];
        *d_ = *s_;
        *(char *)(g[0] + 3) = 1;
        *(int *)g[1] = g[0];
        *(char *)(g[1] + 3) = 0;
        *(int *)0x1F800000 += 8;
        func_8002A344(0x1000C);
        D_8010BF80[0]++;
    }
}

#undef D_800ACC14
#undef D_800F4C6A
#undef D_800F3A26

void func_8001E684(void) {
    s32 threshold;
    register u8 *bank_page;
    u32 bank;
    u8 *frame;
    func_8007D710();
    func_8008BDE4(1);
    func_8008B364();
    bank_page=(u8 *)0x800F0000U;
    func_80096E54(2);
    func_8001E988();
    D_800AC5DC=128;
    func_80092CB8(0);
    D_800F2DE4=0;
    D_800F4C6A=1;
    D_800F3A26=1;
    D_800F4D8A=0;
    D_800ABEAC=0;
    for (;;) {
        {
            /* ASM_BARRIER: prepare the real ordering-table length before its address. */
            register s32 length asm("$5")=0x2C0;
            /* ASM_BARRIER: materialize the frame arena in V1 before indexing it. */
            register RrFrameStorage *frames asm("$3");
            register u8 *frame_page;
            register u8 **scratch;
            register u32 *ordering;
            /* ASM_BARRIER: keep the arena address after the length preparation. */
            __asm__("" : "+r"(length) : : "$3");
            frame_page=(u8 *)0x800B0000U;
            bank=D_800ABEAC;
            /* ASM_BARRIER: read the counter before completing the arena address. */
            __asm__("" : "+r"(frame_page), "+r"(bank));
            frames=(RrFrameStorage *)(frame_page-0x2510);
            bank &=1;
            frame=(u8 *)((unsigned long)bank*sizeof(RrFrameStorage)+(unsigned long)frames);
            ordering=(u32 *)(frame+0x70);
            D_800ACC14=frame;
            *(s32 *)(bank_page+0x4BF0)=bank;
            scratch=(u8 **)0x1F800000;
            scratch[1]=(u8 *)ordering;
            scratch[0]=frame+0x1670;
            func_8009323C(ordering,length);
        }
        func_8009323C((u32 *)(D_800ACC14+0xB70),0x2C0);
        func_800128B8();
        func_8004FD80();
        D_8009E9F0[D_800F4C6A][D_800F3A26]();
        func_80092D50(0);
        threshold=D_800AC5DC;
        do {} while (func_8008B154(1)<threshold);
        func_8008B154(0);
        func_80093580(D_800ACC14+0x5C);
        func_800933B4(D_800ACC14);
        func_80093344(D_800ACC14+0xB6C);
        if(D_800ACBF8) func_80093344(D_800ACC14+0x166C);
        func_8004A6C4();
        D_800ABEAC++;
    }
}
