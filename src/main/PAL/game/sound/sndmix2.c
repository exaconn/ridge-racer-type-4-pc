/* CC1_FLAGS: -fno-strength-reduce */
extern char D_800AD528[];
extern char D_800F4DB8[];
extern int func_8004FE7C();
extern int func_800857A4();
extern int func_80085AEC();
void func_80051378() {
    int i;
    register char *s2 asm("$18"); /* ASM_BARRIER: pin */
    char *s4;
    char *s5;
    int s0, s1, a, b;
    i = 0;
    s5 = D_800AD528;
    s4 = D_800F4DB8;
    s2 = s4;
    for (i = 0; i < 2; i++, s2 += 0x14) {
        if (*(int *)(s2 + 0x1994) == 0) continue;
        s0 = *(short *)(s2 + 0x19A4);
        s1 = *(short *)(s2 + 0x19A6);
        if (*(int *)(s4 + 4) != 0) {
            s1 = 0;
            s0 = 0;
        }
        a = *(short *)(s4 + 0x18);
        s1 = (s1 * a) / 128;
        s0 = (s0 * a) / 128;
        b = *(unsigned char *)(s5 + 0x31);
        s1 = (s1 * b) / 16;
        s0 = (s0 * b) / 16;
        if (s0 + s1 > 0) {
            func_8004FE7C(*(int *)(s2 + 0x19A0), s0, s1);
            if (*(int *)(s2 + 0x1998) == 0) {
                func_800857A4(*(short *)(s2 + 0x19A0), *(short *)(s2 + 0x199C), 7, 0, 0x3C, 0, (short)s0, (short)s1);
                *(int *)(s2 + 0x1998) = 1;
            }
        } else if (*(int *)(s2 + 0x1998) != 0) {
            func_80085AEC(*(short *)(s2 + 0x19A0));
            *(int *)(s2 + 0x1998) = 0;
        }
    }
}
