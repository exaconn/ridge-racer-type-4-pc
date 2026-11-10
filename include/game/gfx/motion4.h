#ifndef RR_GAME_GFX_MOTION4_H
#define RR_GAME_GFX_MOTION4_H

#include "rr/presentation_types.h"

/* Clock, controller and key storage are disjoint. Current/following keys are
 * accessible. Word products and timers wrap32; channel offsets wrap16. */
void UpdateScrollTrack(MotionClock20 *clock);

#endif
