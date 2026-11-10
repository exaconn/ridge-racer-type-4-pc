#ifndef RR_OVERLAY662_TOOL_ACTION_H
#define RR_OVERLAY662_TOOL_ACTION_H
#include "rr/overlay662_distance.h"
#include "rr/menu_choice.h"
#include "rr/overlay662_pixels.h"
#include "rr/overlay662_buttons.h"
void ovl662_select_option_target(s32 mode);
/* Mode0 hit-tests table slot4 before checking input. Negative modes reset
 * indices;1/2/4/8 configure navigation and call the mapper (its table contract
 * applies). Other positive words are idle. Sound may replace current state. */
void ovl662_tool_center_cursor(s32 mode);
/* Mode0 uses button owners in slots7/8. Timer value7 is published before sound;
 * after sound, reload the current byte selector/zoom and apply PAL byte wrap. */
void ovl662_tool_cycle_preview_bg(s32 mode);
void ovl662_tool_cycle_zoom(s32 mode);
/* Slot9 selects the base tool; post-sound state is preserved. */
void ovl662_tool_select_pen(s32 mode);
/* Slot5 clears the freshly selected512-word layer after backup/sound.
 * An already empty layer rejects without backup or clearing. */
void ovl662_tool_clear_layer(s32 mode);
/* unsigned long is the complete address word on both PAL and native LP64. */
typedef unsigned long GarageImageAddress;
typedef char GarageImageAddressWidth[(sizeof(GarageImageAddress)==sizeof(void*))?1:-1];
/* Slot3 backs up images/palette, then copies rows63..0 from the inverse
 * buffer into the freshly selected layer. Buffers cover512 words; ordered
 * writes observe overlaps rather than staging a complete input copy. */
void ovl662_tool_flip_layer_v(s32 mode);
/* Slots10..14 update buffer/tool/brush state before sound. */
void ovl662_tool_toggle_emblem_side(s32 mode);
void ovl662_tool_select_line(s32 mode);
void ovl662_tool_select_fill(s32 mode);
void ovl662_tool_cycle_brush_size(s32 mode);
void ovl662_tool_select_move(s32 mode);
/* Slot15 captures undo state, backs up, publishes timer7 and plays sound.
 * Then swap the selected buffer/TIM, image and palette pairs. Sound may change
 * active21 (must remain0..1) and every pointer; use the fresh selection.
 * Both sides of the buffer/TIM pair retain word-aligned, complete TIM-sized backing. */
void ovl662_tool_swap_emblem_sides(s32 mode);
/* Slot16 composites fresh selected image38 into image40 after backup/sound. */
void ovl662_tool_merge_layers(s32 mode);
/* Slots19/20 select layer0/1 and publish their button timer before sound. */
void ovl662_tool_select_layer_0(s32 mode);
void ovl662_tool_select_layer_1(s32 mode);
/* Slots24..26 publish palette colors4..6 before sound. */
void ovl662_tool_select_color_04(s32 mode);
void ovl662_tool_select_color_05(s32 mode);
void ovl662_tool_select_color_06(s32 mode);
/* Slot21 publishes palette color1 before sound. */
void ovl662_tool_select_color_01(s32 mode);
/* Slots22/23 publish palette colors2/3 before sound. */
void ovl662_tool_select_color_02(s32 mode);
void ovl662_tool_select_color_03(s32 mode);
/* Slots27..35 publish palette colors7..15 before sound. */
void ovl662_tool_select_color_07(s32 mode);
void ovl662_tool_select_color_08(s32 mode);
void ovl662_tool_select_color_09(s32 mode);
void ovl662_tool_select_color_10(s32 mode);
void ovl662_tool_select_color_11(s32 mode);
void ovl662_tool_select_color_12(s32 mode);
void ovl662_tool_select_color_13(s32 mode);
void ovl662_tool_select_color_14(s32 mode);
void ovl662_tool_select_color_15(s32 mode);
#endif
