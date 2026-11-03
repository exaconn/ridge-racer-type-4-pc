#include "types.h"

s32 func_8001292C(s32, void *);
s32 func_80012A58(void);
void func_80012A68(void *);
void func_80012AB4(void *, s32);
void func_80012C7C(void *, s32);
void func_80012E94(s32, s32, s32, s32, s32);
void func_8001301C(s32, s32);
void func_800293AC(void *, s32);
void func_80070108(void *, s32);
void func_80092D50(s32);
void func_80092FFC(s16 *, void *);

extern s32 D_8009DF98;
typedef struct {
    s32 header;
    s32 p4, p8, pC, p10, p14;
} Resource;
extern Resource *D_800ACC18;
extern void *D_800F3A08[];
extern s32 D_80109440;
extern s32 D_80109444;
extern s32 D_80109448;
extern s32 D_8010944C;
extern s32 D_80109450;
extern s32 D_80109454;
extern s32 D_80109468[];

s32 func_800135BC(void) {
    s16 rect[4];
    void *s0;
    void *s1;
    void *s2;
    void *s3;

    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(D_80109440, D_800ACC18) == 0) {
            return 0;
        }
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0) {
            return 0;
        }
        {
            s32 x;
            D_800ACC18->p4 += (s32)D_800ACC18;
            D_800ACC18->p8 += (s32)D_800ACC18;
            D_800ACC18->pC += (s32)D_800ACC18;
            D_800ACC18->p10 += (s32)D_800ACC18;
            D_800ACC18->p14 += (s32)D_800ACC18;
            s0 = (void *)D_800ACC18->pC;
            s1 = (void *)D_800ACC18->p10;
            s2 = (void *)D_800ACC18->p14;
            s3 = (void *)D_800ACC18->p8;
            func_800293AC((void *)D_800ACC18->p4, D_80109444);
            D_800F3A08[D_80109444] = s3;
            func_80070108(s0, D_80109444);
            /* ASM_BARRIER: preserve local-alloc priority of s0 ($s0) over s3 ($s3) */
            __asm__("" : : "r"(s0));
            func_80012AB4(s0, D_80109444);
            D_80109468[D_80109444] = (s32)s1;
            func_80012A68(s1);
            func_80012C7C(s1, D_80109444);
            rect[2] = 0x40;
            rect[1] = 0;
            rect[3] = 0x100;
            x = (D_80109444 << 6) + 0x180;
            rect[0] = x;
        }
        func_80092FFC(rect, s2);
        func_80092D50(0);
        D_800ACC18 = s2;
        func_8001301C(D_80109468[D_80109444], D_80109444);
        if (D_80109454 != 0) {
            func_80012E94(D_80109448, D_8010944C, D_80109450, D_80109454 - 1, D_80109444);
        }
        D_8009DF98 = 0;
        return 1;
    }
}
