#ifndef RR_OVERLAY662_IMAGE_STROKE_H
#define RR_OVERLAY662_IMAGE_STROKE_H
#include "rr/overlay662_image_drag.h"
extern GaragePoint ovl662_eraser_stroke_prev_pt,ovl662_eraser_stroke_curr_pt;
/* Rasterizer entry259FC reads the last two stack arguments as unsigned bytes. */
void ovl662_rasterize_brush_line(u32 *image,s32 x0,s32 y0,s32 x1,s32 y1,u8 color,u8 size);
/* Selected buffers and backup satisfy the drag contract. Coordinates (origins
 * plus offsets2/3) and previous anchor lie in0..63 for image rendering. After
 * the rasterizer, reload the current endpoint before advancing the anchor. */
void ovl662_apply_eraser_stroke(void);
/* Primary-input stroke uses pointsA4/A8, start bit1 and cancel bit4.
 * It shares the same image, coordinate and rasterizer contracts above. */
extern GaragePoint ovl662_prim_pen_prev_pt,ovl662_prim_pen_curr_pt;
void ovl662_apply_primary_pen_stroke(void);
/* Color stroke publishes its fresh endpoint after the rasterizer. Preview
 * stroke keeps its initial anchor, drawing to scratch34 until cancellation
 * commits to the current selected image. Scratch has512 words. Both capture
 * config22/config1D as the rasterizer unsigned-byte color/size arguments. */
extern GaragePoint ovl662_pen_stroke_prev_pt,ovl662_pen_stroke_curr_pt,ovl662_line_stroke_start_pt;
void ovl662_apply_pen_stroke(void);
void ovl662_apply_line_stroke(void);
#endif
