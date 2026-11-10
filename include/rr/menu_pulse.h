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

extern const PulseRow *PadPulseOnceStepTable[], *PadPulseRepeatStepTable[];
void PadStepPulse(void);
s32 ovl665_detect_controller_kind(s32 index);

#endif
