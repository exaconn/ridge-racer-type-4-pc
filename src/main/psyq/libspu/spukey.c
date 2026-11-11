#include "rr/spu_key.h"

void SpuSetKey(s32 mode, u32 mask) {
    u32 high;
    volatile u16 *pending;
    SpuRegisterBank *bank;

    mask &= 0xFFFFFF;
    high = mask >> 16;
    switch (mode) {
    case 1: {
        if (_spu_env & 1) {
            pending = _spu_RQ;
            pending[0] = mask;
            pending[1] = high;
            _spu_RQmask |= 1;
            _spu_RQvoice |= mask;
            if (pending[2] & mask) {
                pending[2] &= ~mask;
            }
            if (pending[3] & high) {
                pending[3] &= ~high;
            }
        } else {
            u32 active = _spu_keystat | mask;
            SpuRegisterBank *bank = _spu_RXX;
            bank->key_on[0] = mask;
            bank->key_on[1] = high;
            _spu_keystat = active;
        }
        break;
    }
    case 0: {
        if (_spu_env & 1) {
            pending = _spu_RQ;
            pending[2] = mask;
            pending[3] = high;
            _spu_RQmask |= 1;
            _spu_RQvoice &= ~mask;
            if (pending[0] & mask) {
                pending[0] &= ~mask;
            }
            if (pending[1] & high) {
                pending[1] &= ~high;
            }
        } else {
            bank = _spu_RXX;
            bank->key_off[0] = mask;
            bank->key_off[1] = high;
            _spu_keystat &= ~mask;
        }
        break;
    }
    }
}
