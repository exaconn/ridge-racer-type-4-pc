/* CC1_FLAGS: -fno-strength-reduce */
#include "rr/game_motion.h"
#include "game/gfx/motion4.h"
#include "rr/cpu_md_leaf.h"
void UpdateScrollTrack(MotionClock20 *arg0)
{
    MotionClock20 *clock;
    PresentationController *root;
    u32 tempo;
    /* ASM_BARRIER: reuse $v0 across disjoint MD and key index calculations. */
    register u32 keyGlyph asm("$2");
    u32 product;
    /* ASM_BARRIER: pin keyBase to $a0 for key lookup. */
    register ScrollKey4 *keyBase asm("$4");
    /* ASM_BARRIER: pin keyPointer to $v0 for key lookup. */
    register ScrollKey4 *keyPointer asm("$2");
    /* ASM_BARRIER: pin indexWord to $v0 for scaled index offset. */
    register u32 indexWord asm("$2");
    ScrollKey4 *keyBaseSecond;
    s32 channelIndex;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_a2;
    s32 temp_v1_2;
    s32 var_v0;
    u16 temp_v0_4;
    /* ASM_BARRIER: pin temp_hi to $a2 after last clock read. */
    register u32 temp_hi asm("$6");
    ScrollTrack40 *temp_a1;
    ScrollChannelCursor *temp_v1_3;
    ScrollChannelCursor *var_a0;
    clock = arg0;
    root = RaceContext.controller3D8;
    temp_a1 = &root->scroll;
    if (temp_a1->disabled != 0)
    {
        return;
    }
    switch (root->scroll.state)
    {
    case 0:
        if (clock->active == 0)
        {
            return;
        }
        root->scroll.state = 2;
    case 1:
        if (clock->active != 0)
        {
            temp_v0 = ((temp_a1->hold) - 1U);
            temp_a1->hold = temp_v0;
            if (temp_v0 >= 0)
            {
                break;
            }
            temp_a1->hold = 0;
        }
        if (temp_a1->hold != 0)
        {
            break;
        }
        temp_v0_2 = ((temp_a1->wait) - 1U);
        temp_a1->wait = temp_v0_2;
        if (temp_v0_2 >= 0)
        {
            break;
        }
        temp_a1->wait = 0;
        temp_a1->state = ((temp_a1->state) + 1U);
    case 2:
        /* GPR aliases below occupy successive phases. Pointer values keep their
        * full native width;
        only the scalar MD inputs/results are word sized. */
        keyGlyph=RaceContext.controller3D8->scale4C;
        keyGlyph<<=1;
        tempo=clock->tempo;
        RR_MD_MULT_S(tempo,keyGlyph);
        RR_MD_LO(product);
        keyGlyph=0x51EB851FU;
        RR_MD_MULT_U(product,keyGlyph);
        keyGlyph=temp_a1->channel[0].glyph;
        RR_MD_HI(temp_v1_2);
        temp_v1_2=(u32)temp_v1_2>>5;
        channelIndex=(keyGlyph!=0);
        if (temp_v1_2 > 0)
        {
            keyGlyph=0x1900;
            RR_MD_DIV_S(var_v0,keyGlyph,temp_v1_2);
            var_v0=(0U-var_v0);
        }
        else
        {
            var_v0 = -0xA0;
        }
        /* ASM_BARRIER: Retain publication before consuming the channel index. */
        temp_a1->step = var_v0;
        asm("" : "+r"(channelIndex) : "m"(temp_a1->step));
        product=channelIndex*4;
        indexWord=temp_a1->index;
        keyBase=temp_a1->keys;
        indexWord<<=2;
        keyPointer=(ScrollKey4*)(indexWord+(unsigned long)keyBase);
        keyGlyph=(u16)keyPointer->glyph;
        temp_v1_3=(ScrollChannelCursor*)((u8*)temp_a1+product);
        temp_v1_3->channel.offset=0;
        temp_v1_3->channel.glyph=keyGlyph;
        temp_a1->hold = (s32)((u32)temp_a1->keys[temp_a1->index].timing << 16) >> 24;
        tempo=clock->tempo;
        keyGlyph=RaceContext.controller3D8->scale4C;
        RR_MD_MULT_S(tempo,keyGlyph);
        RR_MD_LO(product);
        keyGlyph=0x51EB851FU;
        RR_MD_MULT_U(product,keyGlyph);
        indexWord=temp_a1->index;
        keyBaseSecond=temp_a1->keys;
        indexWord<<=2;
        keyPointer=(ScrollKey4*)(indexWord+(unsigned long)keyBaseSecond);
        /* This is the last clock read. GPR6 now holds a scalar HI result. */
        RR_MD_HI(temp_hi);
        tempo=(u8)keyPointer->timing;
        keyGlyph=temp_hi>>5;
        RR_MD_MULT_S(keyGlyph,tempo);
        RR_MD_LO(product);
        keyGlyph=0xAAAAAAABU;
        RR_MD_MULT_U(product,keyGlyph);
        temp_v0_3=(temp_a1->index+1U);
        temp_a1->index=temp_v0_3;
        RR_MD_HI(product);
        product=product>>3;
        temp_a1->wait=product;
        if (temp_a1->keys[temp_v0_3].glyph < 0)
        {
            temp_a1->index = 0;
        }
        temp_a1->state = 1;
        break;
    }
    var_a2 = 0;
    var_a0 = (ScrollChannelCursor *) temp_a1;
    do
    {
        if (var_a0->channel.glyph > 0)
        {
            temp_v0_4 = var_a0->channel.offset + ((u16) temp_a1->step);
            var_a0->channel.offset = temp_v0_4;
            if (((s16) temp_v0_4) < (-0x95F))
            {
                var_a0->channel.glyph = 0;
            }
        }
        var_a2 += 1;
        var_a0 = (ScrollChannelCursor *) (((u8 *) var_a0) + 4);
    }
    while (var_a2 < 2);
}
