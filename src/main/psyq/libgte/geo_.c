#include "types.h"
#include "rr/trig.h"
#include "psyq/libcd/dsstrm32.h"

extern const char DmaStatusErrorFmt[];
extern volatile s32 *D_800A6888, *DICR;
extern void DebugPrintStub(const char *, ...);

void mem2mem(int *d, int *s, unsigned n) {
    unsigned i;
    for (i = 0; i < n; i++) *d++ = *s++;
}

void dma_execute(s32 ch, s32 madr, s32 bcr_hi, s32 bcr_lo, s32 chcr, u8 flag_in) {
    volatile s32 dummy;
    s32 i;
    volatile s32 *p;
    s32 cond;
    s32 bit24;
    s32 flag = flag_in;
    s32 shift;

    i = 0;
    p = (void *)(ch << 4);
    bit24 = 0x01000000;
    if ((cond = p[0x1F801088 / 4] & bit24)) {
        shift = 0x10000;
        do {
            if (i == shift) {
                DebugPrintStub(DmaStatusErrorFmt, p[0x1F801088 / 4]);
                goto after_loop;
            }
            i++;
        } while ((cond = p[0x1F801088 / 4] & bit24));
    }
after_loop:
    {
        s32 mask = 1;
        u8 *dicr;
        if (flag == mask) {
            dicr = (u8 *)DICR;
            i = dicr[2];
            mask = i | (mask << ch);
        } else {
            dicr = (u8 *)DICR;
            i = dicr[2];
            mask = i & ~(mask << ch);
        }
        dicr[2] = mask;
    }
    dummy = *DICR;
    {
        s32 bcr;
        s32 *d88;
        shift = ch << 2;
        shift += 3;
        bit24 = 1;
        bit24 <<= shift;
        p = (void *)0x1F801080;
        bcr = ch << 4;
        p = (void *)(bcr + (s32)p);
        d88 = (s32 *)D_800A6888;
        bcr = (bcr_hi << 16) | bcr_lo;
        shift = *d88;
        shift |= bit24;
        *d88 = shift;
        *p = madr;
        p++;
        *p = bcr;
    }
    p++;
    while ((*CD_REG0_STRM & 0x40) == 0) {
    }
    *p = chcr;
    dummy = *p;
}

s32 rsin(s32 a) {
    int r;
    if (a >= 0) {
        r = sin_1(a & 0xFFF);
    } else {
        r = -sin_1((0U - a) & 0xFFFU);
    }
    return r;
}

s32 sin_1(s32 a) {
    if (a < 0x801) {
        if (a < 0x401) return SinTable[a];
        return SinTable[0x800 - a];
    }
    if (a < 0xC01) return -D_800A58C0[a];
    return -SinTable[0x1000 - a];
}
