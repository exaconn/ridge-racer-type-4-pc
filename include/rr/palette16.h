#ifndef RR_PALETTE16_H
#define RR_PALETTE16_H
#include "rr/layout.h"
typedef union {u16 value;u8 bytes[2];} RrPaletteEntry16;
typedef struct {u8 data[32];} RrPaletteBytes32;
typedef union {RrPaletteBytes32 raw;u32 words[8];RrPaletteEntry16 colors[16];u16 entries[16];} RrPalette16;
/* Address arithmetic must preserve full pointers on both PAL and the native
 * fixture host. This is a pointer-sized address, not a PAL machine-word cast. */
typedef unsigned long RrPaletteAddress;
typedef char rr_palette_address_width[(sizeof(RrPaletteAddress)==sizeof(void*))?1:-1];
RR_ASSERT_SIZE(RrPaletteEntry16,2);
RR_ASSERT_SIZE(RrPalette16,32);
#endif
