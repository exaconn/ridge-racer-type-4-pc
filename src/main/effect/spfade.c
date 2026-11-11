#include "rr/gpu_address.h"
#include "scratchpad.h"
#include "rr/gpu_scratch_types.h"
#include "rr_asm.h"
#include "rr/sprite_preset.h"
#include "game/effect/spfade.h"

void RampValueToLimit(unsigned short *a0, short *a1, int a2);
void RampValuePairTo4096(unsigned short *a0, unsigned short *a1, short *a2);
void RampValueTo4096(unsigned short *a0, short *a1);
void EmitTitleBeamFades(s32 intensity, s32 scale);
void EmitPresetSprite(s32 intensity,s32 index);
void EmitFadeTile(s32 intensity, s32 scale, u32 pos, s32 width);
void EmitTitleMaskSprites(s32 mask, s32 animation);

void RampValueToLimit(unsigned short *a0, short *a1, int a2) {
    int v = *a1;
    unsigned short u = *(unsigned short *)a1;
    if (v > 0) {
        *a0 += u;
        if ((short)*a0 >= a2) {
            *a0 = a2;
            *a1 = 0;
        }
    } else if (v < 0) {
        *a0 += u;
        if ((short)*a0 <= 0) {
            *a0 = 0;
            *a1 = 0;
        }
    }
}

void RampValuePairTo4096(unsigned short *a0, unsigned short *a1, short *a2) {
    int v = *a2;
    unsigned short u = *(unsigned short *)a2;
    if (v > 0) {
        *a1 += u;
        if ((short)*a1 >= 0x1000) {
            *a1 = 0x1000;
            *a2 = 0;
        }
    } else if (v < 0) {
        *a0 += u;
        if ((short)*a0 <= 0) {
            *a0 = 0;
            *a2 = 0;
        }
    }
}

void RampValueTo4096(unsigned short *a0, short *a1) {
    int v = *a1;
    unsigned short u = *(unsigned short *)a1;
    if (v > 0) {
        *a0 += u;
        if ((short)*a0 >= 0x1000) {
            *a0 = 0x1000;
            *a1 = 0;
        }
    } else if (v < 0) {
        *a0 += u;
        if ((short)*a0 <= 0) {
            *a0 = 0;
            *a1 = 0;
        }
    }
}

