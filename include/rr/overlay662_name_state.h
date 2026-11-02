#ifndef RR_OVERLAY662_NAME_STATE_H
#define RR_OVERLAY662_NAME_STATE_H
#include "rr/layout.h"
/* Complete name storage occupies273A8..273B4, before the selected-key word.
 * Eight positions are displayed and edited; the remaining bytes retain the
 * string terminator/tail used by the resident string-copy helper. */
extern s8 ovl662_D_801273A0,ovl662_D_801273A2,ovl662_D_801273A3;
extern u8 ovl662_D_801273A8[12];
extern s32 ovl662_D_801273B4;
#endif
