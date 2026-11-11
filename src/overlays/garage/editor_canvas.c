#include "rr/overlay662_garage.h"
#include "rr/animated_sprite_grid.h"
#include "rr/overlay662_draw_area.h"
#include "rr/overlay662_palette_refresh.h"
#include "rr/overlay662_update.h"
#include "rr/frame_storage_types.h"
#include "rr/trig.h"
#include "rr/overlay662_options.h"

/* Local views of the overlay primitive helpers: this unit passes the
 * rectangle coordinates as signed 16-bit values. */
u8 *ovl662_emit_wire_box(u32 *ordering, u8 *cursor, s16 x, s16 y, s16 w, s16 h,
    u32 r, u32 g, u32 b);
u8 *ovl662_emit_textured_quad(u32 *ordering, u8 *cursor, s16 x, s16 y, s16 w, s16 h,
    s16 u, s16 v, u32 uw, u32 vh, u32 clut, u32 tpage);

/* Zoomed preview panel: clip, optional selection outlines, then the preview
 * image at the current zoom (config1C) and its frame. */
void ovl662_draw_zoomed_canvas(s32 renderArgument)
{
#ifndef RR_GARAGE_SCRATCH
#define RR_GARAGE_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch;
    GarageSelectionPrefix *garage;
    u32 *order0, *order1, *order;
    u16 x, y, w, h;
    s16 size, size2;
    s32 clut, tpage;
    u16 u, v;

    scratch = RR_GARAGE_SCRATCH;
    order0 = scratch->order + 134;
    order1 = scratch->order + 135;
    scratch->cursor = ovl662_emit_draw_area(order0, (u32 *)scratch->cursor, 12, 61, 296, 352);
    if (ovl662_editor_workspace.reset08[4] > 0 && !(ovl662_editor_cursor_state.words[0] & 6)) {
        if (ovl662_editor_workspace.config1E < 2 || ovl662_editor_workspace.config1E == 3
            || ovl662_editor_workspace.config1F == 1) {
            x = ovl662_editor_workspace.reset08[2];
            y = ovl662_editor_workspace.reset08[3];
            w = ovl662_editor_workspace.reset08[4];
            h = ovl662_editor_workspace.reset08[5];
            scratch->cursor = ovl662_emit_wire_box(order0, scratch->cursor,
                (s16)x * ovl662_editor_workspace.config1C + 82,
                (s16)y * ovl662_editor_workspace.config1C * 2 + 71,
                (s16)w * ovl662_editor_workspace.config1C,
                (s16)h * ovl662_editor_workspace.config1C * 2,
                0, ovl662_editor_workspace.shadeD0, 0);
        }
        if (ovl662_editor_workspace.config1E == 2 || ovl662_editor_workspace.config1F == 0) {
            x = ovl662_editor_workspace.reset08[6];
            y = ovl662_editor_workspace.reset08[7];
            w = ovl662_editor_workspace.reset08[8];
            h = ovl662_editor_workspace.reset08[9];
            scratch->cursor = ovl662_emit_wire_box(order0, scratch->cursor,
                (s16)x * ovl662_editor_workspace.config1C + 82,
                (s16)y * ovl662_editor_workspace.config1C * 2 + 71,
                (s16)w * ovl662_editor_workspace.config1C,
                (s16)h * ovl662_editor_workspace.config1C * 2,
                0, ovl662_editor_workspace.shadeD0, 0);
        }
    }
    switch (ovl662_editor_workspace.config1C) {
    case 2:
        x = 81; y = 69; w = 133; h = 265; size = 67; size2 = 67;
        break;
    case 4:
        x = 80; y = 67; w = 137; h = 273; size = 35; size2 = 35;
        break;
    case 8:
        x = 78; y = 63; w = 145; h = 289; size = 19; size2 = 19;
        break;
    default:
        x = 74; y = 55; w = 161; h = 321; size = 11; size2 = 11;
        break;
    }
    garage = &ovl662_editor_workspace;
    order = order0;
    u = garage->reset08[0] + 139;
    v = garage->reset08[1] + 1;
    if (garage->layer23) order = order1;
    clut = 4272;
    tpage = 12;
    scratch->cursor = ovl662_emit_textured_quad(order, scratch->cursor,
        x, y, w, h, u, v, size2, size, clut, tpage);
    scratch->cursor = ovl662_emit_textured_quad(order0, scratch->cursor,
        x, y, w, h, garage->reset08[0] + 71, garage->reset08[1] + 1, size2, size, clut, tpage);
    scratch->cursor = ovl662_emit_textured_quad(order1, scratch->cursor,
        x, y, w, h, garage->reset08[0] + 3, garage->reset08[1] + 1, size2, size, 4273, tpage);
    scratch->cursor = ovl662_emit_draw_area(order1, (u32 *)scratch->cursor, 82, 71, 128, 256);
    scratch->cursor = ovl662_emit_wire_box(order1, scratch->cursor, 81, 69, 130, 260, 255, 255, 255);
}

