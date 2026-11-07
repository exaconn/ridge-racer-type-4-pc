/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/game_motion.h"

#include "rr/cpu_md_leaf.h"

void UpdateScrollTrack(RrMotionClock20 *arg0)
{
    register RrMotionClock20 *clock asm("$6");
    register RrMotionControllerPrefix *root asm("$4");
    register u32 tempo asm("$3");
    register u32 keyGlyph asm("$2");
    register s32 product asm("$3");
    register RrScrollKey4 *keyBase asm("$4"),*keyPointer asm("$2");
    register u32 indexWord asm("$2");
    register RrScrollKey4 *keyBaseSecond asm("$3");
    s32 channelIndex;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_a2;
    register s32 temp_v1_2 asm("$3");
    register s32 var_v0 asm("$2");
    u16 temp_v0_4;
    register u32 temp_hi asm("$6");
    register RrScrollTrack40 *temp_a1 asm("$5");
    register RrScrollChannelCursor *temp_v1_3 asm("$3");
    RrScrollChannelCursor *var_a0;
    clock = arg0;
    asm("" : "+r"(clock));
    root = MotionController;
    temp_a1 = &root->scroll;
    asm("" : : "r"(temp_a1));
    if (temp_a1->disabled != 0)
    {
        return;
    }
    temp_v1 = root->scroll.state;
    if (1 == temp_v1)
    {
        goto update;
    }
    if (temp_v1 < 2)
    {
        if (temp_v1 == 0)
        {
            goto start;
        }
        goto animate;
    }
    else
    {
        if (temp_v1 == 2)
        {
            goto initialize;
        }
        goto animate;
    }
    start:
    if (clock->active == 0)
    {
        return;
    }
    root->scroll.state = 2;
    update:
    if (clock->active != 0)
    {
        temp_v0 = (s32) (((u32) temp_a1->hold) - 1U);
        temp_a1->hold = temp_v0;
        if (temp_v0 >= 0)
        {
            goto animate;
        }
        temp_a1->hold = 0;
    }
    if (temp_a1->hold != 0)
    {
        goto animate;
    }
    temp_v0_2 = (s32) (((u32) temp_a1->wait) - 1U);
    temp_a1->wait = temp_v0_2;
    if (temp_v0_2 >= 0)
    {
        goto animate;
    }
    temp_a1->wait = 0;
    temp_a1->state = (s32) (((u32) temp_a1->state) + 1U);
    initialize:
    /* GPR aliases below occupy successive phases. Pointer values keep their
    * full native width;
    only the scalar MD inputs/results are word sized. */
    keyGlyph=MotionController->speed4C;
    keyGlyph<<=1;
    tempo=clock->tempo;
    RR_MD_MULT_S((s32)tempo,(s32)keyGlyph);
    RR_MD_LO(product);
    keyGlyph=0x51EB851FU;
    RR_MD_MULT_U((u32)product,keyGlyph);
    keyGlyph=(s32)temp_a1->channel[0].glyph;
    RR_MD_HI(temp_v1_2);
    temp_v1_2=(u32)temp_v1_2>>5;
    asm("" : : "r"(temp_v1_2));
    channelIndex=(keyGlyph!=0);
    if (temp_v1_2 > 0)
    {
        keyGlyph=0x1900;
        RR_MD_DIV_S(var_v0,keyGlyph,temp_v1_2);
        var_v0=(s32)(0U-(u32)var_v0);
    }
    else
    {
        var_v0 = -0xA0;
    }
    /* Retain publication before consuming the channel index. */
    temp_a1->step = var_v0;
    asm("" : "+r"(channelIndex) : "m"(temp_a1->step));
    product=channelIndex*4;
    indexWord=(u32)temp_a1->index;
    keyBase=temp_a1->keys;
    indexWord<<=2;
    keyPointer=(RrScrollKey4*)((unsigned long)indexWord+(unsigned long)keyBase);
    keyGlyph=(u16)keyPointer->glyph;
    temp_v1_3=(RrScrollChannelCursor*)((u8*)temp_a1+product);
    temp_v1_3->channel.offset=0;
    temp_v1_3->channel.glyph=keyGlyph;
    temp_a1->hold = ((s32)(((u32)temp_a1->keys[temp_a1->index].timing)<<16)) >> 24;
    tempo=clock->tempo;
    keyGlyph=MotionController->speed4C;
    RR_MD_MULT_S((s32)tempo,(s32)keyGlyph);
    RR_MD_LO(product);
    keyGlyph=0x51EB851FU;
    RR_MD_MULT_U((u32)product,keyGlyph);
    indexWord=(u32)temp_a1->index;
    keyBaseSecond=temp_a1->keys;
    indexWord<<=2;
    keyPointer=(RrScrollKey4*)((unsigned long)indexWord+(unsigned long)keyBaseSecond);
    /* This is the last clock read. GPR6 now holds a scalar HI result. */
    RR_MD_HI(temp_hi);
    tempo=(u8)keyPointer->timing;
    asm("" : "+r"(temp_hi) : "r"(tempo));
    keyGlyph=temp_hi>>5;
    RR_MD_MULT_S((s32)keyGlyph,(s32)tempo);
    RR_MD_LO(product);
    keyGlyph=0xAAAAAAABU;
    RR_MD_MULT_U((u32)product,keyGlyph);
    temp_v0_3=(s32)((u32)temp_a1->index+1U);
    temp_a1->index=temp_v0_3;
    RR_MD_HI(product);
    product=(u32)product>>3;
    temp_a1->wait=product;
    if (temp_a1->keys[temp_v0_3].glyph < 0)
    {
        temp_a1->index = 0;
    }
    temp_a1->state = 1;
    animate:
    var_a2 = 0;
    var_a0 = (RrScrollChannelCursor *) temp_a1;
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
        var_a0 = (RrScrollChannelCursor *) (((u8 *) var_a0) + 4);
    }
    while (var_a2 < 2);
}
