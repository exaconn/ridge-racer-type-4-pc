/* CC1_FLAGS: -ffixed-12 */

/* Reserve the unused T4 register to retain the original division temporary. */
#include "rr/hud_number.h"

extern u8 *AddDrawModePrim(u32 *,u8 *,u16);

void DrawHudNumber5(s32 value_arg,s32 index) {
    register s32 value=value_arg;
    register s32 palette=0x7EFA;
    s32 thousands=value/1000;
    s32 digit_thousands=thousands%10;
    s32 hundreds=value/100;
    register s32 tens=value/10;
    register NumberTarget *targets=HudNumberTargets;
    unsigned long row_offset=(unsigned long)index*sizeof(NumberTarget);
    register NumberTarget *target=(NumberTarget *)(row_offset+(unsigned long)targets);
    register u32 *ordering=*(u32 **)((unsigned long)DrawBufIndex*sizeof(u32 *)+row_offset+(unsigned long)targets);
    u8 *incoming_cursor=((GpuScratchPointers *)0x1F800000U)->cursor;
    register s32 x=target->x;
    register s32 y=target->y;
    register s32 digit_hundreds=hundreds-thousands*10;
    register s32 digit_tens=tens-hundreds*10;
    u8 *cursor;
    cursor=EmitHudGlyph12(ordering,incoming_cursor,x-8,y,value>=10000,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x,y,digit_thousands,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+8,y,digit_hundreds,palette);
    cursor=EmitHudGlyph12(ordering,cursor,x+16,y,digit_tens,palette);
    {register u32 *last_order=ordering;
    register u8 *last_cursor=cursor;
    register s32 last_x=x+24;
    register s32 last_y=y;
    value-=tens*10;
    cursor=EmitHudGlyph12(last_order,last_cursor,last_x,last_y,value,palette);}
    ((GpuScratchPointers *)0x1F800000U)->cursor=AddDrawModePrim(ordering,cursor,5);
}
