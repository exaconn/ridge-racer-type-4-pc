extern int func_80096434();
extern char D_800A1AA0[];
extern char D_800A1B60[];
extern char D_800AC650[];
#define W(p,o) (*(int *)((char *)(p) + (o)))
#define Q(p,o) ((char *)*(volatile int *)((char *)(p) + (o)))

void func_8005B8D8(int a0) {
    char *s1 = D_800AC650;
    char *a;
    register char *t asm("$2");
    int v;
    char *t0;
    func_80096434(Q(s1, 0x3D8) + 0x74, 0x70);
    if (a0 < 4) {
        W(Q(s1, 0x3D8), 0x78) = 0;
        W(Q(s1, 0x3D8), 0x80) = (int)(D_800A1AA0 + a0 * 0x30);
    } else {
        a0 -= 4;
        W(Q(s1, 0x3D8), 0x78) = 1;
        W(Q(s1, 0x3D8), 0x80) = (int)(D_800A1B60 + (a0 << 5));
    }
    a = D_800AC650;
    t0 = Q(a, 0x3D8);
    W(t0, 0xE0) = 0x2D0;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 0);
    W(t, 0xB4) = v;
    W(t, 0x84) = v;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 4);
    W(t, 0xB8) = v;
    W(t, 0x88) = v;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 8);
    W(t, 0xBC) = v;
    W(t, 0x8C) = v;
    t0 = Q(a, 0x3D8);
    W(t0, 0x7C) = 1;
}