void EmitTitleBeamFades(s32 intensity, s32 scale) {
    s32 r_a2 = intensity;
    u32 r_a0;
    u32 middle;
    s32 r_t7;
    s32 r_t6;
    s32 r_t3;
    s32 r_t4;
    u32 r_t5;
    u32 r_t0;
    u32 r_t2;
    GpuScratchPointers *scratch;
    u32 r_a3;
    GpuSpriteCommand *packet;
    u32 r_t1;
    u32 r_v1;

    if (r_a2 != 0) {
        r_a2 = (r_a2 < 0 ? 0 : r_a2) > 255 ? 255 : (r_a2 < 0 ? 0 : r_a2);
        r_a0 = r_a2 * 0x10000U;
        middle = r_a2 * 0x100U;
        middle |= 0x66000000U;
        r_a0 |= middle;
        r_a2 |= r_a0;

        RR_GTE_MTC2_IR0(scale);
        r_t7 = 0;
        RR_GTE_MTC2_IR1(r_t7);
        r_t7 = 0x10;
        RR_GTE_CTC2_FCR(r_t7);
        RR_GTE_INTPL();
        RR_GTE_MFC2(r_t6, 9);
        RR_NOP_DEP_ON(r_t6);

        r_t3 = (r_t6 << 1) + r_t6;
        r_t4 = 0x70;
        r_t5 = r_t4 - r_t3;
        r_t0 = r_t6 + 0x88;
        r_t2 = 0x006C0020U;
        scratch = (GpuScratchPointers *)SCRATCHPAD;
        r_a3 = r_a2 | 0x64000000U;
        r_a2 = r_t0 << r_t7;
        r_v1 = r_a2 | r_t5;
        packet = (GpuSpriteCommand *)scratch->cursor;
        r_t0 = 0x1BA40000U;

        packet->words.color.word = r_a3;
        packet->words.position = r_v1;
        packet->words.uv = r_t0;
        packet->words.size = r_t2;
        r_v1 = (u32)(unsigned long)scratch->order;
        r_v1 = ((u32 *)r_v1)[1];
        r_t1 = 4;
        packet->words.tag.word = r_v1;
        r_v1 = 0x90;
        packet->words.tag.bytes.length = r_t1;
        r_a0 = (u32)(unsigned long)scratch->order;
        r_t5 = r_v1 - r_t6;
        ((u32 *)r_a0)[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        packet->words.color.word = r_a3;
        packet->words.position = r_a2 | r_t5;
        packet->words.uv = r_t0;
        packet->words.size = r_t2;
        packet->words.tag.word = scratch->order[1];
        packet->words.tag.bytes.length = r_t1;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        r_t5 = r_t6 + 0xB0;
        r_a2 |= r_t5;
        r_t5 = r_t4 - r_t6;
        r_v1 = 0x88;
        packet->words.uv = r_t0;
        r_t0 = r_v1 - r_t6;
        packet->words.color.word = r_a3;
        packet->words.position = r_a2;
        packet->words.size = r_t2;
        r_a0 = (u32)(unsigned long)scratch->order;
        r_v1 = ((u32 *)r_a0)[1];
        r_a2 = 0x1BA40020U;
        packet->words.tag.word = r_v1;
        packet->words.tag.bytes.length = r_t1;
        r_a0 = r_t0 << r_t7;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        packet->words.color.word = r_a3;
        packet->words.position = r_a0 | r_t5;
        packet->words.uv = r_a2;
        packet->words.size = r_t2;
        packet->words.tag.word = scratch->order[1];
        packet->words.tag.bytes.length = r_t1;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        r_t5 = r_t6 + 0x90;
        packet->words.color.word = r_a3;
        packet->words.position = r_a0 | r_t5;
        packet->words.uv = r_a2;
        packet->words.size = r_t2;
        packet->words.tag.word = scratch->order[1];
        packet->words.tag.bytes.length = r_t1;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        r_t5 = r_t3 + 0xB0;
        r_a0 |= r_t5;
        packet->words.color.word = r_a3;
        packet->words.position = r_a0;
        packet->words.uv = r_a2;
        packet->words.size = r_t2;
        packet->words.tag.word = scratch->order[1];
        packet->words.tag.bytes.length = r_t1;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        packet++;

        scratch->cursor = (u8 *)packet;
    }
}

void EmitPresetSprite(s32 intensity,s32 index) {
    s32 value=intensity;
    s32 selected=index;
    GpuSpriteCommand *packet;
    SpritePreset *preset;
    u32 hi;
    u32 middle,offset;
    GpuScratchPointers *scratch;
    if(value!=0) {
        value = (value < 0 ? 0 : value) > 255 ? 255 : (value < 0 ? 0 : value);
        scratch=(GpuScratchPointers*)SCRATCHPAD;
        hi=value * 0x10000U;
        middle=value * 0x100U;
        middle|=0x66000000U;
        hi|=middle;
        hi|=value;
        offset=selected*sizeof(SpritePreset);
        packet=(GpuSpriteCommand *)scratch->cursor;
        packet->words.color.word=hi;
        preset=(SpritePreset*)((u8*)PresetSpriteDescriptors+offset);
        packet->words.position=preset->position;
        packet->words.size=preset->size;
        packet->words.uv=preset->uv;
        packet->words.tag.word=scratch->order[1];
        packet->words.tag.bytes.length=4;
        scratch->order[1]=(u32)(unsigned long)packet;
        ((u8*)&scratch->order[1])[3]=0;
        scratch->cursor=(u8 *)(packet+1);
    }
}

void EmitFadeTile(s32 intensity, s32 scale, u32 pos, s32 width) {
    s32 value = intensity;
    u32 position = pos;
    u32 interp;
    GpuTileCommand *packet;
    u32 hi;
    u32 middle;
    GpuScratchPointers *scratch;

    if (value != 0 && scale != 0) {
        RR_GTE_MTC2_IR0(scale);
        RR_GTE_MTC2_IR1(width);
        RR_GTE_CMD_2NOP(0x4B98003D);
        RR_GTE_MFC2(interp, 9);
        value = (value < 0 ? 0 : value) > 255 ? 255 : (value < 0 ? 0 : value);
        scratch = (GpuScratchPointers *)SCRATCHPAD;
        hi = value * 0x10000U;
        middle = value * 0x100U;
        middle |= 0x62000000U;
        hi |= middle;
        value |= hi;
        packet = (GpuTileCommand *)scratch->cursor;
        packet->words.color.word = value;
        packet->words.position = position;
        packet->words.size = interp | 0x20000U;
        packet->words.tag.word = scratch->order[1];
        packet->words.tag.bytes.length = 3;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        scratch->cursor = (u8 *)(packet + 1);
    }
}

void EmitTitleMaskSprites(s32 mask, s32 animation)
{
    GpuSpriteCommand *packet = (GpuSpriteCommand *)((GpuScratchPointers *)SCRATCHPAD)->cursor;
    u32 uv = (((s32)(0U - (u32)animation) >> 3) & 3U) | 0x1BA4F0D0U;
    s16 *vertex = SpinningWheelBaseVerts;
    s32 remaining = RR_TITLE_MASK_SPRITE_COUNT;
    GpuScratchPointers *scratch;
    do {
        if (mask & 1) {
            packet->words.color.word = 0x65000000U;
            packet->words.position = *vertex | 0xA80000;
            packet->words.size = 0x100004;
            packet->words.uv = uv;
            scratch = (GpuScratchPointers *)SCRATCHPAD;
            packet->words.tag.word = scratch->order[1];
            packet->words.tag.bytes.length = 4;
            scratch->order[1] = GpuDmaEncodeAddress(packet);
            packet++;
            ((GpuDmaTag *)&scratch->order[1])->bytes.length = 0;
        }
        mask >>= 1;
        remaining--;
        vertex++;
    } while (remaining > 0);
    ((GpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)packet;
}
