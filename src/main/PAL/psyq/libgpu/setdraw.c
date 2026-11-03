/* CC1_FLAGS: -mno-split-addresses */
#include "rr/draw_environment.h"

typedef char RrDrawEnvironmentAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

void func_80093C78(u32 *arg0, RrFrameEnvironment *arg1);
void func_80093E70(u32 *arg0, RrFrameEnvironment *arg1);

void func_80093C78(u32 *arg0, RrFrameEnvironment *arg1) {
    register RrFrameEnvironment *env=arg1;
    register u32 *output=arg0;
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register s32 high asm("$2");
    RrGpuRectangle rect;
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register u32 color_offset asm("$6"),xy_offset asm("$7"),size_offset;
    register u32 blue,green;
    s32 word_count;
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register s32 signed_side asm("$3");
    register s32 over;
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register s32 raw_limit asm("$3");
    register s32 raw_side;
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register s32 width asm("$2");
    /* ASM_BARRIER: retain this arithmetic or packet-address phase. */
    register s32 height asm("$2");

    output[1] = func_80094100(env->clip.x, env->clip.y);
    output[2] = func_80094198((s16) ((((u16)env->clip.w) + (u16) env->clip.x) - 1), (s16) (((u16) env->clip.y + ((u16)env->clip.h)) - 1));
    output[3] = func_80094230(env->offset_x, env->offset_y);
    output[4] = func_800940E0(env->draw_display, env->dither, ((u16)env->page));
    output[5] = func_8009424C(&env->window);
    output[6] = 0xE6000000;
    word_count = 7;
    if (env->rgb24 != 0) {
        rect.rect.x = (u16) env->clip.x;
        rect.rect.y = (u16) env->clip.y;
        raw_side = ((u16)env->clip.w);
        rect.rect.w = raw_side;
        rect.rect.h = ((u16)env->clip.h);
        /* ASM_BARRIER: finish the four halfword copies before sign extraction. */
        __asm__("" : : "m"(rect.rect.h));
        high=(s32)((u32)raw_side<<16);
        signed_side=high>>16;
        width=0;
        if(signed_side>=0) {
            over=(D_800ABAC4-1)<signed_side;
            raw_limit=(u16)D_800ABAC4;
            if(over)width=raw_limit-1;
            else width=raw_side;
        }
        rect.rect.w = width;
        signed_side=rect.rect.h;
        raw_side=(u16)rect.rect.h;
        if(signed_side>=0) {
            over=(D_800ABAC6-1)<signed_side;
            raw_limit=(u16)D_800ABAC6;
            if(over)height=raw_limit-1;
            else height=raw_side;
        }else height=0;
        color_offset=(u32)word_count++*4U;
        xy_offset=(u32)word_count++*4U;
        size_offset=(u32)word_count++*4U;

        rect.rect.h = height;
        rect.rect.x -= (u16) env->offset_x;
        /* ASM_BARRIER: prepare the color destination before the Y/command phase. */
        __asm__("" : : "m"(output[color_offset/4U]));
        rect.rect.y -= (u16) env->offset_y;
        blue=(u32)env->b<<16;
        green=((u32)env->g<<8)|0x60000000U;

        output[color_offset/4U] = blue | green | env->r;
        output[xy_offset/4U] = rect.words[0];
        output[size_offset/4U] = rect.words[1];
    }
    ((u8*)output)[3] = (s8) (word_count - 1);
}

