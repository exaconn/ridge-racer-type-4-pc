/* CC1_FLAGS: -fno-schedule-insns */
typedef struct { int x0; int x4; int x8; char p0[0x198 - 0xC]; short s198, s19a, s19c, s19e; char p1[0x1E4 - 0x1A0]; unsigned short s1e4; char p2[0x224 - 0x1E6]; short s224; } W10_5A;

void func_8002C138(a0) W10_5A *a0;
{
    W10_5A *a1;
    /* ASM_BARRIER: pin copy reg */
    a1 = a0;
    if (a1->s19c != 0) {
        if (a1->s19e > 0) {
            a1->s19e = a1->s19e - 1;
            if (a1->s19e <= 0) {
                a1->s19c = 0;
                a1->s19e = 0;
                if (a1->s1e4 & 0x10) a1->s224 = 0;
            }
            {
                /* ASM_BARRIER: pin v1 to $3 for original reg order */
                register int v1 __asm__("$3");
                int a, t;
                v1 = a1->s198;
                a = a1->s19a;
                a1->x0 = a1->x0 - v1;
                a1->x8 = a1->x8 - a;
                t = v1 * 7;
                /* ASM_BARRIER: forced reg copy */
                __asm__("" : "=r"(v1) : "0"(a));
                a1->s198 = t >> 3;
                a1->s19a = (v1 * 7) >> 3;
            }
        }
    }
}
