#include "scratchpad.h"
#include "game/gfx/meshquad.h"
#include "game/gfx/ring2.h"
#include "rr/ring_renderer.h"
extern int func_8005599C();
extern int ScaleMatrix();
extern int func_8006EE3C();
struct Q { char pad[0x20]; char q20[0x544]; int h564; };
extern struct { char pad[0x3D8]; struct Q *q; char pad2[0x188]; int i564; } RaceContext;
struct A { int x, y, z, pad; short rx, ry, rz; };
struct S3 { int x, y, z; };
struct W { short x[12]; short h18, h1a; };

void func_8005EFBC(void) {
    int i;
    struct { struct A a; struct S3 s; char pad2[4]; int m[8]; int v[8]; } l;
    char *e = (struct W *)((char *)RaceContext.q + 0x134);
    for (i = 0; i < 2; i++) {
        if (*(short *)((char *)e + i * 4 + 0x18) > 0) {
            l.a.y = 0;
            l.a.x = 0;
            l.a.z = *(short *)((char *)e + i * 4 + 0x1A);
            l.a.rz = 0;
            l.a.ry = 0;
            l.a.rx = 0;
            func_8005599C(&l.a, (char *)RaceContext.q + 0x20, l.m, l.v);
            l.s.z = 0x800;
            l.s.y = 0x800;
            l.s.x = 0x800;
            ScaleMatrix(l.m, &l.s);
            if (*(short *)((char *)e + i * 4 + 0x18) < 0x18) {
                func_8006EE3C((SCRATCHPAD + 0x7C), &l.a, l.m);
                func_80058038(RaceContext.i564, *(short *)((char *)e + i * 4 + 0x18) + 1, 0);
            } else {
                DrawRing(&l.a, 0x300);
            }
        }
    }
}
