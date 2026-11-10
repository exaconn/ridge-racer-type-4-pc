/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_key.h"
#include "psyq/libspu/spukeyx.h"

#define HW (((SpuRegisterStorage *)_spu_RXX)->halves)

extern u16 D_8010EE28[];

u32 SpuSetAnyVoice(s32 on_off_in, u32 voice_bit_in, s32 addr1, s32 addr2) {
    /* ASM_BARRIER: pin on_off to $t1 as in original register allocation */
    register s32 on_off asm("$9") = on_off_in;
    u32 voice_bit;
    u32 ret;

    {
        u16 *p;
        u32 cond;
        u32 hi;
        u32 lo;
        /* ASM_BARRIER: keep on_off copy before _spu_env load */
        __asm__ volatile("" : "+r"(on_off));
        cond = _spu_env & 1;
        /* ASM_BARRIER: keep _spu_env & 1 test before D_8010EE28 address load */
        __asm__ volatile("" : "+r"(cond));
        voice_bit = voice_bit_in;
        if (cond) {
            p = D_8010EE28;
        } else {
            p = HW;
        }
        hi = p[addr2];
        /* ASM_BARRIER: complete addr2 load before computing addr1 address */
        __asm__("" : "+r"(p) : "r"(hi));
        lo = p[addr1];
        hi = (hi & 0xFF) << 16;
        /* ASM_BARRIER: keep shift before switch constant load */
        __asm__ volatile("" : "+r"(hi), "+r"(lo));
        ret = lo | hi;
    }

    switch (on_off) {
    case 1:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (_spu_env & 1) {
            volatile u16 *p = D_8010EE28;
            p[addr1] |= voice_bit;
            p[addr2] |= (voice_bit >> 16) & 0xFF;
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            u16 *p = HW;
            p[addr1] |= voice_bit;
            p[addr2] |= (voice_bit >> 16) & 0xFF;
        }
        {
            u32 m = voice_bit & 0xFFFFFF;
            ret |= m;
        }
        break;
    case 0:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (_spu_env & 1) {
            volatile u16 *p = D_8010EE28;
            p[addr1] &= ~voice_bit;
            p[addr2] &= ~((voice_bit >> 16) & 0xFF);
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            u16 *p = HW;
            p[addr1] &= ~voice_bit;
            p[addr2] &= ~((voice_bit >> 16) & 0xFF);
        }
        {
            u32 m = ~(voice_bit & 0xFFFFFF);
            ret &= m;
        }
        break;
    case 8:
        /* ASM_BARRIER: recompute shifted indices per switch case */
        __asm__("" : "+r"(addr1), "+r"(addr2));
        if (_spu_env & 1) {
            volatile u16 *p = D_8010EE28;
            p[addr1] = voice_bit;
            p[addr2] = (voice_bit >> 16) & 0xFF;
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            u16 *p = HW;
            p[addr1] = voice_bit;
            p[addr2] = (voice_bit >> 16) & 0xFF;
        }
        ret = voice_bit & 0xFFFFFF;
        break;
    }

    return ret & 0xFFFFFF;
}
