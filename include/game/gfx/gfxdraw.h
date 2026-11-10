#ifndef RR_GAME_GFX_GFXDRAW_H
#define RR_GAME_GFX_GFXDRAW_H

#include "types.h"

void UpdateRaceCamera(int arg0, void *arg1);
void BuildObjectLightAndRotMatrices();
int RotTransPersOrigin();
void InitRaceContextBuffers(void);
u8 *EmitSpotlightConePolys(s32 a0);
void InitTrackStartPos(void);


#endif /* RR_GAME_GFX_GFXDRAW_H */
