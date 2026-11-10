/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "psyq/libspu/spu3.h"
#include "psyq/libspu/spu1.h"
#include "psyq/libspu/spu2.h"
#include "rr/spu_init.h"

extern int SpuDmaStartAddr;
extern int _spu_dma_block_count;

int _spu_t(int a0, ...);
u32 _spu_Fw(const u16 *a0, u32 a1);
u32 _spu_Fr(u16 *a0, u32 a1);
void _spu_FsetRXX(int a0, unsigned int a1, int a2);
s32 _spu_FsetRXXa(s32 index, s32 value);
int _spu_FgetRXXa(int a0, int a1);
void _spu_FsetPCR(int a0);

int _spu_t(int a0, ...) {
    unsigned *s0;
    unsigned v1;
    unsigned short x;
    int val;
    /* ASM_BARRIER: Keep temporary values in $a0 in case 2 and case 3 tail */
    register unsigned r_a0 asm("$4");
    s0 = (unsigned *)(&a0 + 1);
    switch (a0) {
    case 2:
        {
            /* ASM_BARRIER: Keep shifted transfer address in $v0 */
            register unsigned r_v0 asm("$2");
            r_a0 = *s0;
            r_v0 = r_a0 >> _spu_mem_mode_plus;
            _spu_tsa = r_v0;
            _spu_RXX->transfer_address = r_v0;
        }
        break;
    case 1:
        _spu_dmaIsRead = 0;
        v1 = 0;
        while (_spu_RXX->transfer_address != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->spu_control = (_spu_RXX->spu_control & 0xFFCF) | 0x20;
        break;
    case 0:
        _spu_dmaIsRead = 1;
        v1 = 0;
        while (_spu_RXX->transfer_address != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->spu_control |= 0x30;
        break;
    case 3:
        x = (_spu_dmaIsRead == 1) ? 0x30 : 0x20;
        v1 = 0;
        while ((_spu_RXX->spu_control & 0x30) != x) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        if (_spu_dmaIsRead == 1) {
            s0++;
            _spu_FsetDelayR();
            val = 0x01000000;
        } else {
            s0++;
            _spu_FsetDelayW();
            val = 0x01000000;
        }
        {
            unsigned r_v1;
            r_a0 = s0[-1];
            SpuDmaStartAddr = r_a0;
            r_a0 = s0[0];
            r_v1 = r_a0 >> 6;
            r_v1 += (r_a0 & 0x3F) != 0;
            _spu_dma_block_count = r_v1;
            r_a0 = SpuDmaStartAddr;
            *DMA4_MADR = r_a0;
        }
        *DMA4_BCR = (_spu_dma_block_count << 16) | 0x10;
        val |= 0x201;
        if (_spu_dmaIsRead == 1) val = 0x01000200;
        *DMA4_CHCR = val;
        break;
    }
    return 0;
}

u32 _spu_Fw(const u16 *a0, u32 a1) {
    if (_spu_transMode == 0) {
        _spu_t(2, (u32)_spu_tsa << _spu_mem_mode_plus);
        _spu_t(1);
        _spu_t(3, a0, a1);
    } else {
        _spu_FwriteByIO(a0, a1);
    }
    return a1;
}

u32 _spu_Fr(u16 *a0, u32 a1) {
    _spu_t(2, (u32)_spu_tsa << _spu_mem_mode_plus);
    _spu_t(0);
    _spu_t(3, a0, a1);
    return a1;
}

void _spu_FsetRXX(int a0, unsigned int a1, int a2) {
    if (a2 == 0) {
        ((SpuRegisterStorage *)_spu_RXX)->halves[a0] = a1;
    } else {
        ((SpuRegisterStorage *)_spu_RXX)->halves[a0] = a1 >> _spu_mem_mode_plus;
    }
}

s32 _spu_FsetRXXa(s32 index, s32 value) {
    u32 address = value;
    u32 units;
    u16 half;
    /* ASM_BARRIER: Preserve the shared return value in v0 across the selector branches. */
    register u32 result __asm__("$2");

    if (_spu_mem_mode && address % _spu_mem_mode_unit != 0) {
        address += _spu_mem_mode_unit;
        address &= ~_spu_mem_mode_unitM;
    }
    units = address >> _spu_mem_mode_plus;
    half = units;
    switch (index) {
    case -1:
        result = half;
        break;
    case -2:
        result = address;
        break;
    default:
        ((SpuRegisterStorage *)_spu_RXX)->halves[index] = units;
        result = address;
        break;
    }
    return result;
}

int _spu_FgetRXXa(int a0, int a1) {
    unsigned short v = ((SpuRegisterStorage *)_spu_RXX)->halves[a0];
    if (a1 == -1) return v;
    return v << _spu_mem_mode_plus;
}

void _spu_FsetPCR(int a0) {
    *SPU_DPCR &= 0xFFF8FFFF;
    if (a0) {
        *SPU_DPCR |= 0x30000;
    } else {
        *SPU_DPCR |= 0x50000;
    }
}
