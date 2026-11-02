extern int func_8006F1EC();
extern int func_8006F260();
extern short *D_8010E790;
extern unsigned short D_800AD5B8;

void func_80071348(void) {
    int a, b;
    short *p;
    short v;
    int base;
    register short *g __asm__("$17");
    a = func_8006F1EC(0x1F800008, 0);
    b = func_8006F260(0x1F800008);
    base = (int)D_8010E790;
    p = (short *)(b * 2 + ((a << 4) + base));
    if (*p >= 0) {
        v = *(unsigned short *)p;
    } else {
        v = D_800AD5B8;
    }
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__("" : "=r"(g) : "0"(0x1F800000));
    g[0x2E] = v;
}