void ovl662_upload_editor_textures(void)
{
    u16 *palette, *backup, *scratch;
    s32 i;
    s32 value;
    s32 adjusted0, adjusted1, adjusted2;
    s32 phase0, phase1, phase2;
    s32 angle1, angle2;
    u16 *source, *target;
    u16 color;

    palette = (u16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21];
    backup = (u16 *)ovl662_editor_workspace.paletteBackup50[ovl662_editor_workspace.active21];
    scratch = (u16 *)ovl662_editor_workspace.paletteScratch58[ovl662_editor_workspace.active21];
    palette[0] = 0;
    scratch[0] = 0x8000;
    phase0 = ovl662_palette_pulse_phase;
    adjusted0 = phase0;
    if (phase0 < 0) adjusted0 = (phase0 + 4095U);
    scratch[0] |= (rsin(phase0 - ((adjusted0 >> 12) * 4096)) / 128 + 32) >> 3;
    phase1 = ovl662_palette_pulse_phase;
    angle1 = (phase1 + 85U);
    adjusted1 = angle1;
    if (angle1 < 0) adjusted1 = (angle1 + 4095U);
    scratch[0] |= ((rsin(angle1 - ((adjusted1 >> 12) * 4096)) / 128 + 32) >> 3) << 5;
    phase2 = ovl662_palette_pulse_phase;
    angle2 = (phase2 + 170U);
    adjusted2 = angle2;
    if (angle2 < 0) adjusted2 = (angle2 + 4095U);
    value = rsin(angle2 - ((adjusted2 >> 12) * 4096));
    i = 1;
    if (value < 0) value += 127;
    target = &scratch[1];
    scratch[0] |= (((value >> 7) + 32) >> 3) << 10;
    source = &palette[1];
    ovl662_palette_pulse_phase = (ovl662_palette_pulse_phase + 32U);
    do {
        i++;
        color = *source | 0x8000;
        *source = color;
        *target = color;
        target++;
        source++;
    } while (i < 16);
    ((Palette16 *)backup)->raw = ((Palette16 *)palette)->raw;
    LoadImage(&ovl662_vram_scratch_rect, ovl662_editor_workspace.scratch34);
    LoadImage(&ovl662_vram_layer0_rect, ovl662_editor_workspace.image38[ovl662_editor_workspace.active21]);
    LoadImage(&ovl662_vram_layer1_rect, ovl662_editor_workspace.image40[ovl662_editor_workspace.active21]);
    LoadImage(&ovl662_vram_palette_rect, palette);
    LoadImage(&ovl662_vram_composite_rect, scratch);
}

void ovl662_update_editor_overlay(void)
{
    s32 first,second;
    if(ovl662_editor_workspace.fade00>=0) {
        union {u32 bits;s32 value;} intensity;
        if(ovl662_editor_workspace.fade00!=0)ovl662_editor_workspace.fade00--;
        intensity.bits=(64U-ovl662_editor_workspace.fade00)<<2;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,intensity.value,69);
    }
    if(ovl662_editor_workspace.flagsD1&1)UpdateBadgeRowAnim(0);
    if(ovl662_editor_workspace.flagsD1&2){DrawUiRow(0);DrawUiRow(1);}
    if(ovl662_editor_workspace.flagsD1&4){SkipDialoguePage(CardDialogueStatePtr);UpdateDialogue(22,320,CardDialogueStatePtr);}
    first = 0;
    if (ovl662_editor_workspace.fade00 < 0) {
        if (ovl662_editor_workspace.flagsD1 & 3) {
            if (ovl662_editor_workspace.flagsD1 & 4) {
                first = 1;
                second = first;
                goto apply_options;
            }
        } else {
            first = 1;
        }
    }
    second = 0;
