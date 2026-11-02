typedef struct { unsigned short s0, s2, s4, s6, s8; short sA, sC, sE, s10; } S;
void func_80080F60(a0, a1, a2) S *a0; short *a1; short *a2; {
    S *p;
    short *d; 
    register int c __asm__("$2"); /* ASM_BARRIER: pin to the original register allocation */
    register int t __asm__("$5"); /* ASM_BARRIER: pin to the original register allocation */
    int v; 
    int e; 
    register int x __asm__("$3"); /* ASM_BARRIER: pin to the original register allocation */
    register int y; 
    register int w __asm__("$5"); /* ASM_BARRIER: pin to the original register allocation */
    int k, g;
    p = a0;
    d = a1;
    k = -0x8000;
    c = p->sC;
    g = p->s10;
    c = c != 0;
    c = -c;
    t = c & k;
    v = t;
    c = p->sA;
    c = c != 0;
    c = -c;
    e = c & k;
    if (g != 0) v = t | 0x4000;
    if (p->sE != 0) v |= 0x20;
    x = (p->s0 << 8) & 0x7F00;
    x = e | x;
    y = (p->s2 * 0x10) & 0xF0;
    x |= y;
    w = p->s8 & 0x1F;
    x |= p->s4 & 0xF;
    y = v | ((p->s6 << 6) & 0x1FC0) | w;
    *d = x;
    *a2 = y;
}
