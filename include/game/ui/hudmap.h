#ifndef RR_GAME_UI_HUDMAP_H
#define RR_GAME_UI_HUDMAP_H

#include "rr/gpu.h"

struct RaceVehicle;
int IsCarFacingReverse(struct RaceVehicle *vehicle);

/* Draws the first intensity/256 of the line a->b (two lines when doubled);
 * nonnegative mode selects LINE_F2 plus a draw-mode packet, negative tiles. */
void AddPartialLine(GpuDmaTag *ordering, s32 x0, s32 y0, s32 x1, s32 y1,
    s32 intensity, s32 doubled, s32 red, s32 green, s32 blue, s32 mode);
void DrawRaceTimesScreen(void);
void DrawRaceStartSignalSprite(int a0);


#endif /* RR_GAME_UI_HUDMAP_H */
