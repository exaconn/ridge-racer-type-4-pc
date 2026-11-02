/* CC1_FLAGS: -fno-schedule-insns */
extern int D_800ABEAC;
extern char D_800F4DB8[];
extern int D_800A099C[][3];
extern int D_800A0904[];
extern int func_800529D8();
extern int func_80052B48();
extern int func_80096434();
extern int func_80096474();
#define W(p,o) (*(int*)((char*)(p)+(o)))
void func_800528A8(a0, a1)
int a0, a1;
{
    register char *g = D_800F4DB8;
    register char *s2;
    register int s1;
    unsigned s0; 
    int a;
    int h;
    unsigned t2;
    int v2, tt;
    register unsigned m asm("$3"); /* ASM_BARRIER: mask constant must live in $v1 */
    int *v; 
    s2 = g + a0 * 8;
    a = W(s2, 0x2638);
    if (a != -1) {
        m = 0xFFF0000;
        /* ASM_BARRIER: keep lui/ori of the 0x0FFFFFFF mask as separate insns for scheduling */
        __asm__("" : "=r"(m) : "0"(m));
        s1 = a1 * 8;
        t2 = s1 + a;
        m |= 0xFFFF;
        s0 = *(unsigned *)t2;
        h = s0 >> 28;
        s0 &= m;
        s0 += W(s2, 0x2634);
        func_800529D8(s0, h, 0);
        W(g, 0x2690) = (*(int *)(s1 + W(s2, 0x2638) + 4) - 2) * 8;
        func_80096434(g + 0x26D0, 0x1C);
        { int b = (int)D_800A099C; v = (int *)(b + a1 * 12); }
        W(g, 0x26EC) = 0;
        W(g, 0x26F0) = 0;
        W(g, 0x26E0) = v[0];
        W(g, 0x26F4) = v[1];
        tt = W(g, 0x2690);
        v2 = v[2];
        W(g, 0x26A8) = 0;
        W(g, 0x2690) = tt + s0;
        W(g, 0x26E8) = v2;
        W(g, 0x26C0) = D_800A0904[a1];
        func_80052B48(0);
    }
    func_80096474(D_800ABEAC);
}
