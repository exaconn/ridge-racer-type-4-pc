/* GCC_VERSION: 2.7.2 */
/* RODATA 0x800121e0 0x24 */
extern volatile int D_800ABDF8;
extern int D_80114730;
extern int *D_8011471C;
extern int *D_80114720;
int func_8009DE54(a0) int a0; {
    /* ASM_BARRIER: pin the four case temporaries to the original v0/v1 so the switch index lands in v1 */
    register int *p __asm__("$3");
    register int *q __asm__("$2");
    register int v __asm__("$2");
    register int w __asm__("$3");
    int r;
    switch (D_800ABDF8) {
    case 2:
        D_80114730 = 8;
        break;
    case 3:
        if ((a0 & 0xFF) != 0x5A) {
            r = -1;
            goto out;
        }
        break;
    case 7:
        p = D_8011471C;
        v = a0 & 0xFF;
        goto st;
    case 8:
        q = D_8011471C;
        goto sh;
    case 9:
        p = D_80114720;
        v = a0 & 0xFF;
    st:
        *p = v;
        break;
    case 10:
        q = D_80114720;
    sh:
        w = *q;
        a0 &= 0xFF;
        w = w << 8;
        /* ASM_BARRIER: hide the zero low byte of w from combine so + stays addu (not or) */
        __asm__("" : "=r"(w) : "0"(w));
        w = w + a0;
        *q = w;
        break;
    }
    r = 0;
out:
    return r;
}
