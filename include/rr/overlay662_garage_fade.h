#ifndef RR_OVERLAY662_GARAGE_FADE_H
#define RR_OVERLAY662_GARAGE_FADE_H
#include "rr/overlay662_input.h"
void PlaySfx(s32 sound);
void ovl662_step_panel_fade_timers(void);
/* GTE INTPL protocol: two packed signed-halfword points supply IR1/IR2 and
 * far-colour1/2; IR0 receives factor. IR3/far-colour3 keep their previous
 * inputs. INTPL also changes other GTE result registers/FLAG; this macro is
 * not a pure two-coordinate arithmetic operation. Only resulting IR1/IR2
 * are written to the destination word. Native tests override the hardware
 * boundary and do not claim to emulate hardware timing or saturation. */
#include "rr_asm.h"
static inline void ovl662_gte_lerp(s32 factor, const void *source, void *destination) {
    const s16 *from = source; /* IR1, IR2, far-colour R, far-colour G */
    s16 *to = destination;
    Psyz_GteDataWrite(9, (u32)(s32)from[0]);
    Psyz_GteDataWrite(10, (u32)(s32)from[1]);
    Psyz_GteCtrlWrite(21, (u32)(s32)from[2]);
    Psyz_GteCtrlWrite(22, (u32)(s32)from[3]);
    Psyz_GteDataWrite(8, (u32)factor);
    Psyz_GteCommand(0x0980011); /* INTPL */
    to[0] = (s16)Psyz_GteDataRead(9);
    to[1] = (s16)Psyz_GteDataRead(10);
}
#define RR_GARAGE_GTE_LERP(factor, source, destination) ovl662_gte_lerp((s32)(factor), (source), (destination))
#endif
