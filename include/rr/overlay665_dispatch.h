#ifndef RR_OVERLAY665_DISPATCH_H
#define RR_OVERLAY665_DISPATCH_H
#include "rr/overlay665_calibration.h"
#include "rr/menu_transition.h"
#include "game/camera/campath.h"
#include "game/camera/camrun.h"
void StartAttractDemo();
void ovl665_func_80116D10(s32);
void ovl665_func_801151A4(void);
/* Dispatch captures the scene and selection at their respective switches.
 * Scene0 increments the fresh value left by initialization, modulo32 bits. */
void ovl665_func_80118EB8(void);
#endif
