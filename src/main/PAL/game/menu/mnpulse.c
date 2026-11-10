#include "game/system/pad.h"
#include "rr/menu_pulse.h"

void PadStepPulse(void) {
    s32 port;
    u16 repeat_tick;
    u16 once_tick;
    const PulseRow *repeat_row;
    const PulseRow *once_row;

    for (port = 0; port < 2; port++) {
        ControllerState5C *output;
        PadPulseState18 *state;
        state = &PadPulseState[port];
        output = &PlayerControllers[port];
        if (Options.controls.pulse[port].disabled02 != 0) {
            /* ASM_BARRIER: retain the disabled-port path before the shared clear. */
            __asm__ ("" ::: "memory");
            output->output19[0] = 0;
            output->output19[1] = 0;
        } else {
            if (state->repeat04 != 0) {
                if (output->kind1C != 0) {
                    repeat_row = &PadPulseRepeatStepTable[state->repeat04 - 1][state->repeatStep0C];
                    output->output19[0] = repeat_row->first;
                    output->output19[1] = repeat_row->second;
                } else {
                    repeat_row = &PadPulseRepeatStepTable[state->repeat04 + 2][state->repeatStep0C];
                    output->output19[0] = 64;
                    output->output19[1] = repeat_row->first;
                }
                repeat_tick = ++state->repeatTick08;
                if ((s16)repeat_tick >= repeat_row->duration) {
                    state->repeatStep0C = (u16)state->repeatStep0C + 1;
                    if (repeat_row[1].duration < 0) {
                        state->repeatTick08 = 0;
                        state->repeatStep0C = 0;
                    }
                }
            } else {
                output->output19[0] = 0;
                output->output19[1] = 0;
            }
            if (state->once06 != 0) {
                if (output->kind1C != 0) {
                    once_row = &PadPulseOnceStepTable[state->once06 - 1][state->onceStep0E];
                    output->output19[0] = once_row->first;
                    output->output19[1] = once_row->second;
                } else {
                    once_row = &PadPulseOnceStepTable[state->once06 + 2][state->onceStep0E];
                    output->output19[0] = 64;
                    output->output19[1] = once_row->first;
                }
                once_tick = ++state->onceTick0A;
                if ((s16)once_tick >= once_row->duration) {
                    state->onceStep0E = (u16)state->onceStep0E + 1;
                    if (once_row[1].duration < 0) {
                        state->once06 = 0;
                        if (output->kind1C != 0) {
                            output->output19[0] = 0;
                        } else {
                            output->output19[0] = 64;
                        }
                        output->output19[1] = 0;
                    }
                }
            }
        }
    }
}
