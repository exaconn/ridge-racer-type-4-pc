#include "rr/gpu.h"
#include "game/ui/uipart1.h"
#include "game/race/raceset.h"
#include "rr/sprite_emit.h"
#include "rr/fade_sprite.h"
#include "rr/audio_loop.h"
#include "rr/race_setup.h"
struct O { char pad[0x70]; u32 addr:24; u32 len:8; };
extern s32 func_8004F2D8();
extern s32 D_800FF6B0;
extern s32 D_8010C120[];
void func_8003BFC8(a0) s32 a0;
{
    s32 s1, s2, s3, s4, s5, t;
    s32 c;
    RrSpritePacket *p;
    if (D_800F4D44 == 0 && D_800FF6B0 != 0 && D_800FF6B0 < 4) {
        t = D_8010C120[D_8010C12C];
        if (t < 0) {
            s3 = -1;
            s4 = -1;
            s2 = 0;
        } else {
            s4 = t;
            if (D_8010C114 > 0) {
                if (a0 == 0) D_8010C114--;
                s3 = D_8010C10C;
                s2 = D_8010C104;
                s4 = D_8010C108;
                s1 = D_8010C120[s2] - s3;
                s5 = 0x84;
                if (s1 < 0) {
                    s5 = 0x78;
                    s1 = -s1;
                    c = 0x15;
                } else {
                    c = 0x16;
                }
                if (D_8010C114 == 0x3B && s2 < 2 && a0 == 0) func_8004F2D8(c, 0);
                *(RrSpritePacket **)0x1F800000 = func_8001FEC0((char *)D_800ACC14 + 0x70, *(RrSpritePacket **)0x1F800000, 0x70, 0x60, 0xC, 0xC, s5, 0xA8, 0x7EF8);
                func_8002B5D0(0x80, 0x60, s1, 0x7EF8, 1);
            } else {
                s3 = D_8010C0F8[D_8010C12C];
                s2 = D_8010C12C;
            }
        }
        func_8002B5D0(0x14, 0x1C, s3, 0x7EF8, 0);
        func_8002B5D0(0x14, 0x26, s4, 0x7EF8, 0);
        p = *(RrSpritePacket **)0x1F800000;
        p->len = 4;
        p->code = 0x64;
        p->x = 0xA;
        p->y = 0x26;
        p->w = 0xC;
        p->h = 0xC;
        p->u = s2 * 12 + 12;
        p->v = 0xA8;
        p->clut = 0x7EF8;
        p->code |= 1;
        p->addr = ((struct O *)D_800ACC14)->addr;
        ((struct O *)D_800ACC14)->addr = (u32)p++;
        /* ASM_BARRIER: keeps the 0x1F800000 pointer store after the bitfield store (original order) */
        __asm__ volatile("" : : : "memory");
        *(RrSpritePacket **)0x1F800000 = p;
    }
}
