#ifndef RR_MEMORY_H
#define RR_MEMORY_H

#include "types.h"

/* BIOS A0/2B memset: write the low byte of value and return destination. */
void *func_80096454(void *destination, s32 value, u32 length);

/* Overlap-safe byte copy. For positive length and destination < source, returns
 * destination + length; otherwise returns destination. Nonpositive length is inert. */
u8 *func_80096D34(u8 *destination, const u8 *source, s32 length);

/* Copy the fixed PAL code patch to its kernel RAM destination. */
void func_80095FA0(void);

#endif
