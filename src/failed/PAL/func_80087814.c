extern char *D_800A47D8;
extern int D_800A4844, D_800A486C, D_800A4870, D_800A48A4;
extern volatile unsigned short D_8010EFB0[];
void func_80087814(a0, a1) int a0; int a1; {
    unsigned lo;
    unsigned hi;
    unsigned v;
    lo = a1 & 0xFFFFFF;
    hi = lo >> 16;
    switch (a0) {
    case 1:
        if (D_800A48A4 & 1) {
            D_8010EFB0[0] = lo;
            D_8010EFB0[1] = hi;
            D_800A4870 |= 1;
            D_800A486C |= lo;
            if (D_8010EFB0[2] & lo) D_8010EFB0[2] &= ~lo;
            if (D_8010EFB0[3] & hi) D_8010EFB0[3] &= ~hi;
        } else {
            *(unsigned short *)(D_800A47D8 + 0x188) = lo;
            *(unsigned short *)(D_800A47D8 + 0x18A) = hi;
            v = D_800A4844 | lo;
            goto out;
        }
        break;
    case 0:
        if (D_800A48A4 & 1) {
            D_8010EFB0[2] = lo;
            D_8010EFB0[3] = hi;
            D_800A4870 |= 1;
            D_800A486C &= ~lo;
            if (D_8010EFB0[0] & lo) D_8010EFB0[0] &= ~lo;
            if (D_8010EFB0[1] & hi) D_8010EFB0[1] &= ~hi;
        } else {
            *(unsigned short *)(D_800A47D8 + 0x18C) = lo;
            *(unsigned short *)(D_800A47D8 + 0x18E) = hi;
            v = D_800A4844 & ~lo;
        out:
            D_800A4844 = v;
        }
        break;
    }
}
