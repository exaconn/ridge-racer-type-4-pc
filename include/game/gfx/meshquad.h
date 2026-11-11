#ifndef RR_GAME_GFX_MESHQUAD_H
#define RR_GAME_GFX_MESHQUAD_H

#include "types.h"
#include "rr/model_table.h"

int StepCarShowcaseScene(int a0);
/* table is a decoded host model table (full-width mesh pointers). */
void DrawMeshPrimList(const RroModelTable *table, int index, int bias);

#endif /* RR_GAME_GFX_MESHQUAD_H */
