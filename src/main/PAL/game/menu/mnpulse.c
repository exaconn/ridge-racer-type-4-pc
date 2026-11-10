#include "game/system/pad.h"
#include "rr/menu_pulse.h"

void PadStepPulse(void) {
    ControllerState5C *next_output;
    PadPulseState18 *next_state;
    /* ASM_BARRIER: reuse V1 for selector, step and duration in disjoint lifetimes. */
#ifdef __mips__
    register s32 word asm("$3");
#else
    s32 word;
#endif
    s32 port;
    u16 repeat_tick;
    u16 once_tick;
    const PulseRow *repeat_row;
    const PulseRow *once_row;

    PulseSetting4 *settings;
    const PulseRow **repeat, **once;
    s32 small;
    port = 0;
    settings = Options.controls.pulse;
    repeat = D_8009FFA0;
    small = 64;
    once = D_8009FF80;
    next_output = PlayerControllers;
    next_state = PadPulseState;
    do {
        /* ASM_BARRIER: keep the per-port state cursor in A2 inside the loop. */
#ifdef __mips__
        register PadPulseState18 *state asm("$6") = next_state;
#else
        PadPulseState18 *state = next_state;
#endif
        /* ASM_BARRIER: keep the output cursor in A1 after address initialization. */
#ifdef __mips__
        register ControllerState5C *output asm("$5") = next_output;
#else
        ControllerState5C *output = next_output;
#endif
        if (settings[port].disabled02 != 0) {
            /* ASM_BARRIER: retain the disabled-port path before the shared clear. */
            __asm__ ("" ::: "memory");
            output->output19[0] = 0U;
            goto clear_second;
        }
        word = state->repeat04;
        if (word != 0) {
            s32 index;
            if (output->kind1C != 0) {
                index = word - 1;
                word = state->repeatStep0C;
                repeat_row = repeat[index] + word;
                output->output19[0] = (u8) repeat_row->first;
                output->output19[1] = (u8) repeat_row->second;
            } else {
                index = word + 2;
                word = state->repeatStep0C;
                repeat_row = repeat[index] + word;
                output->output19[0] = small;
                output->output19[1] = (u8) repeat_row->first;
            }
            repeat_tick = state->repeatTick08 + 1;
            state->repeatTick08 = repeat_tick;
            if ((s16) repeat_tick >= (word = repeat_row->duration)) {
                state->repeatStep0C = (s16) ((u16) state->repeatStep0C + 1);
                if (repeat_row[1].duration < 0) {
                    state->repeatTick08 = 0U;
                    state->repeatStep0C = 0;
                }
            }
        } else {
            output->output19[0] = 0U;
            output->output19[1] = 0U;
        }
        word = state->once06;
        if (word != 0) {
            s32 index;
            if (output->kind1C != 0) {
                index = word - 1;
                word = state->onceStep0E;
                once_row = once[index] + word;
                output->output19[0] = (u8) once_row->first;
                output->output19[1] = (u8) once_row->second;
            } else {
                index = word + 2;
                word = state->onceStep0E;
                once_row = once[index] + word;
                output->output19[0] = small;
                output->output19[1] = (u8) once_row->first;
            }
            once_tick = state->onceTick0A + 1;
            state->onceTick0A = once_tick;
            if ((s16) once_tick >= (word = once_row->duration)) {
                state->onceStep0E = (s16) ((u16) state->onceStep0E + 1);
                if (once_row[1].duration < 0) {
                    state->once06 = 0;
                    if (output->kind1C != 0) {
                        output->output19[0] = 0U;
                    } else {
                        output->output19[0] = small;
                    }
clear_second:
                    output->output19[1] = 0U;
                }
            }
        }
        next_output = output + 1;
        port += 1;
        next_state = state + 1;
    } while (port < 2);
}
