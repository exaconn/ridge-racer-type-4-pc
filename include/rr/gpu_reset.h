#ifndef RR_GPU_RESET_H
#define RR_GPU_RESET_H

#include "rr/library.h"
#include "rr/gpu_environment_types.h"

typedef GpuDriver40 GpuResetDriver40;

/* PAL has a 0x80-byte state. Native builds keep the callback pointer whole;
 * the cached environments follow it at their natural aligned offsets. */
typedef struct {
    u8 kind00, enabled01;
    GpuDebugSettings debug02;
    u32 pending08;
    GpuCompletionCallback completion0C;
    DrawEnvironment draw10;
    DisplayEnvironment display6C;
} GpuResetState80;

typedef struct { u16 value, reserved; } GpuDimension4;
typedef struct {
    GpuDimension4 dimensions[3];
    u32 commands0C[2];
} GpuHeightTable14;

extern GpuResetState80 GpuResetState;
extern GpuResetDriver40 GpuDriverTable, *GpuFuncTable;
extern GpuDimension4 D_800ABB40[3];
extern GpuHeightTable14 D_800ABB4C;
extern const char ResetGraphFmt[], StrResetGraphFmt[];

s32 DebugPrintStub(const char *format, ...);
void GPU_memset(s8 *destination, s8 value, s32 length);
s32 _reset(s32 mode);
/* BIOS service receives an encoded low-24-bit PAL address, not a native pointer. */
s32 ResetGraph(s32);

RR_ASSERT_OFFSET(GpuResetState80, debug02, 2);
RR_ASSERT_OFFSET(GpuResetState80, pending08, 8);
#if defined(__mips__)
RR_ASSERT_SIZE(GpuResetState80, 0x80);
RR_ASSERT_OFFSET(GpuResetState80, completion0C, 0xC);
RR_ASSERT_OFFSET(GpuResetState80, draw10, 0x10);
RR_ASSERT_OFFSET(GpuResetState80, display6C, 0x6C);
#else
RR_ASSERT_OFFSET(GpuResetState80, draw10,
                 RR_FIELD_OFFSET(GpuResetState80, completion0C) + sizeof(GpuCompletionCallback));
RR_ASSERT_OFFSET(GpuResetState80, display6C,
                 RR_FIELD_OFFSET(GpuResetState80, draw10) + sizeof(DrawEnvironment));
#endif
RR_ASSERT_SIZE(GpuDimension4, 4);
RR_ASSERT_SIZE(GpuHeightTable14, 0x14);

#endif