void func_80093E70(u32 *arg0, RrFrameEnvironment *arg1) {
    register RrFrameEnvironment *env=arg1;
    register u32 *output=arg0;
    /* ASM_BARRIER: retain the original width sign-extraction register. */
    register s32 high asm("$2");
    RrGpuRectangle rect;
    /* ASM_BARRIER: retain packet-offset and RGB arithmetic registers. */
    register u32 color_offset,xy_offset asm("$7"),size_offset;
    register u32 blue asm("$2"),green asm("$3");
    s32 word_count;
    /* ASM_BARRIER: retain signed/unsigned clamp phases in their registers. */
    register s32 signed_side asm("$3");
    register s32 over;
    /* ASM_BARRIER: retain the unsigned limit reload register. */
    register s32 raw_limit asm("$3");
    register s32 raw_side;
    /* ASM_BARRIER: retain the width/height result registers. */
    register s32 width asm("$2");
    register s32 height asm("$2");

    output[1] = func_80094100(env->clip.x, env->clip.y);
    output[2] = func_80094198((s16) ((((u16)env->clip.w) + (u16) env->clip.x) - 1), (s16) (((u16) env->clip.y + ((u16)env->clip.h)) - 1));
    output[3] = func_80094230(env->offset_x, env->offset_y);
    output[4] = func_800940E0(env->draw_display, env->dither, ((u16)env->page));
    output[5] = func_8009424C(&env->window);
    output[6] = 0xE6000000;
    word_count = 7;
    if (env->rgb24 != 0) {
        rect.rect.x = (u16) env->clip.x;
        rect.rect.y = (u16) env->clip.y;
        raw_side = ((u16)env->clip.w);
        rect.rect.w = raw_side;
        rect.rect.h = ((u16)env->clip.h);
        /* ASM_BARRIER: finish the four halfword copies before sign extraction. */
        __asm__("" : : "m"(((u8*)&rect.rect.h)[0]));
        high=(s32)((u32)raw_side<<16);
        signed_side=high>>16;
        width=0;
        if(signed_side>=0) {
            over=(D_800ABAC4-1)<signed_side;
            raw_limit=(u16)D_800ABAC4;
            if(over)width=raw_limit-1;
            else width=raw_side;
        }
        rect.rect.w = width;
        signed_side=rect.rect.h;
        raw_side=(u16)rect.rect.h;
        if(signed_side>=0) {
            over=(D_800ABAC6-1)<signed_side;
            raw_limit=(u16)D_800ABAC6;
            if(over)height=raw_limit-1;
            else height=raw_side;
        }else height=0;
        rect.rect.h=height;
        color_offset=(u32)word_count*4U;
        if(((u16)rect.rect.x&63U)||((u16)rect.rect.w&63U)) {
            word_count++;
            xy_offset=(u32)word_count++*4U;
            size_offset=(u32)word_count++*4U;
            rect.rect.x-=(u16)env->offset_x;
            /* ASM_BARRIER: prepare the color address before Y/command arithmetic. */
            __asm__("" : : "m"((*(u8 *)((unsigned long)color_offset+(unsigned long)output))));
            {
                register u32 y=(u16)rect.rect.y;
                register u32 offset_y=(u16)env->offset_y;
                register u32 command;
                command=0x60000000U;
                rect.rect.y=y-offset_y;
                blue=(u32)env->b<<16;
                green=((u32)env->g<<8)|command;
                (*(u32 *)((unsigned long)color_offset+(unsigned long)output))=blue|green|env->r;
                output[xy_offset/4U]=rect.words[0];
                output[size_offset/4U]=rect.words[1];
            }
        } else {
            register u32 fast_color,fast_xy,fast_size;
            /* Modulo32 byte offset: masking before the shift also keeps this
             * preparation independent of the extended branch's offset. */
            fast_color=((u32)word_count++&0x3FFFFFFFU)<<2;
            fast_xy=(u32)word_count++*4U;
            fast_size=(u32)word_count++*4U;
            /* ASM_BARRIER: prepare the fast color address before its command. */
            __asm__("" : : "m"((*(u8 *)((unsigned long)fast_color+(unsigned long)output))));
            {
                register u32 command=0x02000000U;
                blue=(u32)env->b<<16;
                green=((u32)env->g<<8)|command;
                (*(u32 *)((unsigned long)fast_color+(unsigned long)output))=blue|green|env->r;
                output[fast_xy/4U]=rect.words[0];
                output[fast_size/4U]=rect.words[1];
            }
        }
    }
    ((u8*)output)[3] = (s8) (word_count - 1);
}
