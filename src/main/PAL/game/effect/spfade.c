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
    /* ASM_BARRIER: keep interpolated GTE IR1 result in $t6 */
    register s32 r_t6 __asm__("$14");
    /* ASM_BARRIER: keep 3*r_t6 in $t3 */
    register s32 r_t3 __asm__("$11");
    /* ASM_BARRIER: keep 0x70 constant in $t4 */
    register s32 r_t4 __asm__("$12");
    /* ASM_BARRIER: keep x offset in $t5 */
    register u32 r_t5 __asm__("$13");
    u32 r_t0;
    /* ASM_BARRIER: keep size constant 0x6C0020 in $t2 */
    register u32 r_t2 __asm__("$10");
    /* ASM_BARRIER: keep scratch pointer in $a1 */
    register GpuScratchPointers *scratch __asm__("$5");
    /* ASM_BARRIER: keep sprite command word in $a3 */
    register u32 r_a3 __asm__("$7");
    GpuSpriteCommand *packet;
    /* ASM_BARRIER: keep tag size 4 in $t1 */
    register u32 r_t1 __asm__("$9");
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
        /* ASM_BARRIER: mfc2 load delay slot before reading $t6 */
        RR_NOP_DEP_ON(r_t6);

        r_t3 = (r_t6 << 1) + r_t6;
        r_t4 = 0x70;
        r_t5 = r_t4 - r_t3;
        r_t0 = r_t6 + 0x88;
        r_t2 = 0x006C0020U;
        scratch = (GpuScratchPointers *)SCRATCHPAD;
        /* ASM_BARRIER: stage scratch pointer at 8001B938 */
        __asm__("" : "+r"(scratch));
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
        /* ASM_BARRIER: keep r_t5 live so r_a2 | r_t5 writes to $v1 */
        __asm__("" : : "r"(r_t5));
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
        /* ASM_BARRIER: keep 0x88 load in $v1 after packet 2 order stores */
        __asm__ ("" : : "r"(r_t5));
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
        /* ASM_BARRIER: keep r_t5 live so r_a0 | r_t5 writes to $v1 */
        __asm__("" : : "r"(r_t5));
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
        /* ASM_BARRIER: keep r_t5 live so r_a0 | r_t5 writes to $v1 */
        __asm__("" : : "r"(r_t5));
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
    /* ASM_BARRIER: captured packet cursor, independent of scratch pointer reloads. */
    register GpuSpriteCommand *packet __asm__("$5");
    /* ASM_BARRIER: table address reuses the dead offset temporary. */
    register SpritePreset *preset __asm__("$2");
    /* ASM_BARRIER: keep the packed color in the original word temporary. */
    register u32 hi __asm__("$3");
    u32 middle,offset;
    /* ASM_BARRIER: retain the scratch page across packet and ordering writes. */
    register GpuScratchPointers *scratch __asm__("$6");
    if(value!=0) {
        value = (value < 0 ? 0 : value) > 255 ? 255 : (value < 0 ? 0 : value);
        scratch=(GpuScratchPointers*)SCRATCHPAD;
        /* ASM_BARRIER: stage the scratch page before packing the RGB command. */
        __asm__("" : "+r"(scratch));
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
    /* ASM_BARRIER: keep interpolated width in $t1 */
    register u32 interp __asm__("$9");
    /* ASM_BARRIER: keep packet pointer in $a1 */
    register GpuTileCommand *packet __asm__("$5");
    u32 hi;
    u32 middle;
    /* ASM_BARRIER: keep scratch base in $a2 */
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
        /* ASM_BARRIER: keep interp ($t1) live so (interp | 0x20000) writes to $v0 */
        __asm__("" : : "r"(interp));
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
