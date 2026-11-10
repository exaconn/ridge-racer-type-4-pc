#ifndef RR_OVERLAY662_INIT_H
#define RR_OVERLAY662_INIT_H
#include "rr/overlay662_garage.h"

/* Opaque saved word: resident code copies its 32 bits to/from buffer offset0x280. */
extern u32 EmblemEditFrames;
void ovl662_reset_editor_state(void);
#endif
