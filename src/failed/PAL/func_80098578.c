/* CC1_FLAGS: -mno-split-addresses */
typedef struct {
    char pad0[0x3C];
    unsigned char *p3C;
    unsigned char *p40;
    unsigned char b44;
    unsigned char b45;
} S;
typedef struct {
    volatile unsigned char b0;
    char pad1[3];
    volatile unsigned short h4;
    char pad6[4];
    volatile unsigned short hA;
    char padC[2];
    volatile unsigned short hE;
} H;
extern int func_8009A814();
extern volatile unsigned char *D_800ABCC8;
extern H *D_800ABCCC;
extern int D_801146B8, D_801146BC;

int func_80098578(a0, a1)
S *a0;
int a1;
{
    int s1;
    int s2;
    int r;

    if (a1 < 0) {
        a0->b44 = 0xFF;
        a0->b45 = 1;
        *a0->p40 = ~a1;
        s2 = D_800ABCCC->b0;
        if (!(D_800ABCCC->h4 & 1)) {
            while (!(D_800ABCCC->h4 & 1)) ;
        }
        while (func_8009A814() == 0) ;
        D_800ABCCC->b0 = ~a1;
        return (signed char)s2;
    }
    s1 = 0x88;
    if ((*a0->p3C >> 4) == 8) {
        if (a0->b44 >= 9) s1 = 0x22;
    }
    D_801146BC = 0x1AE;
    D_801146B8 = *(volatile unsigned short *)0x1F801120;
    if (!(D_800ABCCC->h4 & 2)) {
        while (!(D_800ABCCC->h4 & 2)) ;
    }
    D_800ABCCC->hE = s1;
    s2 = D_800ABCCC->b0;
    if (!(*D_800ABCC8 & 0x80)) {
        for (;;) {
            r = -0x14;
            if (func_8009A814() != 0) return (signed char)r;
            if (*D_800ABCC8 & 0x80) break;
        }
    }
    D_800ABCCC->b0 = a1;
    if (s1 == 0x22) {
        *D_800ABCC8 = 0x7F;
        D_800ABCCC->hA |= 0x10;
    }
    a0->b45++;
    a0->p3C[a0->b44] = s2;
    a0->b44++;
    return (signed char)s2;
}
