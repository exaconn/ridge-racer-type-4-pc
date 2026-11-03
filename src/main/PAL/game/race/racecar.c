#include "game/car/carmove1.h"
#include "game/car/carparam.h"
#include "game/car/carsetup.h"
#include "rr/overlay662_distance.h"
extern int func_80024A88();
extern int func_80025FBC();
extern int func_8002648C();
extern int func_8002679C();
extern int func_80026F5C();
extern int func_80027C54();
extern int func_8002C638();
extern int func_8006E368();
extern int func_80090424();
extern int func_800904F4();
extern int D_8010718C;
extern struct w3g2o { int pad0[4]; int f10, f14, f18, f1C; char pad1[0x10]; int f30; int pad34; int f38; char pad2[0x18]; int f54; } D_800ABEF8;
typedef struct { int a, b, c, d; } w3g2V;
struct w3g2q { int pad0[4]; int f10, f14, f18, f1C; char pad1[0x10]; int f30; int pad34; int f38; char pad2[0x18]; int f54;
  char pad3[0x1D4 - 0x58]; short s1D4, s1D6; short s1D8; short s1DA, s1DC, s1DE; unsigned short u1E0; short s1E2, s1E4, s1E6, s1E8, s1EA; };

void func_80033808(struct w3g2q *p) {
    int sp[3];
    int v, t, u, v2, v3, v4;
    int *g = (int *)0x1F800000;
    int *q = (int *)0x1F80007C;
    D_8010718C = 0;
    u = p->s1D8;
    p->s1D8 = (unsigned short)p->s1D8 - (u * 6) / 100;
    p->s1D8 = p->s1D8 + p->u1E0;
    v = p->s1D8 * func_80090424(p->s1D4 + (p->s1EA << 11));
    if (v < 0) v += 0xFF;
    p->f30 = v >> 8;
    v2 = p->s1D8 * func_800904F4(p->s1D4 + (p->s1EA << 11));
    if (v2 < 0) v2 += 0xFF;
    p->f38 = v2 >> 8;
    func_80024A88(p);
    t = func_80025FBC(p->f54, p->s1D4);
    *q = 0;
    p->f54 = p->f54 + t / 2;
    func_8002679C(p);
    func_8002C638(p);
    func_800244B0(p);
    func_8002648C(p);
    func_80022888(p);
    func_80026F5C(p);
    func_80027C54(p);
    func_80028B30(p);
    v3 = func_80090424(D_800ABEF8.f54);
    if (v3 < 0) v3 += 0x7F;
    sp[0] = D_800ABEF8.f10 + (v3 >> 7) - g[2];
    sp[1] = D_800ABEF8.f14 - g[3];
    v4 = func_800904F4(D_800ABEF8.f54);
    if (v4 < 0) v4 += 0x7F;
    sp[2] = D_800ABEF8.f18 + (v4 >> 7) - g[4];
    *(w3g2V *)0x1F800008 = *(w3g2V *)&p->f10;
    g[3] -= 0x100;
    g[7] = 0x400 - func_8006E368(sp[0], sp[2]);
    g[6] = 0x400 - func_8006E368(sp[1], func_80090758(sp[0] * sp[0] + sp[2] * sp[2]) >> 6);
    g[8] = 0;
}
