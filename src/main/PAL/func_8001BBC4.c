#include "rr/gpu_scratch_types.h"

void func_8001BBC4(s32 intensity, s32 scale, u32 pos, s32 width) {
    /* ASM_BARRIER: keep intensity in $t0 across GTE interpolation and clamping */
    register s32 value __asm__("$8") = intensity;
    /* ASM_BARRIER: keep pos in $t2 until packet write */
    register u32 position __asm__("$10") = pos;
    /* ASM_BARRIER: keep interpolated width in $t1 */
    register u32 interp __asm__("$9");
    s32 clamped;
    /* ASM_BARRIER: keep packet pointer in $a1 */
    register u8 *packet __asm__("$5");
    /* ASM_BARRIER: keep hi shift in $a0 */
    register u32 hi __asm__("$4");
    u32 middle;
    /* ASM_BARRIER: keep scratch base in $a2 */
    register RrGpuScratchPointers *scratch __asm__("$6");

    if (value != 0 && scale != 0) {
        /* ASM_BARRIER: load GTE IR0 with scale */
        __asm__ volatile ("mtc2 %0, $8" : : "r"(scale));
        /* ASM_BARRIER: load GTE IR1 with width */
        __asm__ volatile ("mtc2 %0, $9" : : "r"(width));
        /* ASM_BARRIER: GTE delay slots and GPF12 interpolation */
        __asm__ volatile ("nop\n\tnop\n\t.word 0x4B98003D");
        /* ASM_BARRIER: read interpolated GTE IR1 result */
        __asm__ volatile ("mfc2 %0, $9" : "=r"(interp));
        if (value >= 0) {
            clamped = value < 256;
            if (clamped) goto low;
            clamped = 255;
        } else {
low:
            clamped = value;
            if (value < 0) clamped = 0;
        }
        value = clamped;
        scratch = (RrGpuScratchPointers *)0x1F800000U;
        /* ASM_BARRIER: stage the scratch page before packing the RGB command */
        __asm__("" : "+r"(scratch));
        hi = (u32)value << 16;
        middle = (u32)value << 8;
        middle |= 0x62000000U;
        hi |= middle;
        value |= hi;
        packet = scratch->cursor;
        *(u32 *)(packet + 4) = value;
        *(u32 *)(packet + 8) = position;
        *(u32 *)(packet + 12) = interp | 0x20000U;
        /* ASM_BARRIER: keep interp ($t1) live so (interp | 0x20000) writes to $v0 */
        __asm__("" : : "r"(interp));
        *(u32 *)packet = scratch->order[1];
        packet[3] = 3;
        scratch->order[1] = (u32)(unsigned long)packet;
        ((u8 *)&scratch->order[1])[3] = 0;
        scratch->cursor = packet + 0x10;
    }
}
