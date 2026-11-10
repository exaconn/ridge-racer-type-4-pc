/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_key.h"
#include "psyq/libspu/spukeyx.h"

#define HW (((SpuRegisterStorage *)_spu_RXX)->halves)

extern volatile u16 D_8010EE28[];

u32 SpuSetAnyVoice(s32 on_off, u32 voice_bit, s32 addr1, s32 addr2) {
    u32 ret;

    if (_spu_env & 1) {
        ret = ((D_8010EE28[addr2] & 0xFF) << 16) | D_8010EE28[addr1];
    } else {
        volatile u16 *p = HW;
        ret = ((p[addr2] & 0xFF) << 16) | p[addr1];
    }

    switch (on_off) {
    case 1:
        if (_spu_env & 1) {
            D_8010EE28[addr1] |= voice_bit;
            D_8010EE28[addr2] |= (voice_bit >> 16) & 0xFF;
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            HW[addr1] |= voice_bit;
            HW[addr2] |= (voice_bit >> 16) & 0xFF;
        }
        ret |= voice_bit & 0xFFFFFF;
        break;
    case 0:
        if (_spu_env & 1) {
            D_8010EE28[addr1] &= ~voice_bit;
            D_8010EE28[addr2] &= ~((voice_bit >> 16) & 0xFF);
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            HW[addr1] &= ~voice_bit;
            HW[addr2] &= ~((voice_bit >> 16) & 0xFF);
        }
        ret &= ~(voice_bit & 0xFFFFFF);
        break;
    case 8:
        if (_spu_env & 1) {
            D_8010EE28[addr1] = voice_bit;
            D_8010EE28[addr2] = (voice_bit >> 16) & 0xFF;
            _spu_RQmask |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            HW[addr1] = voice_bit;
            HW[addr2] = (voice_bit >> 16) & 0xFF;
        }
        ret = voice_bit & 0xFFFFFF;
        break;
    }

    return ret & 0xFFFFFF;
}
