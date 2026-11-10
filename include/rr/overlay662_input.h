#ifndef RR_OVERLAY662_INPUT_H
#define RR_OVERLAY662_INPUT_H
#include "game/system/pad.h"
#include "rr/overlay662_garage.h"
#include "rr/tables.h"
#include "rr/memory.h"
extern s16 ovl662_analog_cursor_accum[2];
/* Analog movement with per-axis fractional accumulation outside112..144.
 * Vertical movement is doubled before its contribution is subtracted from
 * the accumulator. Neutral axes clear their accumulator. */
void ovl662_read_analog_cursor_delta(GaragePoint *output);
extern u32 ovl662_stick_repeat_up, ovl662_stick_repeat_down;
extern u32 ovl662_stick_repeat_left, ovl662_stick_repeat_right;
extern u32 ovl662_dpad_repeat_up, ovl662_dpad_repeat_down;
extern u32 ovl662_dpad_repeat_left, ovl662_dpad_repeat_right;
/* Digital movement: Mask0x8 selects the faster step; later opposite directions win. */
void ovl662_read_dpad_cursor_delta(GaragePoint *output);
/* Direction repeat events at counter1 and40; values47 and above wrap to40.
 * Active directions write the accumulated mask even when no event fires;
 * neutral directions reset their counter and preserve the caller's output. */
void ovl662_poll_stick_direction_repeat(u32 *output);
void ovl662_poll_dpad_direction_repeat(u32 *output);
#endif
