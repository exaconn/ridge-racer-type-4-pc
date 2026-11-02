#include "rr/overlay666_preview.h"
#ifndef RR_PROJECTION_SCRATCH
#define RR_PROJECTION_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000U)
#endif
void ovl666_func_80115A88(void)
{
    RrProjectionScratchPrefix *sp;

    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    ovl666_D_80116F98.word = 0;
    ovl666_D_80116F90 = 0;
    D_800AC64A = 0;
    D_800AC5E0 = 2;
    func_8003E928(0, 24, 88, 68, 32, 0, 0, 1, 0, 84);
    func_8003E928(1, 24, 128, 68, 32, 0, 0, 1, 0, 85);
    func_80013890(5);
    func_8003A198();
    func_80037110();
    D_800ACC18 = D_800F4D40;
    D_800F3AFC[0].half02 = 1;
    sp = RR_PROJECTION_SCRATCH;
    sp->word18 = 120;
    sp->word08 = -16;
    sp->word0C = -248;
    sp->word1C = 0;
    sp->word20 = 0;
    sp->word10 = -96;
    func_8002E4F4(D_800ABEF8);
    D_800ABEF8[0].pos20[1] = 10;
    D_800ABEF8[0].pos20[2] = 308;
    D_800ABEF8[0].rot54 = 2048;
    func_80037040((s8 *)&D_800F4D98, ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].glyph);
    D_800ACBD0[0].first = ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].first;
    D_800ACBD0[0].second = ovl666_D_80116FA0[D_800F3826][ovl666_D_80116F98.word].second;
    func_8002AF8C(0, 1, 1);
}
