#ifndef RR_DRIVER_CONTROL_H
#define RR_DRIVER_CONTROL_H
#include "rr/timing.h"

s32 func_800929D4(s32 mode);
/* The tagged dispatcher takes a target machine word as its last argument.
 * This caller uses only scalar control values0 and1. */
s32 func_8009AEAC(s32 command,s32 operation,unsigned long argument);
void func_80092CB8(s32 mode);
void ovl667_func_80115494(void);
#endif
