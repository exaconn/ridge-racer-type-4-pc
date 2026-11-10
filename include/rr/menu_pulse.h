#ifndef RR_MENU_PULSE_H
#define RR_MENU_PULSE_H
#include "game/system/pad.h"

#include "rr/tables.h"
#include "rr/runtime_settings.h"

typedef struct {
    u8 first, second;
    s16 duration;
} PulseRow;

RR_ASSERT_SIZE(PulseRow, 4);
RR_ASSERT_OFFSET(PulseRow, duration, 2);

extern const PulseRow *D_8009FF80[], *D_8009FFA0[];
void PadStepPulse(void);
s32 ovl665_func_8011629C(s32 index);

#endif
