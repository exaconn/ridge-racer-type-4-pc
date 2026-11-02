/* GCC_VERSION: 2.7.2 */
extern int func_80094B28(), func_80094B70();
extern short D_800ABAC4, D_800ABAC6;
extern volatile unsigned *D_800ABBC8;
extern unsigned D_80112788[], D_801127B0[];
struct R { unsigned short x; short p; short y; short h; };
int func_800943C4(a0, a1) struct R *a0; unsigned a1; {
    short t;
    unsigned short v;
    unsigned a2;
    t = a0->y;
    if (t >= 0) {
        v = D_800ABAC4 - 1;
        if (D_800ABAC4 - 1 >= t) v = a0->y;
    } else v = 0;
    t = a0->h;
    a0->y = v;
    if (t >= 0) {
        v = D_800ABAC6 - 1;
        if (D_800ABAC6 - 1 >= t) v = a0->h;
    } else v = 0;
    a0->h = v;
    if ((a0->x & 0x3F) == 0 && ((unsigned short)a0->y & 0x3F) == 0) {
        D_80112788[0] = 0x05FFFFFF;
        D_80112788[1] = 0xE6000000;
        D_80112788[3] = (a1 & 0xFFFFFF) | 0x02000000;
        D_80112788[2] = (*D_800ABBC8 & 0x7FF) | (((a1 >> 31) << 10) | 0xE1000000);
        D_80112788[4] = a0->x;
        D_80112788[5] = a0->y;
    } else {
        D_80112788[0] = ((unsigned)D_801127B0 & 0xFFFFFF) | 0x08000000;
        a2 = (a1 & 0xFFFFFF) | 0x60000000;
        D_80112788[4] = 0xE6000000;
        D_80112788[1] = 0xE3000000;
        D_80112788[2] = 0xE4FFFFFF;
        D_80112788[3] = 0xE5000000;
        D_80112788[6] = a2;
        D_80112788[5] = (*D_800ABBC8 & 0x7FF) | (((a1 >> 31) << 10) | 0xE1000000);
        D_80112788[7] = a0->x;
        D_801127B0[0] = 0x03FFFFFF;
        D_80112788[8] = a0->y;
        D_801127B0[1] = func_80094B70(3, D_80112788, a2, 0x03FFFFFF) | 0xE3000000;
        D_801127B0[2] = func_80094B70(4) | 0xE4000000;
        D_801127B0[3] = func_80094B70(5) | 0xE5000000;
    }
    func_80094B28(D_80112788);
    return 0;
}
