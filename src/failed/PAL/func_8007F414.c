extern int D_800A47D8;
extern int D_800A47FC;
extern int D_800A4800;
extern unsigned int D_800A4804;
extern int D_800A4808;
unsigned int func_8007F414(a0, a1) int a0; unsigned int a1; {
    /* ASM_BARRIER: pins reproduce the original register allocation (a2=a0 copy, a3 shifted value, v1 masked copy, v0 result) */
    register int a2 __asm__("$6");
    register unsigned int a3 __asm__("$7");
    register unsigned int r __asm__("$2");
    register unsigned int c __asm__("$3");
    a2 = a0;
    if (D_800A47FC != 0) {
        if ((a1 % D_800A4804) != 0) {
            a1 = (a1 + D_800A4804) & ~D_800A4808;
        }
    }
    a3 = a1 >> D_800A4800;
    c = a3;
    if (a2 == -2) { r = a1; goto out; }
    r = a1;
    if (a2 == -1) { r = c & 0xFFFF; goto out; }
    *(short *)(a2 * 2 + D_800A47D8) = a3;
out:
    return r;
}
