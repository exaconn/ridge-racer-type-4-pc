#include "types.h"
typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    s16 clut;
    s16 w, h;
} SPRT;
typedef struct {
    u32 addr: 24;
    u32 len: 8;
    u32 code[2];
} DR_MODE;
#define ADDPRIM(ot, p) \
    (p)->addr = ((SPRT *)(ot))->addr; \
    ((SPRT *)(ot))->addr = (u32)(p)
extern u8 *D_800ACC14;
extern void func_80093C24();
extern void func_80058A3C();
void func_800587E4(s32 arg0, u8 *arg1, s32 *arg2) {
    SPRT *sprt;
    s32 i, j;
    s32 x0, y0;
    s32 base_idx;
    u32 idx;
    u8 val;
    
    base_idx = ((0x100 - (arg2[2] + 0x200) / 256) << 8) + (0x100 - (arg2[0] + 0x200) / 256);
    sprt = *(SPRT **)0x1F800000;
    for (i = 0; i < 24; i++) {
        x0 = (i + 24) * 4 + 0x6C;
        y0 = (i - 24) * 4 + 0xD0;
        for (j = 0; j < 24; j++) {
            val = 0;
            idx = base_idx + (i - 12) * 256 + (j - 12);
            if (idx < 0x10000) val = arg1[idx];
            sprt->len = 4; sprt->code = 0x64; sprt->w = 4; sprt->h = 8;
            if (val == 0) {
                idx = 1;
                sprt->u0 = 0x20; sprt->v0 = 0xE8;
                sprt->x0 = x0;
                sprt->y0 = y0;
                sprt->code = (sprt->code & ~2) | 1;
                sprt->clut = 0x7E24;
            } else {
                idx = 0;
                sprt->r0 = 0xE4; sprt->g0 = 0x40; sprt->b0 = 0x18;
                sprt->y0 = y0 - 0x18;
                sprt->u0 = 0x20; sprt->v0 = 0xE8;
                sprt->x0 = x0;
                sprt->code = (sprt->code & ~2) | 1;
                sprt->clut = 0x7F64;
            }
            ADDPRIM(D_800ACC14 + idx * 4 + 0x98, sprt);
            sprt++;
            x0 -= 4;
            y0 += 4;
        }
    }
    func_80093C24(sprt, 0, 1, 0x19, 0);
    (sprt)->addr = ((SPRT *)(D_800ACC14 + 0x9C))->addr;
    ((SPRT *)(D_800ACC14 + 0x9C))->addr = (u32)(sprt);
    ((struct { void *p; } *)0x1F800000)->p = (DR_MODE *)sprt + 1;
    func_80058A3C(arg2);
}
