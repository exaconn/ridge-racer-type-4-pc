#ifndef RR_PALETTE16_H
#define RR_PALETTE16_H
#include "rr/layout.h"
typedef union {u16 value;u8 bytes[2];} PaletteEntry16;
typedef struct {u8 data[32];} PaletteBytes32;
typedef union {PaletteBytes32 raw;u32 words[8];PaletteEntry16 colors[16];u16 entries[16];} Palette16;
/* Address arithmetic must preserve full pointers on both PAL and the native
 * fixture host. This is a pointer-sized address, not a PAL machine-word cast. */
typedef unsigned long PaletteAddress;
typedef char rr_palette_address_width[(sizeof(PaletteAddress)==sizeof(void*))?1:-1];
RR_ASSERT_SIZE(PaletteEntry16,2);
RR_ASSERT_SIZE(Palette16,32);
#endif
