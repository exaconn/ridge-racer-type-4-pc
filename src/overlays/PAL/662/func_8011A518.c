#include "rr/overlay662_arena_init.h"
#include "rr/load_cursor.h"

void ovl662_init_editor_arenas(void)
{
    s32 stage,i;
    register u8 *base __asm__("$3");
    u32 *clear;
    register u8 *second __asm__("$6");
    GarageSelectionPrefix *owner;
    register u8 *allocated __asm__("$2");
    stage=(ovl662_arena_init_stage+1U);
    ovl662_arena_init_stage=stage;
    switch (stage) {
    case 1:
        ovl662_emit_indexed_sprite(-1,0,0,0);
        ovl662_dispatch_editor_actions(-1);
        ovl662_init_option_groups(-1);
        ovl662_reset_editor_state();
        base=MenuSharedScratchPtr+4;
        owner=&ovl662_editor_workspace;
        base-=((GarageArenaAddress)MenuSharedScratchPtr&3U);
        second=base+0x840;
        /* Retain each publication of the shared arena cursor. */
        *(void **)&LoadCursor=base;
        *(void *volatile*)&LoadCursor=second;
        allocated=base+0x1080;
        *(void **)&LoadCursor=allocated;
        owner->buffers24[1]=(u32*)allocated;
        allocated=base+0x18C0;
        *(void **)&LoadCursor=allocated;
        owner->tim2C[1]=(GarageTimBytes*)allocated;
        allocated=base+0x2100;
        *(void **)&LoadCursor=allocated;
        owner->scratch34=(u32*)allocated;
        allocated=base+0x2900;
        *(void **)&LoadCursor=allocated;
        owner->paletteScratch58[0]=(u32*)allocated;
        allocated=base+0x2920;
        *(void **)&LoadCursor=allocated;
        owner->paletteScratch58[1]=(u32*)allocated;
        allocated=base+0x2940;
        *(void **)&LoadCursor=allocated;
        allocated=base+0x40;
        owner->image38[0]=(u32*)allocated;
        allocated=base+0x880;
        owner->image40[0]=(u32*)allocated;
        allocated=base+0x14;
        owner->palette48[0]=(u32*)allocated;
        allocated=base+0x854;
        owner->paletteBackup50[0]=(u32*)allocated;
        allocated=base+0x10C0;
        owner->image38[1]=(u32*)allocated;
        allocated=base+0x1900;
        owner->image40[1]=(u32*)allocated;
        allocated=base+0x1094;
        owner->palette48[1]=(u32*)allocated;
        allocated=base+0x18D4;
        owner->paletteBackup50[1]=(u32*)allocated;
        GarageSavedDecalTimA=base;
        owner->buffers24[0]=(u32*)base;
        owner->tim2C[0]=(GarageTimBytes*)second;
        GarageSavedDecalTimB=second;
        break;
    case 2:
        StoreImage(&ovl662_vram_emblem_image_rect,ovl662_editor_workspace.image38[0]);
        StoreImage(&ovl662_vram_emblem_clut_rect,ovl662_editor_workspace.palette48[0]);
        clear=ovl662_editor_workspace.image40[0];
        for(i=511;i>=0;i--)*clear++=0;
        break;
    case 3:
        ovl662_arena_init_stage=0;
        ovl662_backup_layer_and_palette();
        ovl662_editor_workspace.dirtyD3=0;
        ClearScreenOverlayState();
        InitBadgeRow(0,12,304,296,88,0,0);
        InitUiRow(0,140,136,28,32,0,0,1,0,104);
        InitUiRow(1,140,176,28,32,0,0,1,0,105);
        ModeGroup=stage;
        ModeIndex=4;
        SubmodeState=0;
        break;
    }
}
