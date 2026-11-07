/* GCC_VERSION: 2.7.2 */
typedef struct { int mask; int pad[3]; short l0, r0; int pad2[2]; short l1, r1; int pad3[2]; } w3l0S;
extern void SpuSetCommonAttr(w3l0S *, int);
int SsSetSerialVol(signed char a0, int a1, int a2) {
    w3l0S s;
    short x = a1, y = a2;
    if (a0 == 0) {
        s.mask = 0xC0;
        if (x >= 0x80) x = 0x7F;
        if (y >= 0x80) y = 0x7F;
        s.l0 = x * 0x102;
        s.r0 = y * 0x102;
    }
    if (a0 == 1) {
        s.mask = 0xC00;
        if (x >= 0x80) x = 0x7F;
        if (y >= 0x80) y = 0x7F;
        s.l1 = x * 0x102;
        s.r1 = y * 0x102;
    }
    SpuSetCommonAttr(&s, a1);
}
