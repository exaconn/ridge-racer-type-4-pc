#include "types.h"
#include "rr_asm.h"

void func_80014F34(s32 *a0_in, char *a1_in) {
    /* ASM_BARRIER: keep a3 in $7 */
    register char *a3 __asm__("$7") = a1_in;
    /* ASM_BARRIER: keep a0 in $4 */
    register char *a0 __asm__("$4");
    /* ASM_BARRIER: keep t0 in $8 */
    register char *t0 __asm__("$8");
    /* ASM_BARRIER: keep t1 in $9 */
    register s32 *t1 __asm__("$9");
    /* ASM_BARRIER: keep v0 in $2 */
    register s32 v0 __asm__("$2");
    /* ASM_BARRIER: keep v1 in $3 */
    register s32 v1 __asm__("$3");
    /* ASM_BARRIER: keep a1 in $5 */
    register s32 a1 __asm__("$5");
    /* ASM_BARRIER: keep a2 in $6 */
    register s32 a2 __asm__("$6");

    v0 = a0_in[2];
    a0 = (char *)a0_in[0];
    /* ASM_BARRIER: order loads from a0_in before t0 addition */
    __asm__ volatile("" : "+r"(v0), "+r"(a0), "+r"(a3));
    t0 = (char *)v0 + 4;
    a1 = *(u16 *)((char *)v0 + 4);
    v1 = *(u16 *)(t0 + 2);
    v0 = *(u16 *)(t0 + 4);
    /* ASM_BARRIER: order 3 index loads before shifts */
    __asm__ volatile("" : "+r"(a1), "+r"(v1), "+r"(v0), "+r"(t0));
    a1 = (s32)(a0 + (a1 << 3));
    v1 = (s32)(a0 + (v1 << 3));
    v0 = (s32)(a0 + (v0 << 3));
    RR_GTE_RTPT_LDV(a1, v1, v0);
    RR_GTE14_NOP2();
    RR_GTE14_NCLIP();
    RR_GTE14_SWC2_SXYZ(a3 + 0x1A0);
    RR_GTE14_SWC2_MAC0(a3 + 0x74);

    v0 = *(u16 *)(t0 + 6);
    /* ASM_BARRIER: load-use delay before shift */
    __asm__ volatile("" : "+r"(v0));
    v0 <<= 3;
    a0 += v0;
    RR_GTE_RTPS_LDV(a0);
    RR_GTE14_AVSZ4();
    RR_GTE14_NCLIP();
    RR_GTE14_SWC2_SXY2(a3 + 0x1AC);
    RR_GTE14_SWC2_MAC0(a3 + 0x78);
    RR_GTE14_SWC2_OTZ(a3 + 0x70);

    v1 = *(s32 *)(a3 + 0x70);
    t1 = (s32 *)0x1F800000;
    if (v1 <= 0) {
        return;
    }
    /* ASM_BARRIER: place lui $t1, 0x1F80 in blez delay slot */
    __asm__ volatile("" : "+r"(t1), "+r"(v1));
    v0 = t1[0x60 / 4];
    /* ASM_BARRIER: load-use delay on shift count */
    __asm__ volatile("" : "+r"(v0));
    a1 = v1 >> v0;
    *(s32 *)(a3 + 0x70) = a1;
    if (a1 >= 0x1C0) {
        return;
    }

    v0 = *(s32 *)(a3 + 0x74);
    v1 = *(s32 *)(a3 + 0xBC);
    a2 = *(s32 *)(a3 + 0x78);
    a0 = (char *)a2;
    /* ASM_BARRIER: order 3 loads before xor instructions */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(a0));
    v0 ^= v1;
    a2 = (s32)a0 ^ v1;
    /* ASM_BARRIER: keep second xor in $a0 for bgtz delay slot */
    a0 = (char *)a2;
    if (v0 <= 0 && (s32)a0 >= 0) {
        return;
    }

    v0 = a1 << 2;
    v1 = *(s8 *)(t0 + 0x19);
    a0 = (char *)t1[1];
    /* ASM_BARRIER: order lb and lw before sll v1 */
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(a0));
    v1 <<= 2;
    v0 += (s32)a0;
    v1 += v0;
    a0 = (char *)t1[0];
    v0 = *(s32 *)((char *)v1 + 0x200);
    /* ASM_BARRIER: order lw a0 and lw v0 before sw v0 */
    __asm__ volatile("" : "+r"(a0), "+r"(v0));
    *(s32 *)a0 = v0;
    *(u8 *)(a0 + 3) = 9;

    v1 = *(s8 *)(t0 + 0x19);
    v0 = *(s32 *)(a3 + 0x70);
    a1 = t1[1];
    /* ASM_BARRIER: order reloads before shifts */
    __asm__ volatile("" : "+r"(v1), "+r"(v0), "+r"(a1));
    v1 <<= 2;
    v0 <<= 2;
    v0 += a1;
    v1 += v0;
    *(char **)((char *)v1 + 0x200) = a0;

    v1 = *(s8 *)(t0 + 0x19);
    v0 = *(s32 *)(a3 + 0x70);
    a1 = t1[1];
    /* ASM_BARRIER: order reloads before shifts */
    __asm__ volatile("" : "+r"(v1), "+r"(v0), "+r"(a1));
    v1 <<= 2;
    v0 <<= 2;
    v0 += a1;
    v1 += v0;
    *(u8 *)((char *)v1 + 0x203) = 0;

    a1 = *(s32 *)(a3 + 0x1A0);
    a2 = *(s32 *)(a3 + 0x1A4);
    v1 = *(s32 *)(a3 + 0x1A8);
    v0 = *(s32 *)(a3 + 0x1AC);
    /* ASM_BARRIER: order 4 SXY loads before 4 packet stores */
    __asm__ volatile("" : "+r"(a1), "+r"(a2), "+r"(v1), "+r"(v0));
    *(s32 *)(a0 + 0x8) = a1;
    *(s32 *)(a0 + 0x10) = a2;
    *(s32 *)(a0 + 0x18) = v1;
    *(s32 *)(a0 + 0x20) = v0;

    v0 = *(s32 *)(a3 + 0x170);
    /* ASM_BARRIER: load-use delay on 0x170(a3) */
    __asm__ volatile("" : "+r"(v0));
    *(s32 *)(a0 + 0x4) = v0;
    *(u8 *)(a0 + 0x7) = 0x2E;

    a1 = *(s32 *)(t0 + 0xC);
    v1 = *(s32 *)(t0 + 0x14);
    a2 = *(s32 *)(t0 + 0x10);
    /* ASM_BARRIER: order 3 UV/clut loads before sra */
    __asm__ volatile("" : "+r"(a1), "+r"(v1), "+r"(a2));
    v0 = v1 >> 16;
    /* ASM_BARRIER: order sra before 4 stores */
    __asm__ volatile("" : "+r"(v0));
    *(s32 *)(a0 + 0xC) = a1;
    *(s32 *)(a0 + 0x14) = a2;
    *(s32 *)(a0 + 0x1C) = v1;
    *(s32 *)(a0 + 0x24) = v0;
    a0 += 0x28;
    t1[0] = (s32)a0;
}
