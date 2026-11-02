#define W(b, o) (*(int *)((char *)(b) + (o)))
#define H(b, o) (*(short *)((char *)(b) + (o)))
#define UH(b, o) (*(unsigned short *)((char *)(b) + (o)))
extern char *D_800F39C8[];
extern unsigned char D_800F4D98[][4];
extern int D_800FF6B0;
extern int func_80096434(), func_80037618();

void func_80022980(a0, a1, a2)
char *a0; int a1; int a2;
{
    char *s3, *s2, *p0;
    short *p2;
    int i, t;
    s2 = a0 + 0x1F4;
    s3 = D_800F39C8[a1];
    func_80096434(a0, 0x320);
    H(s2, 0x84) = D_800F4D98[a1][3] & 1;
    H(a0, 0x1E6) = a1 * 2;
    W(s2, 0x24) = a1;
    UH(a0, 0x1E4) = UH(s2, 0x24) | 0x110;
    W(s2, 0xA4) = 4;
    t = H(s3, 0x1DE);
    D_800FF6B0 = 2;
    W(s2, 0x94) = t;
    if (t < 0x2710) {
        W(s2, 0x94) = 0x2710;
    }
    p2 = (short *)(s3 + 4);
    W(s2, 0x1C) = (int)(s3 + 0x1C8);
    H(s2, 0x34) = 1;
    H(s2, 0x86) = 1;
    W(s2, 0x18) = (int)s3;
    H(s2, 0x48) = 4;
    H(s2, 0x2A) = 0x64;
    i = 0;
    if (W(s3, 0) >= 0) {
        do {
            p0 = (char *)p2 + 2;
            *p2 = (((H(p0, 0) * 0x36C000) / 1600) << 8) / 100000;
            H(p0, 0x30) = 0;
            if (i == W(s3, 0)) {
                W(s2, 0x98) = (H(p0, 0) * 0x490) / 160;
            }
            i++;
            p2 += 0x1E;
        } while (W(s3, 0) >= i);
    }
    func_80037618(a0, a2, p2);
}
