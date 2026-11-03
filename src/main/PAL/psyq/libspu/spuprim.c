/* CC1_FLAGS: -mno-split-addresses */
extern int func_8007F54C(), func_8007F574();
struct H { char p[0x1A6]; volatile unsigned short f1A6; char q[2]; volatile unsigned short f1AA; };
extern struct H *D_800A47D8;
extern int *D_800A47DC, *D_800A47E0, *D_800A47E4;
extern unsigned short D_800A47F0;
extern int D_800A4800, D_800A4828, D_800A482C, D_800A4830;
int func_8007F068(int a0, ...) {
    unsigned *s0;
    unsigned v1;
    unsigned short x;
    /* ASM_BARRIER: Keep control value in $a2 across case 3 setup */
    register int val asm("$6");
    /* ASM_BARRIER: Keep temporary values in $a0 in case 2 and case 3 tail */
    register unsigned r_a0 asm("$4");
    s0 = (unsigned *)(&a0 + 1);
    switch (a0) {
    case 2:
        {
            /* ASM_BARRIER: Keep shifted transfer address in $v0 */
            register unsigned r_v0 asm("$2");
            r_a0 = *s0;
            r_v0 = r_a0 >> D_800A4800;
            D_800A47F0 = r_v0;
            D_800A47D8->f1A6 = r_v0;
        }
        break;
    case 1:
        D_800A4828 = 0;
        v1 = 0;
        while (D_800A47D8->f1A6 != D_800A47F0) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        D_800A47D8->f1AA = (D_800A47D8->f1AA & 0xFFCF) | 0x20;
        break;
    case 0:
        D_800A4828 = 1;
        v1 = 0;
        while (D_800A47D8->f1A6 != D_800A47F0) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        D_800A47D8->f1AA |= 0x30;
        break;
    case 3:
        x = (D_800A4828 == 1) ? 0x30 : 0x20;
        v1 = 0;
        while ((D_800A47D8->f1AA & 0x30) != x) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        if (D_800A4828 == 1) {
            s0++;
            func_8007F574();
            val = 0x01000000;
        } else {
            s0++;
            func_8007F54C();
            val = 0x01000000;
        }
        {
            /* ASM_BARRIER: Keep block count in $v1 */
            register unsigned r_v1 asm("$3");
            r_a0 = s0[-1];
            D_800A482C = r_a0;
            r_a0 = s0[0];
            r_v1 = r_a0 >> 6;
            /* ASM_BARRIER: Equalize dependence depth so srl schedules before andi */
            __asm__("" : "=r"(r_v1) : "0"(r_v1));
            r_v1 += (r_a0 & 0x3F) != 0;
            D_800A4830 = r_v1;
            r_a0 = D_800A482C;
            *D_800A47DC = r_a0;
        }
        *D_800A47E0 = (D_800A4830 << 16) | 0x10;
        /* ASM_BARRIER: Keep low half of 0x01000201 after *D_800A47E0 store so it fills bne delay slot */
        __asm__ volatile("" : "=r"(val) : "0"(val));
        val |= 0x201;
        if (D_800A4828 == 1) val = 0x01000200;
        *D_800A47E4 = val;
        break;
    }
    return 0;
}
