#include "rr/widescreen.h"
#include "scratchpad.h"
#include "rr/hud_number.h"
#include "rr/sprite_emit.h"

/* Five-digit HUD counter: a 0/1 ten-thousands glyph, then the thousands,
 * hundreds, tens and units of the value, followed by draw mode 5. */
void DrawHudNumber5(s32 value_arg,s32 index) {
    s32 value=value_arg;
    s32 palette=0x7EFA;
    s32 thousands=value/1000;
    s32 digit_thousands=thousands%10;
    s32 hundreds=value/100;
    s32 tens=value/10;
    NumberTarget *target=&HudNumberTargets[index];
    GpuDmaTag *ordering=target->heads[DrawBufIndex];
    u8 *cursor=RR_GPU_SCRATCH->cursor;
    s32 x=WIDE_HUD_RIGHT(target->x);
    s32 y=target->y;
    s32 digit_hundreds=hundreds-thousands*10;
    s32 digit_tens=tens-hundreds*10;
    cursor=EmitHudGlyph12(ordering,cursor,x-8,y,value>=10000,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x,y,digit_thousands,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+8,y,digit_hundreds,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+16,y,digit_tens,palette);
    value-=tens*10;
    cursor=EmitHudGlyph12(ordering,cursor,x+24,y,value,palette);
    RR_GPU_SCRATCH->cursor=AddDrawModePrim(ordering,(GpuDrawModePacket *)cursor,5);
}
