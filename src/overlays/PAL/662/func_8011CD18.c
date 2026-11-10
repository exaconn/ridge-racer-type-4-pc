#include "rr/overlay662_palette_refresh.h"

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
