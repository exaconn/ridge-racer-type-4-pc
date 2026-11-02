#include "rr/course_choices.h"
#include "rr/race.h"
#include "rr/projection_state.h"
#include "rr/renderer_setup.h"
#ifndef RR_COURSE_SCRATCH
#define RR_COURSE_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000)
#endif
extern void *D_800F4D40;
extern s32 D_800F2DE4,D_800ABEB0;
extern s16 D_800AC64A,D_800AC5E0;
extern s16 ovl660_D_8011793C,ovl660_D_80117934,ovl660_D_8011792C;
extern s16 ovl660_D_80117908[2];
void func_80037110(void);
void func_8002E4F4(RrRaceVehicle *vehicle);
void ovl660_func_801163C4(void)
{
    RrRaceVehicle *state;
    RrProjectionScratchPrefix *scratch;

    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    func_80013890(5);
    D_800AC64A = 0;
    D_800AC5E0 = 3;
    func_8003A198();
    func_80037110();
    state = D_800ABEF8;
    scratch = RR_COURSE_SCRATCH;
    scratch->word18 = 120;
    scratch->word08 = -16;
    scratch->word0C = -248;
    scratch->word10 = -96;
    scratch->word1C = 0;
    scratch->word20 = 0;
    D_800ACC18 = D_800F4D40;
    func_8002E4F4(state);
    state->pos20[1] = 10;
    state->pos20[2] = 308;
    state->rot54 = 2048;
    ovl660_D_8011792C = 0;
    func_80037040((s8 *)&D_800F4D98, D_800ABE08[ovl660_D_8011793C][ovl660_D_80117934].glyph);
    D_800ACBD0[0].first = D_800ABE08[ovl660_D_8011793C][ovl660_D_80117934].first;
    D_800ACBD0[0].second = D_800ABE08[ovl660_D_8011793C][ovl660_D_80117934].second;
    ovl660_D_80117908[0] = func_800370BC(&D_800ABE08[0][0]);
    ovl660_D_80117908[1] = func_800370BC(&D_800ABE08[1][0]);
}
