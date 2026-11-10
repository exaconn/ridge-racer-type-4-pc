#ifndef RR_OVERLAY662_PALETTE_H
#define RR_OVERLAY662_PALETTE_H
#include "rr/overlay662_primitives.h"
/* The same32 palette bytes are copied as eight words and selected as16 colors. */
typedef union {u16 value;u8 bytes[2];} GaragePaletteEntry;
typedef union {
    u32 words[8];
    GaragePaletteEntry colors[16];
    u8 bytes[32];
} GaragePalette;
void ovl662_draw_rgb_sliders(void);
/* Channel0..2; panel supplies a five-bit value. */
void ovl662_draw_rgb_digits(s32 channel,s32 value);
RR_ASSERT_SIZE(GaragePaletteEntry,2);
RR_ASSERT_SIZE(GaragePalette,32);
#endif
