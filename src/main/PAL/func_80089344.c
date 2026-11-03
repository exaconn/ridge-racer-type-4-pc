/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
extern short func_80088608();
extern char D_8010F916[], D_8010FE18[], D_8010FE3C[], D_8010F918[], D_8010F91C[], D_8010F91E[], D_8010F920[];
extern int D_801100F8;
extern signed char D_80110107;
extern char D_8011010C[];

int func_80089344(short a0, short a1, short a2, short a3, int a4) {
    short t0;
    int a;
    unsigned short s;
    int e;
    unsigned short idx;
    t0 = a4 - 0x40;
    a = a0 * 0x36;
    if (*(short *)(D_8010F918 + a) == a1) {
        if (*(short *)(D_8010F920 + a) == a2) {
            if (*(short *)(D_8010F91C + a) == a3) {
                int d1, d2, d3, d4, d5, d6, d7, d8, d9;
                short ret;
                int v0 = D_80110107;
                /* ASM_BARRIER: force load order of D_80110107, D_8010F91E, and D_8010F916 */
                __asm__ volatile("" : "+r"(v0));
                idx = *(unsigned short *)(D_8010F91E + a);
                /* ASM_BARRIER: force load of D_8010F916 before shift/add into idx */
                __asm__ volatile("" : "+r"(idx));
                s = *(unsigned short *)(D_8010F916 + a);
                /* ASM_BARRIER: keep s load scheduled before shift/add */
                __asm__ volatile("" : "+r"(s), "+r"(v0), "+r"(idx));
                idx += v0 * 16;
                if (t0 > 0) {
                    int x1 = t0 * *(unsigned char *)((idx << 5) + D_801100F8 + 0xD);
                    int sign;
                    int q;
                    /* ASM_BARRIER: reserve $t0 across division so mfhi conflicts with $t0 */
                    register int r8 __asm__("$8");
                    /* ASM_BARRIER: start $t0 live range before sign shift */
                    __asm__ volatile("" : "=r"(r8));
                    sign = x1 >> 31;
                    q = x1 / 63;
                    /* ASM_BARRIER: keep sign and $t0 live across division so sra gets $v1 and mfhi gets $t2 */
                    __asm__ volatile("" : : "r"(sign), "r"(r8));
                    s += q;
                    e = (x1 - q * 63) * 2;
                } else if (t0 < 0) {
                    int x2 = t0 * *(unsigned char *)((idx << 5) + D_801100F8 + 0xC);
                    s = s + (x2 / 64) - 1;
                    e = ((x2 - ((x2 / 64) << 6)) * 2) + 0x7F;
                } else {
                    e = 0;
                }
                *(unsigned char *)D_8011010C = *(unsigned char *)(D_8010F91E + a0 * 0x36);
                *(short *)(D_8011010C + 0xC) = a0;
                ret = func_80088608(s, e & 0xFFFF);
                *(short *)(D_8010FE3C + a0 * 16) = ret;
                /* ASM_BARRIER: prevent scheduling D_8010FE18 load before D_8010FE3C store */
                __asm__ volatile("");
                *(unsigned char *)(D_8010FE18 + a0) |= 4;
                /* ASM_BARRIER: keep ret ($v0) live across D_8010FE18 update and allocate d9 to $t2 so mfhi picks $t2 */
                __asm__ volatile("" : "=r"(d1), "=r"(d2), "=r"(d3), "=r"(d4), "=r"(d5), "=r"(d6), "=r"(d7), "=r"(d8), "=r"(d9) : "r"(ret));
                /* ASM_BARRIER: end live range of d1..d9 */
                __asm__ volatile("" : : "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d5), "r"(d6), "r"(d7), "r"(d8), "r"(d9));
                return 1;
            }
        }
    }
    return 0;
}
