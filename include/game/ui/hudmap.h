#ifndef RR_GAME_UI_HUDMAP_H
#define RR_GAME_UI_HUDMAP_H

#include "types.h"

struct RaceVehicle;
int IsCarFacingReverse(struct RaceVehicle *vehicle);

void AddPartialLine(u32 *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
void DrawRaceTimesScreen(void);
void DrawRaceStartSignalSprite(int a0);


#endif /* RR_GAME_UI_HUDMAP_H */
