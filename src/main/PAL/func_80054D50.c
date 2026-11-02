#include "types.h"

typedef struct {
    s32 active00;
    s32 pad[7];
} RrParticleEntry;

typedef struct {
    s32 unk00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 unk1C[4];
    RrParticleEntry e[24];
} RrParticleSys;

extern RrParticleSys D_800F2AB8;
extern u8 *D_800ACC14;

void func_80053EC4(void);
s32 func_80096464(void);
void func_80093C24(u32 *, s32, s32, s32, s32);
void func_80054128(s32);
void func_80054264(s32);

void func_80054D50(void) {
    u32 *p;
    u32 *ot;
    s32 i;

    if (D_800F2AB8.f0C > 0) {
        D_800F2AB8.f0C--;
        D_800F2AB8.f18 = D_800F2AB8.f14 - ((D_800F2AB8.f14 - D_800F2AB8.f10) * D_800F2AB8.f0C) / D_800F2AB8.f08;
    }
    if (D_800F2AB8.f04 <= 0) {
        func_80053EC4();
        D_800F2AB8.f04 = (func_80096464() % 16) * 2 + 8;
    }
    p = *(u32 **)0x1F800000;
    D_800F2AB8.f04--;
    func_80093C24(p, 0, 1, 0xA0, 0);
    ot = (u32 *)(D_800ACC14 + 0x19C);
    *p = (*p & 0xFF000000) | (*ot & 0x00FFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0x00FFFFFF);
    p += 3;
    *(u32 **)0x1F800000 = p;
    for (i = 0; i < 0x18; i++) {
        if (D_800F2AB8.e[i].active00 & 1) {
            func_80054128(i);
            func_80054264(i);
        }
    }
}
