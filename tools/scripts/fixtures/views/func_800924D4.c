/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

extern short D_800AB260[];

/* ratan_tbl */
/* PsyQ libgte ratan2(y, x) */
long func_800924D4(long y, long x) {
    long v, r, ret, c;
    int s1 = 0, s2 = 0;

    if (x < 0) {
        s1 = 1;
        x = -x;
    }
    if (y < 0) {
        s2 = 1;
        y = -y;
    }
    c = y < x;
    if (x == 0) {
        /* ASM_BARRIER: hide x == 0 from cse so the compare is recomputed */
        __asm__("" : "=r"(x) : "0"(x));
        ret = 0;
        if (y == 0) goto out;
        c = y < x;
    }
    if (c) {
        if (y & 0x7FE00000) {
            v = y / (x >> 10);
        } else {
            v = (y << 10) / x;
        }
        r = D_800AB260[v];
    } else {
        if (x & 0x7FE00000) {
            v = x / (y >> 10);
        } else {
            v = (x << 10) / y;
        }
        r = 1024 - D_800AB260[v];
    }
    if (s1) {
        r = 2048 - r;
    }
    if (s2) {
        r = -r;
    }
    ret = r;
out:
    return ret;
}
