#include "rr/overlay662_tim.h"

void ovl662_pack_emblem_tims(void) {
    u32 *p;
    GarageTimBytes *src2C = ovl662_editor_workspace.tim2C[ovl662_editor_workspace.active21];
    u32 *img38 = ovl662_editor_workspace.image38[ovl662_editor_workspace.active21];
    PaletteBytes32 *pal48 = (PaletteBytes32 *)(Palette16 *)ovl662_editor_workspace.palette48[ovl662_editor_workspace.active21];

    p = CustomEmblemBuffers.words;
    *(GarageTimBytes *)p = *src2C;
    *p++ = 0x10;
    *p++ = 8;
    *p++ = 44;
    *p++ = 0x00400280;
    *p++ = 0x00010010;
    *(PaletteBytes32 *)p = *pal48;
    p += 8;
    *p++ = 0x80C;
    *p++ = 0x280;
    *p++ = 0x00400010;
    ovl662_composite_emblem_layers(img38, p);
}
