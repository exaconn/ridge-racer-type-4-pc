typedef struct { char p0[8]; int s8; char p1[0x34 - 0xC]; int w34, w38, w3c, w40; char p2[0x114 - 0x44]; int w114[3][4]; int w144[3]; short h150[4]; char p3[0x250 - 0x158]; short h250[64]; } T;
extern int func_8002C1D4();
extern int func_8002CBC0();
extern short func_8006E368();
typedef struct {
    char p0[0x10]; int w10, w14, w18; char p1[0x20 - 0x1C]; int w20, w24, w28;
    char p2[0x40 - 0x2C]; int w40, w44, w48; char p3[0x50 - 0x4C]; int w50; unsigned short h54; short h56; int w58;
    char p5[0x70 - 0x5C]; int w70, w74, w78; char p6[0x80 - 0x7C]; short h80; short h82; short h84; char p7[0x8C - 0x86];
    int w8c[13]; char p8[0xC8 - 0xC0]; int wc8, wcc, wd0;
    char p9[0xF8 - 0xD4]; unsigned short hf8[4]; unsigned short h100[4]; char pa[0x10C - 0x108]; short h10c, h10e;
    char pb[0x124 - 0x110]; short h124[16]; short h144[16]; unsigned short h164, h166, h168, h16a; int w16c;
    char pc[0x1A8 - 0x170]; unsigned short h1a8, h1aa, h1ac, h1ae;
    char pd[0x1B8 - 0x1B0]; unsigned short h1b8, h1ba; unsigned short h1bc[4]; short h1c4[4]; short h1cc[4];
} A;
#define W9C w8c[4]
#define WAC w8c[8]
#define WBC w8c[12]
void func_80028B30(a)
A *a;
{
    int *q7c = (int *)0x1F80007C;
    T *SC = (T *)0x1F800080;
    int t, t2, t3, v;
    A *p2;
    int *pw;
    SC->s8 = 0;
    do {
        t = SC->s8;
        SC->w114[t][0] = *(short *)((char *)SC + (*q7c * 8 + t * 2) + 0x250);
        if ((short)a->h100[SC->s8] != (short)a->hf8[SC->s8]) {
            if ((short)a->h1bc[SC->s8] == 0) {
                if (func_8002CBC0(a, SC->s8) != 0)
                    a->h1cc[SC->s8] = SC->w114[SC->s8][0];
            }
        }
        t2 = *(volatile int *)&SC->s8;
        p2 = (A *)((char *)a + t2 * 2);
        /* ASM_BARRIER: pin c to a0 */
        {
            register int c asm("a0") = (short)p2->h1bc[0];
            if (c > 0) {
                int lo = p2->h1c4[0] * c;
            a->w8c[t2 * 4] = p2->h1cc[0] + lo + ((c * c << 11) / 900);
            t3 = SC->s8;
            v = SC->w114[t3][0];
            if (a->w8c[t3 * 4] >= v) {
                if ((short)a->h1bc[t3] >= 2) {
                    a->w8c[t3 * 4] = v;
                    if (SC->s8 == 0) {
                        a->h1ba = a->h1bc[0];
                        func_8002C1D4(1, a);
                    }
                    a->h1bc[SC->s8] = 0;
                    a->h1b8 ^= 1 << SC->s8;
                } else goto b11;
            } else {
b11:
                a->h1bc[SC->s8]++;
            }
            SC->h150[SC->s8] = a->w8c[SC->s8 * 4] - SC->w114[SC->s8][0];
            } else {
                a->w8c[t2 * 4] = SC->w114[t2][0];
                SC->h150[SC->s8] = 0;
            }
        }
        a->h100[SC->s8] = a->hf8[SC->s8];
    } while (++SC->s8 < 4);
    a->h1a8 += func_8006E368(a->h10e, (SC->h150[0] + SC->h150[1] - (SC->h150[2] + SC->h150[3])) >> 3);
    {
        short r = func_8006E368(a->h10c, (SC->h150[0] + SC->h150[2] - (SC->h150[1] + SC->h150[3])) >> 3);
        int wac = a->w8c[8];
        int wbc = a->w8c[12];
        int m = (wac + wbc) >> 1;
        a->w14 = m;
        a->h1aa += r;
        a->w14 = ((((a->w8c[0] + a->w8c[4]) - (a->w8c[8] + a->w8c[12])) >> 1) * 0x14) / a->h10e + m;
        /* ASM_BARRIER: keep wbc live past h1aa update so local-alloc picks a2 */
        __asm__ volatile("" : : "r"(wbc));
    }
    {
        int m = (SC->w114[2][0] + SC->w114[3][0]) >> 1;
        int v, d;
        a->wcc = m;
        d = ((((SC->w114[0][0] + SC->w114[1][0]) - (SC->w114[2][0] + SC->w114[3][0])) >> 1) * 0x14) / a->h10e;
        v = a->w16c & 0xF;
        a->wcc = d + m;
        a->w16c = v;
        SC->w34 = v;
        SC->w38 = (v + 6) & 0xF;
        SC->w3c = (SC->w34 + 0xC) & 0xF;
        SC->w40 = (SC->w34 + 2) & 0xF;
    }
    a->h124[SC->w34] = func_8006E368(a->h10e, (a->w8c[0] + a->w8c[4] - (a->w8c[8] + a->w8c[12])) >> 3);
    a->h144[SC->w34] = func_8006E368(a->h10c, (a->w8c[0] + a->w8c[8] - (a->w8c[4] + a->w8c[12])) >> 3);
    a->h164 += a->h124[SC->w34];
    a->h166 += a->h144[SC->w34];
    a->h164 = a->h164 - a->h124[SC->w38];
    a->h166 = a->h166 - a->h144[SC->w38];
    a->w50 = (short)((short)a->h164 / 10);
    a->w58 = (short)((short)a->h166 / 10);
    a->h168 += a->h124[SC->w3c];
    a->h16a += a->h144[SC->w3c];
    a->h168 -= a->h124[SC->w40];
    a->h16a -= a->h144[SC->w40];
    a->h80 = (short)a->h168 / 10 + ((short)a->h1ac >> 1);
    a->h82 = a->h54;
    a->h84 = a->h1ae + (short)a->h16a / 10;
    a->w70 += a->w10;
    a->w74 += a->w14;
    a->w78 += a->w18;
    a->w10 += a->w40;
    a->w14 += a->w44;
    a->w18 += a->w48;
    a->w70 += a->w40;
    a->w74 += a->w44;
    {
        /* ASM_BARRIER: pin w78 to v0 */
        register int w78 asm("v0") = a->w78;
        /* ASM_BARRIER: pin w48 to a0 */
        register int w48 asm("a0") = a->w48;
        int w10 = a->w10;
        int w18 = a->w18;
        /* ASM_BARRIER: pin wcc to v1 */
        register int wcc asm("v1");
        /* ASM_BARRIER: pin w44 to a1 */
        register int w44 asm("a1") = a->w44;
        /* ASM_BARRIER: pin v2 to v1 */
        register int v2 asm("v1");
        /* ASM_BARRIER: schedule w10/w18 loads before wcc load */
        __asm__ volatile("" : "+r"(w78) : "r"(w10), "r"(w18), "r"(w48), "r"(w44));
        wcc = a->wcc;
        w78 += w48;
        wcc += w44;
        /* ASM_BARRIER: schedule both addu before w78 store */
        __asm__ volatile("" : "+r"(w78), "+r"(wcc));
        a->w78 = w78;
        /* ASM_BARRIER: schedule w78 store before wcc store */
        __asm__ volatile("" : "+r"(wcc));
        a->wcc = wcc;
        v2 = w10;
        /* ASM_BARRIER: force addu v1, a2, zero */
        __asm__ volatile("" : "+r"(v2));
        v2 >>= 2;
        a->w20 = v2;
        a->w16c += 1;
        v2 = w18;
        /* ASM_BARRIER: force addu v1, a3, zero */
        __asm__ volatile("" : "+r"(v2));
        v2 >>= 2;
        a->wc8 = w10;
        a->wd0 = w18;
        a->w28 = v2;
        a->w24 = a->w14 >> 2;
        /* ASM_BARRIER: keep w48 and w44 live across tail so local-alloc picks a2/a3 for w10/w18 */
        __asm__ volatile("" : : "r"(w48));
    }
}
