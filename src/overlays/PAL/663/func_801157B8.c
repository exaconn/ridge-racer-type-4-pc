#include "rr/overlay663_preview.h"
#ifndef RR_PROJECTION_SCRATCH
#define RR_PROJECTION_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000U)
#endif

void ovl663_func_801157B8(void)
{
    RrProjectionScratchPrefix *scratch;
    RrRaceVehicle *state;

    ovl663_D_80116898 = 0;
    func_8002A604();
    D_800ACC68 = 2;
    D_800F2DE4 = 0;
    func_80013890(3);
    scratch = RR_PROJECTION_SCRATCH;
    scratch->word18 = 0;
    scratch->word1C = 0;
    scratch->word20 = 0;
    scratch->word08 = 0;
    scratch->word0C = 0;
    scratch->word10 = 0;
    state = D_800ABEF8;
    state->pos20[1] = -200;
    state->pos20[2] = 2024;
    state->pos20[0] = 0;
    state->rot50 = 40;
    state->rot54 = 0;
    state->rot58 = 0;
    func_8003E928(0, 12, 64, 68, 32, 0, 0, 1, 0, 83);
    func_8003D228(0, 12, 128, 76, 34, 0, 0xC01);
    D_800F3AFC[0].half02 = 1;
}
