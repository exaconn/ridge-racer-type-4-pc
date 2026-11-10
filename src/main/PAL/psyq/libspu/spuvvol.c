/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "psyq/libspu/spuvget.h"

int SsUtGetDetVVol(unsigned short a0, u16 *a1, u16 *a2);
s32 SsUtSetDetVVol(s32 voice, s32 left, s32 right);
int SsUtGetVVol();
s32 SsUtSetVVol(s32 input, s32 left, s32 right);

int SsUtGetDetVVol(unsigned short a0, u16 *a1, u16 *a2) { int r; if (a0 >= 0x18) r = -1; else { SpuGetVoiceVolume((short)a0, a1, a2); r = 0; } return r; }

s32 SsUtSetDetVVol(s32 voice, s32 left, s32 right) {
    s32 i;
    if ((u16)voice < 24) {
        /* ASM_BARRIER: Preserve the PAL page-address register used for the cache flag load. */
        register u8 *base __asm__("$4");
        register u32 flags;
        i = (s16)voice;
        _svm_sreg_dirty.voices[i].right = right;
        base = (u8 *)0x80110000U;
        base += i;
        flags = base[-0x1E8];
        _svm_sreg_dirty.voices[i].left = left;
        _svm_sreg_dirty.flags[i] = flags | 3;
        return 0;
    }
    return -1;
}

int SsUtGetVVol(a0, a1, a2) short a0; short *a1; short *a2; {
    short sp[2];
    if ((unsigned short)a0 < 0x18) {
        SpuGetVoiceVolume(a0, &sp[0], &sp[1]);
        *a1 = sp[0] / 129;
        *a2 = sp[1] / 129;
        return 0;
    }
    return -1;
}

s32 SsUtSetVVol(s32 input, s32 left, s32 right) {
    register s32 voice = input;
    if ((u16)voice < 24) {
        register s32 result;
        s32 right_value;
        register s32 right_scaled;
        register s32 index;
        register s32 offset;
        register s32 left_value;
        register s32 left_scaled;
        /* ASM_BARRIER: Preserve the PAL page-address register used for the cache flag load. */
        register u8 *base __asm__("$5");
        /* ASM_BARRIER: Reuse the flag-address register after its last pointer use. */
        u32 flags;
        /* ASM_BARRIER: Materialize the success value before narrowing and scaling the right volume. */
        __asm__("" : "=r"(result) : "0"(0) : "$3");
        right_value = (s16)right;
        right_scaled = right_value * 129;

        index = (s16)voice;
        offset = index * 16;
        ((SpuCachedVolumeEntry *)((u8 *)_svm_sreg_dirty.voices + offset))->right = right_scaled;
        left_value = (s16)left;

        left_scaled = left_value * 128;
        base = (u8 *)0x80110000U;
        base += index;
        flags = base[-0x1E8];
        left_scaled += left_value;
        ((SpuCachedVolumeEntry *)((u8 *)_svm_sreg_dirty.voices + offset))->left = left_scaled;
        _svm_sreg_dirty.flags[index] = flags | 3;
        return result;
    }
    return -1;
}
