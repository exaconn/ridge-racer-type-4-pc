#ifndef RR_GAME_GFX_MOTION1_H
#define RR_GAME_GFX_MOTION1_H

#include "rr/presentation_types.h"

void InitMotionPanel(s32 arg0);
void UpdateMotionPanel(MotionClock20 *clock);
void DrawMotionControllerPanel(void);
void ResetSlotOrder(void);

#endif /* RR_GAME_GFX_MOTION1_H */
