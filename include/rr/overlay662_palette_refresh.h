#ifndef RR_OVERLAY662_PALETTE_REFRESH_H
#define RR_OVERLAY662_PALETTE_REFRESH_H
#include "psyq/libgpu/gpuenv.h"
#include "rr/overlay662_garage.h"
#include "rr/palette16.h"
#include "rr/gpu.h"
#include "rr/trig.h"
extern s32 ovl662_palette_pulse_phase;
extern GpuRectangle ovl662_vram_scratch_rect,ovl662_vram_layer0_rect,ovl662_vram_layer1_rect,ovl662_vram_palette_rect,ovl662_vram_composite_rect;
/* active21 stays0..1. Each palette pointer addresses words[0] of a complete
 * Palette16 owner; owners may coincide. Captured owners stay valid through
 * all callbacks. Sine returns -4096..4096. Callbacks may change the counter,
 * palette contents and garage roots; each following phase/root is read fresh.
 * The counter and phase offsets wrap as 32-bit words. GPU upload is a service. */
void ovl662_upload_editor_textures(void);
#endif
