extern short func_80025FBC();
extern int func_8006E030(), func_8006E09C(), func_8006E108(), func_80090CA4(), func_80090DB4(), func_80090EC4();
extern int D_8010718C;
typedef struct { int a, b, c, d; } w3g2V;
struct w3g2p {
    int pad0[4]; int f10, f14, f18, f1C; char pad1[0x20]; int f40; int pad44; int f48; int pad4c; int f50, f54, f58, f5C;
    char pad2[0x108 - 0x60]; int f108; char pad3[0x1AA - 0x10C]; short s1AA, s1AC; unsigned short u1AE; char pad4[6]; unsigned short u1B6;
    char pad5[0x1E4 - 0x1B8]; unsigned short u1E4;
};

void func_80033BE4(struct w3g2p *p) {
    struct { char a[0x20]; char b[0x20]; short v[3]; short pad; int w[3]; } s;
    int *g = (int *)0x1F800000;
    int r;
    short t;
    unsigned short a1, a2;
    D_8010718C = 1;
    if (p->u1E4 & 0x100) {
        func_8006E09C(s.a, 0x800 - p->f54);
        func_8006E108(s.b, p->f50);
        func_80090CA4(s.a, s.b);
        func_8006E030(s.b, p->f58);
        func_80090DB4(s.a, s.b);
        s.v[0] = 0;
        s.v[1] = -0x64;
        if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x8C;
        s.v[2] = 0;
        func_80090EC4(s.b, s.v, s.w);
        *(w3g2V *)0x1F800008 = *(w3g2V *)&p->f10;
        g[2] -= p->f40;
        g[4] -= p->f48;
        *(w3g2V *)0x1F800018 = *(w3g2V *)&p->f50;
        g[6] += p->s1AC * 2;
        a1 = p->u1B6;
        a2 = p->u1AE;
        g[2] += s.w[0];
        g[3] += s.w[1];
        g[4] += s.w[2];
        g[8] += ((short)a1 / 2) + ((short)a2 / 2);
        return;
    }
    s.v[0] = func_80025FBC(g[6], p->f50) / 8;
    r = func_80025FBC(g[7], p->f54);
    t = r / 6;
    g[6] += s.v[0];
    s.v[1] = t;
    g[7] += t;
    func_8006E09C(s.a, 0x800 - g[7], g[7]);
    func_8006E108(s.b, g[6]);
    func_80090CA4(s.a, s.b);
    func_8006E030(s.b, g[8]);
    func_80090DB4(s.a, s.b);
    s.v[0] = 0;
    s.v[1] = -0xFA;
    if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x122;
    s.v[2] = 0x2D0;
    func_80090EC4(s.b, s.v, s.w);
    *(w3g2V *)0x1F800008 = *(w3g2V *)&p->f10;
    g[8] = p->f58 - p->s1AA;
    g[2] += s.w[0];
    g[3] += s.w[1];
    g[4] += s.w[2];
}
