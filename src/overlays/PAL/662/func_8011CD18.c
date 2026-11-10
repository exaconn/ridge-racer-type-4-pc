#include "rr/overlay662_palette_refresh.h"

void ovl662_func_8011CD18(void)
{
    Palette16 *palette, *backup, *scratch;
    s32 i;
    s32 value;
    register s32 pulse __asm__("$2");
    s32 adjusted0, adjusted1, adjusted2;
    s32 phase0, phase1, phase2;
    s32 angle1, angle2;
    u16 *source, *target;
    u16 color;

    palette = (Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21];
    backup = (Palette16 *)ovl662_D_801276A8.paletteBackup50[ovl662_D_801276A8.active21];
    scratch = (Palette16 *)ovl662_D_801276A8.paletteScratch58[ovl662_D_801276A8.active21];
    /* Address/lifetime metadata only; no instruction is emitted. */
    __asm__("" : : "m"(ovl662_D_801268C8));
    palette->entries[0] = 0;
    scratch->entries[0] = 0x8000;
    __asm__("" : : "m"(scratch->entries[0]));
    phase0 = ovl662_D_801268C8;
    adjusted0 = phase0;
    if (phase0 < 0) adjusted0 = (phase0 + 4095U);
    pulse = rsin(phase0 - ((adjusted0 >> 12) * 4096));
    scratch->entries[0] |= (pulse / 128 + 32) >> 3;
    __asm__("" : : "m"(scratch->entries[0]));
    phase1 = ovl662_D_801268C8;
    angle1 = (phase1 + 85U);
    adjusted1 = angle1;
    if (angle1 < 0) adjusted1 = (angle1 + 4095U);
    pulse = rsin(angle1 - ((adjusted1 >> 12) * 4096));
    scratch->entries[0] |= ((pulse / 128 + 32) >> 3) << 5;
    __asm__("" : : "m"(scratch->entries[0]));
    phase2 = ovl662_D_801268C8;
    angle2 = (phase2 + 170U);
    adjusted2 = angle2;
    if (angle2 < 0) adjusted2 = (angle2 + 4095U);
    value = rsin(angle2 - ((adjusted2 >> 12) * 4096));
    i = 1;
    if (value < 0) value += 127;
    target = &scratch->entries[1];
    scratch->entries[0] |= (((value >> 7) + 32) >> 3) << 10;
    __asm__("" : : "m"(scratch->entries[0]));
    source = &palette->entries[1];
    ovl662_D_801268C8 = (ovl662_D_801268C8 + 32U);
    do {
        i++;
        color = *source | 0x8000;
        *source = color;
        *target = color;
        target++;
        source++;
    } while (i < 16);
    backup->raw = palette->raw;
    LoadImage(&ovl662_D_801268A0, ovl662_D_801276A8.scratch34);
    LoadImage(&ovl662_D_801268A8, ovl662_D_801276A8.image38[ovl662_D_801276A8.active21]);
    LoadImage(&ovl662_D_801268B0, ovl662_D_801276A8.image40[ovl662_D_801276A8.active21]);
    LoadImage(&ovl662_D_801268B8, palette);
    LoadImage(&ovl662_D_801268C0, scratch);
}
