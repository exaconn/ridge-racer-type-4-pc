#include "rr/track_lateral.h"
#include "rr/steering.h"

typedef struct {
    char pad0[0x38];
    short unk38;
    char pad3A[0x52];
    short unk8C;
    char pad8E[0x6];
    short unk94;
    short unk96;
    int unk98;
    int unk9C;
    int unkA0;
    char padA4[0x4];
    int unkA8;
    int unkAC;
    char padB0[0x4];
    int unkB4;
    char padB8[0x4];
    int unkBC;
    char padC0[0x8];
    RrCurve24 primaryC8;
} Sub1F4;

typedef struct {
    char pad0[0x54];
    int unk54;
    char pad58[0x98];
    int unkF0;
    char padF4[0xE0];
    short unk1D4;
    char pad1D6[0x2];
    short unk1D8;
    char pad1DA[0x1A];
    Sub1F4 sub1F4;
} Arg0;

void func_80047FBC(Arg0 *a0);
int func_80048384(int a0, int a1);

void func_80047FBC(Arg0 *a0) {
    int s0;
    int s1;
    Sub1F4 *s2 = &a0->sub1F4;
    int s3;
    int s4;
    int s5 = 0;
    int s6;
    int s7;
    int fp = 0;
    int a0_v;
    int v1;

    s3 = a0->unk1D8 * 6;
    if (s3 < 0x400) {
        s3 = 0x400;
    }
    s2->unkA8 = s3;
    s3 = 0;
    s1 = a0->unkF0;
    s7 = func_8004C454(s1, s2->unk38);
    s6 = (short)-a0->unk1D4 + 0x400;
    s6 &= 0xFFF;
    s0 = s3;
    if (s6 & 0x800) {
        s6 |= ~0xFFF;
    }
    s4 = s1;
    s1 = s4 - 1;
    if (s1 < 0) {
        s1 += D_800ABECC;
    }
    while (s3 < s2->unkA8) {
        v1 = func_8004C454(s1, s2->unk38);
        a0_v = v1 - s7;
        a0_v &= 0xFFF;
        s7 = v1;
        if (a0_v & 0x800) {
            a0_v |= ~0xFFF;
        }
        /* ASM_BARRIER: prevent CSE from substituting v1 for s7 across branch merge */
        __asm__("" : "+r"(s7));
        v1 = s7 - s6;
        v1 &= 0xFFF;
        if (v1 & 0x800) {
            v1 |= ~0xFFF;
        }
        switch (s5) {
        case 1:
            if (a0_v < 0 || v1 < 0) {
                s3 = s2->unkA8;
                fp = 1;
            }
            break;
        case -1:
            if (a0_v > 0 || v1 > 0) {
                s3 = s2->unkA8;
                fp = 1;
            }
            break;
        case 0:
            if (a0_v > 0) {
                s5 = 1;
            }
            if (a0_v < 0) {
                s5 = -1;
            }
            break;
        }
        if (!fp) {
            s4 = s1;
            s3 += D_800ABEA4[s4].segmentLength1C;
            s1 = s4 - 1;
            if (s1 < 0) {
                s1 += D_800ABECC;
            }
        }
    }
    s1 = s4 - a0->unkF0;
    s1 %= D_800ABECC;
    if (s1 > D_800ABECC / 2) {
        s1 -= D_800ABECC;
    }
    if (s1 < 0) {
        s1 = -s1;
    }
    if (s1 >= 5) {
        s0 = func_8004C454(s4, s2->unk38);
        s2->unkAC = s4;
        v1 = (short)-a0->unk1D4 + 0x400;
        v1 &= 0xFFF;
        if (v1 & 0x800) {
            v1 |= ~0xFFF;
        }
        s0 = s0 - v1;
        s0 &= 0xFFF;
        if (s0 & 0x800) {
            s0 |= ~0xFFF;
        }
        s2->unk9C = s0;
        s2->unk98 = s0 / 32;
        if (s2->unk98 < 0) {
            s2->unk98 = -s2->unk98;
        }
        if (s2->unk98 < 0x20) {
            s2->unk98 = 0x20;
        }
        if (s2->unk98 > 0x40) {
            s2->unk98 = 0x40;
        }
        s0 = a0->unk54;
        s0 &= 0xFFF;
        if (s0 & 0x800) {
            s0 |= ~0xFFF;
        }
        s2->unkA0 = s0 << 12;
        func_8004D354(&s2->primaryC8, s2->unk9C, s2->unk98, -s2->unk8C);
        /* ASM_BARRIER: extend s0 live range across call to keep s0 in callee-saved register */
        __asm__("" : : "r"(s0));
        s2->unk94 = 1;
        s2->unkBC = 0;
        s2->unk96 = 0;
        s2->unk9C <<= 12;
        s2->unkB4 = -s2->unk9C / 24;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEA4 ((char *)D_800ABEA4)

int func_80048384(int a0, int a1) {
    if (a1 > 0) {
        int n = D_800ABECC;
        char *p = D_800ABEA4;
        do {
            a0--;
            if (a0 < 0) a0 += n;
            a1 -= *(short *)(p + a0 * 60 + 0x1C);
        } while (a1 > 0);
    }
    return a0;
}

#undef D_800ABEA4
