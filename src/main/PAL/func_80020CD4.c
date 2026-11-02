#include "rr/gpu.h"
extern s32 func_80093C24();
extern s32 func_80095A44();
extern char *D_800ACC14;


void func_80020CD4(a0, a1, a2, a3)
s32 a0;
s32 a1;
u8 *a2;
s32 a3;
{
    RrSpritePacketSignedClut *p = *(RrSpritePacketSignedClut **)0x1F800000;
    s32 t;
    s32 kb;
    s16 c;
    kb = 0xA8;
    if (*a2 != 0) {
        do {
            s32 ch = *a2;
            a2++;
            p->len = 4;
            p->code = 0x64;
            p->x = a0;
            t = ch - 0x30;
            p->y = a1;
            if (t >= 0xD) {
                c = ch * 8 - 0x14C;
                p->w = 4;
                p->h = 8;
                if (t == 0xF)
                    a0 += 5;
                else
                    a0 += 4;
            } else {
                c = t * 12;
                a0 += 10;
                p->w = 12;
                p->h = 12;
            }
            p->u = c;
            p->v = kb;
            p->code |= 1;
            p->clut = a3;
            p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
            ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr = (u32)p;
            p++;
        } while (*a2 != 0);
    }
    func_80093C24(p, 0, 1, 5, 0);
    func_80095A44(D_800ACC14 + 0x70, p);
    *(RrSpritePacketSignedClut **)0x1F800000 = (RrSpritePacketSignedClut *)((char *)p + 0xC);
}
