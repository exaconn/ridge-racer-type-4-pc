extern int D_800F2EB4;
extern unsigned char D_800F4C38[];
extern unsigned int *D_8010E690;

void func_8006E8F8() {
    int *t1;
    int v;
    int t0;
    unsigned int i;
    unsigned int t2;
    int off;
    unsigned char *pb;

    t1 = (int *)D_8010E690;
    t2 = *t1;
    off = t2 << 2;
    /* ASM_BARRIER: prevent fold from reassociating (t2 << 2) + 4 with pointer addition */
    __asm__("" : "+r"(off));
    t1 = (int *)((char *)t1 + (off + 4));
    for (t0 = 0; t0 < D_800F4C38[0]; t0++) {
        v = D_800F2EB4 + (pb = D_800F4C38 + t0)[1] * 0x50;
        for (i = 0; i < *D_8010E690; i++) {
            if (v == D_8010E690[i + 1]) {
                break;
            }
        }
        if (i == *D_8010E690) {
            *t1 = v;
            t1++;
            t2++;
        }
    }
    *D_8010E690 = t2;
}
