/* CC1_FLAGS: -ffixed-12 */
/* Reserve the unused T4 to retain the original division temporary. */
#include "scratchpad.h"
#include "rr/hud_number.h"
#include "rr/sprite_emit.h"
void DrawHudNumber5(s32 value_arg,s32 index) {
    s32 value=value_arg;
    s32 palette=0x7EFA;
    s32 thousands=value/1000;
    s32 digit_thousands=thousands%10;
    s32 hundreds=value/100;
    s32 tens=value/10;
    NumberTarget *targets=HudNumberTargets;
    unsigned long row_offset=index*sizeof(NumberTarget);
    NumberTarget *target=(NumberTarget *)(row_offset+(unsigned long)targets);
    u32 *ordering=*(u32 **)(DrawBufIndex*sizeof(u32 *)+row_offset+(unsigned long)targets);
    u8 *incoming_cursor=((GpuScratchPointers *)SCRATCHPAD)->cursor;
    s32 x=target->x;
    s32 y=target->y;
    s32 digit_hundreds=hundreds-thousands*10;
    s32 digit_tens=tens-hundreds*10;
    u8 *cursor;
    cursor=EmitHudGlyph12(ordering,incoming_cursor,x-8,y,value>=10000,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x,y,digit_thousands,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+8,y,digit_hundreds,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+16,y,digit_tens,palette);
    {u32 *last_order=ordering;
    u8 *last_cursor=cursor;
    s32 last_x=x+24;
    s32 last_y=y;
    value-=tens*10;
    cursor=EmitHudGlyph12(last_order,last_cursor,last_x,last_y,value,palette);}
    ((GpuScratchPointers *)SCRATCHPAD)->cursor=AddDrawModePrim(ordering,(GpuDrawModePacket *)cursor,5);
}
