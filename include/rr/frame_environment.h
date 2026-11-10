#ifndef RR_FRAME_ENVIRONMENT_H
#define RR_FRAME_ENVIRONMENT_H
#include "rr/frame_environment_types.h"
#include "rr/runtime_settings.h"
/* Both bank selectors are passed to this currently empty hook. */
void InitFrameEnvStub(s32 bank);
enum {
    FRAME_CLEAR_BACKGROUND = 1,
    FRAME_INTERLACED = 2,
    FRAME_RGB24 = 4
};
/* RGB uses the low eight bits. Bits0/1/2 select background clearing,
 * interlacing and RGB24 output. RGB24 stores320 pixels in480 VRAM words.
 * Settings halfwords 0x34/0x38 provide screen placement. */
void SetupDrawDispEnvs(s32 r,s32 g,s16 b,u32 flags);
#endif
