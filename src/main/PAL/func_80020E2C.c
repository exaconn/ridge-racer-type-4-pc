#include "rr/gpu.h"
extern s32 func_8001FCC8();
void func_80020E2C(a0, a1, a2, a3, a4)
RrSpritePacket *a0; s32 a1; s32 a2; u8 *a3; s32 a4;
{
    RrSpritePacket *p = *(RrSpritePacket **)0x1F800000;
    s32 c, u;
    s32 kb;
    kb = 0xB4;
    if (*a3 != 0) {
        do {
            c = *a3;
            a3++;
            p->len = 4;
            p->code = 0x64;
            p->x = a1;
            p->y = a2;
            p->w = 8;
            p->h = 8;
            c -= 0x30;
            u = c * 8 + 0x6C;
            if (c >= 10) a1 += 3; else a1 += 7;
            p->u = u;
            p->v = kb;
            p->code |= 1;
            p->clut = a4;
            p->addr = a0->addr;
            a0->addr = (u32)p;
            p++;
        } while (*a3 != 0);
    }
    *(s32 *)0x1F800000 = func_8001FCC8(a0, p, 5);
}
