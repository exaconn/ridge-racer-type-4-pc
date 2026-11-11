#include "rr/spu.h"
#include "psyq/libspu/spu3.h"
#include "psyq/libspu/spu1.h"
#include "psyq/libspu/spu2.h"

extern s32 DeliverEvent(u32,s32);

void _spu_Fr_(int a0, int a1, int a2);

void _spu_FiDMA(void) {
    u32 i;
    SpuRegisterBank *bank;
    if (_spu_dmaIsRead == 0) {
        _spu_Fw1ts();
    }
    bank = _spu_RXX;
    bank->spu_control &= 0xFFCF;
    i = 0;
    while (bank->spu_control & 0x30) {
        if (++i > 0xF00) {
            break;
        }
    }
    if (_spu_transferCallback) {
        _spu_transferCallback();
    } else {
        DeliverEvent(0xF0000009U, 0x20);
    }
}

void _spu_Fr_(int a0, int a1, int a2) {
    _spu_RXX->transfer_address = a1;
    _spu_Fw1ts();
    _spu_RXX->spu_control |= 0x30;
    _spu_Fw1ts();
    _spu_FsetDelayR();
    *DMA4_MADR = a0;
    *DMA4_BCR = (a2 * 0x10000U) | 0x10;
    _spu_dmaIsRead = 1;
    *DMA4_CHCR = 0x1000200;
}