apply_options:;
    SetMenuFrameOptions(first,second,0);
    UpdateScreenFade(0x1004E);
}

s32 ovl662_is_active_layer_empty(void)
{
    u32 mode=ovl662_editor_workspace.layer23;
    u32 selected=ovl662_editor_workspace.active21;
    u32 *cursor;
    s32 i;
    if(mode==0)
        cursor=ovl662_editor_workspace.image38[selected];
    else
        cursor=ovl662_editor_workspace.image40[selected];
    for(i=0;i<512;cursor++){
        i++;
        if(*cursor)return 0;
    }
    return 1;
}

void ovl662_backup_layer_and_palette(void)
{
    s32 i;
    u8 selected=ovl662_editor_workspace.active21;
    u8 other;
    u32 *source,*destination;
    ovl662_editor_workspace.dirtyD3=1;
    other=!selected;
    source=ovl662_editor_workspace.image38[selected];
    destination=ovl662_editor_workspace.image38[other];
    for(i=0;i<512;){
        i++;
        *destination++=*source++;
    }
    source=ovl662_editor_workspace.image40[selected];
    destination=ovl662_editor_workspace.image40[other];
    for(i=0;i<512;){
        i++;
        *destination++=*source++;
    }
    source=ovl662_editor_workspace.palette48[selected];
    destination=ovl662_editor_workspace.palette48[other];
    for(i=0;i<8;){
        i++;
        *destination++=*source++;
    }
}

void ovl662_update_cursor_pulse(void)
{
    ovl662_editor_workspace.shadeD0=rsin(ovl662_editor_workspace.phaseCC)/64-65;
    ovl662_editor_workspace.phaseCC+=96U;
    ovl662_editor_workspace.phaseCC=(s32)ovl662_editor_workspace.phaseCC%4096;
}

void ovl662_init_option_groups(s32 mode)
{
 if(mode<0){
  ovl662_editor_workspace.options78.flat[0]=ovl662_option_group_0;
  ovl662_editor_workspace.options78.flat[1]=ovl662_option_group_1;
  ovl662_editor_workspace.options78.flat[2]=ovl662_option_group_2;
  ovl662_editor_workspace.options78.flat[3]=ovl662_option_group_3;
  ovl662_editor_workspace.options78.flat[4]=ovl662_option_group_4;
  ovl662_editor_workspace.options78.flat[5]=ovl662_option_group_5;
  ovl662_editor_workspace.options78.flat[6]=ovl662_option_group_6;
  ovl662_editor_workspace.options78.flat[7]=ovl662_option_group_7;
  ovl662_editor_workspace.optionIndices98.flat[0]=0;
  ovl662_editor_workspace.optionIndices98.flat[1]=1;
  ovl662_editor_workspace.optionIndices98.flat[2]=0;
  ovl662_editor_workspace.optionIndices98.flat[3]=0;
  ovl662_editor_workspace.optionIndices98.flat[4]=1;
  ovl662_editor_workspace.optionIndices98.flat[5]=0;
  ovl662_editor_workspace.optionIndices98.flat[6]=0;
  ovl662_editor_workspace.optionIndices98.flat[7]=0;
  ovl662_editor_workspace.current60=0;
  ovl662_editor_workspace.selectorsB8[0]=0;
  ovl662_editor_workspace.selectorsB8[1]=0;
  ovl662_editor_workspace.selectorsB8[2]=0;
  ovl662_editor_workspace.selectorsB8[3]=0;
  ovl662_editor_workspace.saved64[0]=0;
  ovl662_editor_workspace.saved64[1]=0;
  ovl662_editor_workspace.saved64[2]=0;
  ovl662_editor_workspace.saved64[3]=0;
  ovl662_editor_workspace.saved64[4]=0;
 }
}
