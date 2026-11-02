#include "rr/geometry.h"
typedef struct { char pad[0x20]; u16 f20; char p2[6]; u16 f28; char p3[0xC6]; s32 fF0; } T;
typedef struct { char pad[8]; u16 f8; char p[2]; u16 fC; char p2[0x2E]; } U;
extern U *D_800ABEA4;
extern s32 func_800464E8();
void func_800468F0(T *t) {
    struct { RrVec2s a, b; } w;
    w.a.x = t->f20 - D_800ABEA4[t->fF0].f8;
    w.b.x = t->f28 - D_800ABEA4[t->fF0].fC;
    func_800464E8(w.a, w.b, t->fF0, (char *)t + 0x170, (char *)t + 0x172, (char *)t + 0x188);
}
