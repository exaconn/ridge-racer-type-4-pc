#ifndef RR_GPU_RESET_H
#define RR_GPU_RESET_H

#include "rr/library.h"

/* Resident graphics driver: one name and fifteen callable slots. Only the
 * reset slot's ABI is used here; opaque slots are never invoked through this view. */
typedef void (*RrGpuOpaqueEntry)(void);
typedef struct {
    const char *name00;
    RrGpuOpaqueEntry entries04[12];
    s32 (*reset34)(s32 mode);
    RrGpuOpaqueEntry entries38[2];
} RrGpuResetDriver40;

typedef struct {
    u8 kind00, enabled01;
    RrGpuDebugSettings debug02;
    u32 reserved08[2];
    u8 queue10[0x5C];
    u8 tail6C[0x14];
} RrGpuResetState80;

typedef struct { u16 value, reserved; } RrGpuDimension4;
typedef struct {
    RrGpuDimension4 dimensions[3];
    u32 commands0C[2];
} RrGpuHeightTable14;

extern RrGpuResetState80 D_800ABAC0;
extern RrGpuResetDriver40 D_800ABA78, *D_800ABAB8;
extern RrGpuDimension4 D_800ABB40[3];
extern RrGpuHeightTable14 D_800ABB4C;
extern const char D_80011DF0[], D_80011E10[];

s32 func_8001E84C(const char *format, ...);
void GPU_memset(s8 *destination, s8 value, s32 length);
s32 _reset(s32 mode);
/* BIOS service receives an encoded low-24-bit PAL address, not a native pointer. */
s32 func_80096E24(unsigned long address);
s32 ResetGraph(s32);

RR_ASSERT_SIZE(RrGpuResetDriver40, 0x40);
RR_ASSERT_OFFSET(RrGpuResetDriver40, reset34, 0x34);
RR_ASSERT_SIZE(RrGpuResetState80, 0x80);
RR_ASSERT_OFFSET(RrGpuResetState80, debug02, 2);
RR_ASSERT_OFFSET(RrGpuResetState80, queue10, 0x10);
RR_ASSERT_OFFSET(RrGpuResetState80, tail6C, 0x6C);
RR_ASSERT_SIZE(RrGpuDimension4, 4);
RR_ASSERT_SIZE(RrGpuHeightTable14, 0x14);

#endif
