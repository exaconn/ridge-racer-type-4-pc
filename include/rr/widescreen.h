#ifndef RR_WIDESCREEN_H
#define RR_WIDESCREEN_H
#include "types.h"

struct GpuDmaTag;

/* Hor+ 16:9 widescreen (Escape menu Graphics > Aspect ratio > Widescreen
 * 16:9, PsyZ Psyz_VideoSetWidescreen with square pixels). The projection is
 * unchanged, so the 4:3 picture stays exactly where it was. A full-width 3D
 * view (the race, replay, attract demo and both halves of the split screen)
 * also draws what projects up to WideMargin() game pixels past each side of
 * the 320-pixel screen; PsyZ squeezes the display buffer to hold it and
 * presents it at 16:9. The squeeze is fitted to the display mode
 * (Psyz_VideoSetWidescreenSqueeze 0), so the buffer holds exactly the 16:9
 * view at square pixels, without stretching it. Frames without such a view
 * (menus, 2D screens) are drawn and shown exactly as with the option off
 * (Psyz_VideoSetWidescreenNative).
 *
 * With the option off every value here is the original one (margin 0). */

/* Game pixels the squeezed buffer holds past each side of the screen for
 * the current display mode (Psyz_VideoGetWidescreenBufferMargin): 68 for
 * the 320x256 PAL screen (a 16:9 view at square pixels is 455.1 pixels
 * wide), 54 for 320x240. 0 while the option is off. */
s32 WideMargin(void);

/* Game pixels the current view's culling reaches past each screen side:
 * WideMargin() while the option is on and the scene viewport (SetupViewport)
 * is the screen's full width, otherwise 0. */
extern s32 WideMarginX;

/* Horizontal screen bounds of the current view for "some vertex on screen"
 * tests (originally 0 and 0x140). */
#define WIDE_SCREEN_LEFT (-WideMarginX)
#define WIDE_SCREEN_RIGHT (0x140 + WideMarginX)

/* SetupViewport: widen the culling of a full-width view. */
void WideSetupViewport(s32 viewport);
/* MainLoop, before the frame's game step: follow the option. */
void WideBeginFrame(void);
/* A full-width view draws the course: the frame shows 16:9. */
void WideCourseView(void);
/* Whether this frame's buffer is squeezed for 16:9 (this frame or the
 * previous one drew a wide course view); a caller that draws for the
 * squeezed buffer keeps the frame 16:9. Other frames (menus, 2D screens)
 * are drawn and shown exactly as with the option off. */
s32 WideFrameSqueezed(void);
/* HUD x offset towards the 16:9 edges for an element on the left (side < 0)
 * or right (side > 0) of the screen: how far the view reaches past the
 * screen (Psyz_VideoGetWidescreenViewMargin) while race frames show
 * 16:9 (this frame or the previous one drew a wide course view; a moved
 * element keeps its frame 16:9), otherwise 0. */
s32 WideHudShift(s32 side);
#define WIDE_HUD_LEFT(x) ((x) + WideHudShift(-1))
#define WIDE_HUD_RIGHT(x) ((x) + WideHudShift(1))
/* MainLoop, before the frame's ordering tables (secondary may be NULL) are
 * drawn: tell PsyZ whether this buffer holds a 16:9 picture, and reset the
 * per-frame state. */
void WideCommitFrame(s32 bufferY, s32 width, s32 height, const struct GpuDmaTag *ordering,
                     const struct GpuDmaTag *secondary);

#endif
