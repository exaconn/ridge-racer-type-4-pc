#include "rr/frame_loop.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A26 (*(s16 (*))&D_800F3A26)

#define D_800F4C6A (*(s16 (*))&D_800F4C6A)

#undef D_800F3A26

#undef D_800F4C6A

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(int (*))&D_800ACC14)

#define D_800F4C6A (*(short (*))&D_800F4C6A)

#define D_800F3A26 (*(short (*))&D_800F3A26)

#undef D_800ACC14

#undef D_800F4C6A

#undef D_800F3A26

extern void func_8007D710(void),func_8008B364(void),func_80096E54(s32),func_8001E988(void);

extern s32 func_8008BDE4(s32),func_800128B8(void);

extern void func_80092CB8(s32),func_8004FD80(void),func_80092D50(s32);

extern u32 *func_8009323C(u32 *,s32);

extern s32 func_8008B154(s32);

extern void *func_80093580(void *), *func_800933B4(void *);

extern void func_80093344(void *),func_8004A6C4(void);

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
