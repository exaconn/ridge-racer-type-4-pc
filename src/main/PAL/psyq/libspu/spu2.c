/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "psyq/libspu/spu3.h"
#include "psyq/libspu/spu1.h"
#include "psyq/libspu/spu2.h"
#include "rr/spu_init.h"

struct H { char p[0x1A6]; volatile unsigned short f1A6; char q[2]; volatile unsigned short f1AA; };

extern int SpuDmaStartAddr;
extern int D_800A4830;

int _spu_t(int a0, ...);
u32 _spu_Fw(const u16 *a0, u32 a1);
u32 _spu_Fr(u16 *a0, u32 a1);
void _spu_FsetRXX(int a0, unsigned int a1, int a2);
s32 _spu_FsetRXXa(s32 index, s32 value);
int _spu_FgetRXXa(int a0, int a1);
void _spu_FsetPCR(int a0);

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((struct H *)_spu_RXX)
#define _spu_tsa (*(unsigned short (*))&_spu_tsa)

int _spu_t(int a0, ...) {
    unsigned *s0;
    unsigned v1;
    unsigned short x;
    /* ASM_BARRIER: Keep control value in $a2 across case 3 setup */
    register int val asm("$6");
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
            _spu_RXX->f1A6 = r_v0;
        }
        break;
    case 1:
        D_800A4828 = 0;
        v1 = 0;
        while (_spu_RXX->f1A6 != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->f1AA = (_spu_RXX->f1AA & 0xFFCF) | 0x20;
        break;
    case 0:
        D_800A4828 = 1;
        v1 = 0;
        while (_spu_RXX->f1A6 != _spu_tsa) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        _spu_RXX->f1AA |= 0x30;
        break;
    case 3:
        x = (D_800A4828 == 1) ? 0x30 : 0x20;
        v1 = 0;
        while ((_spu_RXX->f1AA & 0x30) != x) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        if (D_800A4828 == 1) {
            s0++;
            _spu_FsetDelayR();
            val = 0x01000000;
        } else {
            s0++;
            _spu_FsetDelayW();
            val = 0x01000000;
        }
        {
            /* ASM_BARRIER: Keep block count in $v1 */
            register unsigned r_v1 asm("$3");
            r_a0 = s0[-1];
            SpuDmaStartAddr = r_a0;
            r_a0 = s0[0];
            r_v1 = r_a0 >> 6;
            /* ASM_BARRIER: Equalize dependence depth so srl schedules before andi */
            __asm__("" : "=r"(r_v1) : "0"(r_v1));
            r_v1 += (r_a0 & 0x3F) != 0;
            D_800A4830 = r_v1;
            r_a0 = SpuDmaStartAddr;
            *DMA4_MADR = r_a0;
        }
        *DMA4_BCR = (D_800A4830 << 16) | 0x10;
        /* ASM_BARRIER: Keep low half of 0x01000201 after *DMA4_BCR store so it fills bne delay slot */
        __asm__ volatile("" : "=r"(val) : "0"(val));
        val |= 0x201;
        if (D_800A4828 == 1) val = 0x01000200;
        *DMA4_CHCR = val;
        break;
    }
    return 0;
}

#undef _spu_RXX
#undef _spu_tsa

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

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

void _spu_FsetRXX(int a0, unsigned int a1, int a2) {
    if (a2 == 0) {
        _spu_RXX[a0] = a1;
    } else {
        _spu_RXX[a0] = a1 >> _spu_mem_mode_plus;
    }
}

#undef _spu_RXX

s32 _spu_FsetRXXa(s32 index, s32 value) {
    u32 address = (u32)value;
    u32 units;
    /* ASM_BARRIER: Preserve the original copy of the shifted address in v1
     * and the shared return value in v0 across the selector branches. */
    register u32 copy __asm__("$3");
    register u32 result __asm__("$2");

    if (_spu_mem_mode && address % _spu_mem_mode_unit != 0) {
        address += _spu_mem_mode_unit;
        address &= ~_spu_mem_mode_unitM;
    }
    units = address >> _spu_mem_mode_plus;
    copy = units;
    if (index == -2) goto address_result;
    if (index != -1) goto store;
    result = copy & 0xFFFF;
    goto out;
address_result:
    result = address;
    goto out;
store:
    ((RrSpuRegisterStorage *)_spu_RXX)->halves[index] = units;
    result = address;
out:
    return (s32)result;
}

/* legacy typing of shared globals in this part of the unit */
#define _spu_RXX ((unsigned short *)_spu_RXX)

int _spu_FgetRXXa(int a0, int a1) {
    unsigned short v = _spu_RXX[a0];
    if (a1 == -1) return v;
    return v << _spu_mem_mode_plus;
}

#undef _spu_RXX

void _spu_FsetPCR(int a0) {
    *SPU_DPCR &= 0xFFF8FFFF;
    if (a0) {
        *SPU_DPCR |= 0x30000;
    } else {
        *SPU_DPCR |= 0x50000;
    }
}
