#ifndef RR_OVERLAY662_PALETTE_EDIT_H
#define RR_OVERLAY662_PALETTE_EDIT_H
#include "rr/overlay662_image_drag.h"
#include "rr/palette16.h"
/* Palette pointers address words[0] of a complete32-byte Palette16 owner.
 * active21 is0/1 and config22 is0..15. Active horizontal clips give x149..212
 * (component0..31). Backup copies both512-word images and8 palette words. */
extern s32 ovl662_red_bar_dragging,ovl662_green_bar_dragging,ovl662_blue_bar_dragging;
extern u16 ovl662_red_bar_saved_color,ovl662_green_bar_saved_color,ovl662_blue_bar_saved_color;
void ovl662_drag_palette_red_bar(void);
void ovl662_drag_palette_green_bar(void);
void ovl662_drag_palette_blue_bar(void);
/* Step buttons capture the selected palette entry before input handling.
 * Timer is published before sound. After sound, update the captured entry
 * using its fresh contents and the component captured before backup/sound. */
void ovl662_step_palette_red_down(void);
void ovl662_step_palette_red_up(void);
void ovl662_step_palette_green_down(void);
void ovl662_step_palette_green_up(void);
void ovl662_step_palette_blue_down(void);
void ovl662_step_palette_blue_up(void);
#endif
