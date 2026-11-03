/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"

extern volatile u16 *D_800A47D8;
extern volatile u32 D_800A4870;
extern volatile u32 D_800A48A4;
extern volatile u16 D_8010EE28[];

u32 func_80087524(s32 on_off_in, u32 voice_bit_in, s32 addr1, s32 addr2) {
    /* ASM_BARRIER: pin on_off to $t1 as in original register allocation */
    register s32 on_off asm("$9") = on_off_in;
    /* ASM_BARRIER: pin voice_bit to $t0 as in original register allocation */
    register u32 voice_bit asm("$8");
    u32 ret;

    {
        /* ASM_BARRIER: pin base pointer in initial read block to $a0 */
        register volatile u16 *p asm("$4");
        u32 cond;
        u32 hi;
        u32 lo;
        /* ASM_BARRIER: keep on_off copy before D_800A48A4 load */
        __asm__ volatile("" : "+r"(on_off));
        cond = D_800A48A4 & 1;
        /* ASM_BARRIER: keep D_800A48A4 & 1 test before D_8010EE28 address load */
        __asm__ volatile("" : "+r"(cond));
        voice_bit = voice_bit_in;
        if (cond) {
            p = D_8010EE28;
        } else {
            p = D_800A47D8;
        }
        hi = ((u16 *)p)[addr2];
        /* ASM_BARRIER: complete addr2 load before computing addr1 address */
        __asm__("" : "+r"(p) : "r"(hi));
        lo = ((u16 *)p)[addr1];
        hi = (hi & 0xFF) << 16;
        /* ASM_BARRIER: keep shift before switch constant load */
        __asm__ volatile("" : "+r"(hi), "+r"(lo));
        ret = lo | hi;
    }

    switch (on_off) {
    case 1:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (D_800A48A4 & 1) {
            /* ASM_BARRIER: pin base pointer in case 1 to $a1 */
            register volatile u16 *p asm("$5") = D_8010EE28;
            p[addr1] |= voice_bit;
            p[addr2] |= (voice_bit >> 16) & 0xFF;
            D_800A4870 |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            /* ASM_BARRIER: pin base pointer in case 1 to $a1 */
            register volatile u16 *p asm("$5") = D_800A47D8;
            p[addr1] |= voice_bit;
            p[addr2] |= (voice_bit >> 16) & 0xFF;
        }
        {
            /* ASM_BARRIER: compute masked voice_bit in $v0 */
            register u32 m asm("$2") = voice_bit & 0xFFFFFF;
            ret |= m;
        }
        break;
    case 0:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (D_800A48A4 & 1) {
            /* ASM_BARRIER: pin base pointer in case 0 to $a1 */
            register volatile u16 *p asm("$5") = D_8010EE28;
            p[addr1] &= ~voice_bit;
            p[addr2] &= ~((voice_bit >> 16) & 0xFF);
            D_800A4870 |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            /* ASM_BARRIER: pin base pointer in case 0 to $a1 */
            register volatile u16 *p asm("$5") = D_800A47D8;
            p[addr1] &= ~voice_bit;
            p[addr2] &= ~((voice_bit >> 16) & 0xFF);
        }
        {
            /* ASM_BARRIER: compute inverted masked voice_bit in $v0 */
            register u32 m asm("$2") = ~(voice_bit & 0xFFFFFF);
            ret &= m;
        }
        break;
    case 8:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (D_800A48A4 & 1) {
            /* ASM_BARRIER: pin base pointer in case 8 to $a0 */
            register volatile u16 *p asm("$4") = D_8010EE28;
            p[addr1] = voice_bit;
            p[addr2] = (voice_bit >> 16) & 0xFF;
            D_800A4870 |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            /* ASM_BARRIER: pin base pointer in case 8 to $a0 */
            register volatile u16 *p asm("$4") = D_800A47D8;
            p[addr1] = voice_bit;
            p[addr2] = (voice_bit >> 16) & 0xFF;
        }
        ret = voice_bit & 0xFFFFFF;
        break;
    }

    return ret & 0xFFFFFF;
}
