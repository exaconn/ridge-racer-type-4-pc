#ifndef RR_GAME_GFX_SKY_H
#define RR_GAME_GFX_SKY_H

#include "types.h"

extern int TrackSkyTable;
void RenderSkyToRect(int a0, int a1, short *a2, int a3);
void DrawTrackScenery(int a0);
void RunSkyCommandList(char *a0, int a1);
void SetupSkyLightMatrices(char *a0, unsigned short *a1_in);


#endif /* RR_GAME_GFX_SKY_H */
