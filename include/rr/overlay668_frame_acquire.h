#ifndef RR_OVERLAY668_FRAME_ACQUIRE_H
#define RR_OVERLAY668_FRAME_ACQUIRE_H
#include "rr/overlay668_stream.h"
#include "rr/frame_storage_types.h"
#include "psyq/libgpu/gpuenv.h"
extern FrameStorage DrawBuffers[2];
/* Poll at most2000 times. A successful header has the full queue-record
 * owner; activeB8 points to a complete playback state. Config and both frame
 * owners stay live across clear-image callbacks, which may replace activeB8.
 * Width arithmetic wraps32 bits before signed division and narrowing. */
u8 *ovl668_acquire_stream_frame(StrDisplayConfig *config);
#endif
