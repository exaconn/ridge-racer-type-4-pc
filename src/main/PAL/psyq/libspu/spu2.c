/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "psyq/libspu/spu3.h"
#include "psyq/libspu/spu1.h"
#include "psyq/libspu/spu2.h"

struct H { char p[0x1A6]; volatile unsigned short f1A6; char q[2]; volatile unsigned short f1AA; };

extern int D_800A482C;
extern int D_800A4830;
extern unsigned int *D_800A47E8;

int func_8007F068(int a0, ...);
u32 func_8007F2E8(const u16 *a0, u32 a1);
u32 func_8007F36C(u16 *a0, u32 a1);
void func_8007F3D0(int a0, unsigned int a1, int a2);
s32 func_8007F414(s32 index, s32 value);
int func_8007F4B8(int a0, int a1);
void func_8007F4F4(int a0);

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((struct H *)D_800A47D8)
#define D_800A47F0 (*(unsigned short (*))&D_800A47F0)

int func_8007F068(int a0, ...) {
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
            r_v0 = r_a0 >> D_800A4800;
            D_800A47F0 = r_v0;
            D_800A47D8->f1A6 = r_v0;
        }
        break;
    case 1:
        D_800A4828 = 0;
        v1 = 0;
        while (D_800A47D8->f1A6 != D_800A47F0) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        D_800A47D8->f1AA = (D_800A47D8->f1AA & 0xFFCF) | 0x20;
        break;
    case 0:
        D_800A4828 = 1;
        v1 = 0;
        while (D_800A47D8->f1A6 != D_800A47F0) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        D_800A47D8->f1AA |= 0x30;
        break;
    case 3:
        x = (D_800A4828 == 1) ? 0x30 : 0x20;
        v1 = 0;
        while ((D_800A47D8->f1AA & 0x30) != x) {
            v1++;
            if (v1 >= 0xF01) return -2;
        }
        if (D_800A4828 == 1) {
            s0++;
            func_8007F574();
            val = 0x01000000;
        } else {
            s0++;
            func_8007F54C();
            val = 0x01000000;
        }
        {
            /* ASM_BARRIER: Keep block count in $v1 */
            register unsigned r_v1 asm("$3");
            r_a0 = s0[-1];
            D_800A482C = r_a0;
            r_a0 = s0[0];
            r_v1 = r_a0 >> 6;
            /* ASM_BARRIER: Equalize dependence depth so srl schedules before andi */
            __asm__("" : "=r"(r_v1) : "0"(r_v1));
            r_v1 += (r_a0 & 0x3F) != 0;
            D_800A4830 = r_v1;
            r_a0 = D_800A482C;
            *D_800A47DC = r_a0;
        }
        *D_800A47E0 = (D_800A4830 << 16) | 0x10;
        /* ASM_BARRIER: Keep low half of 0x01000201 after *D_800A47E0 store so it fills bne delay slot */
        __asm__ volatile("" : "=r"(val) : "0"(val));
        val |= 0x201;
        if (D_800A4828 == 1) val = 0x01000200;
        *D_800A47E4 = val;
        break;
    }
    return 0;
}

#undef D_800A47D8
#undef D_800A47F0

u32 func_8007F2E8(const u16 *a0, u32 a1) {
    if (D_800A47F4 == 0) {
        func_8007F068(2, (u32)D_800A47F0 << D_800A4800);
        func_8007F068(1);
        func_8007F068(3, a0, a1);
    } else {
        func_8007ED44(a0, a1);
    }
    return a1;
}

u32 func_8007F36C(u16 *a0, u32 a1) {
    func_8007F068(2, (u32)D_800A47F0 << D_800A4800);
    func_8007F068(0);
    func_8007F068(3, a0, a1);
    return a1;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((unsigned short *)D_800A47D8)

void func_8007F3D0(int a0, unsigned int a1, int a2) {
    if (a2 == 0) {
        D_800A47D8[a0] = a1;
    } else {
        D_800A47D8[a0] = a1 >> D_800A4800;
    }
}

#undef D_800A47D8

s32 func_8007F414(s32 index, s32 value) {
    u32 address = (u32)value;
    u32 units;
    /* ASM_BARRIER: Preserve the original copy of the shifted address in v1
     * and the shared return value in v0 across the selector branches. */
    register u32 copy __asm__("$3");
    register u32 result __asm__("$2");

    if (D_800A47FC && address % D_800A4804 != 0) {
        address += D_800A4804;
        address &= ~D_800A4808;
    }
    units = address >> D_800A4800;
    copy = units;
    if (index == -2) goto address_result;
    if (index != -1) goto store;
    result = copy & 0xFFFF;
    goto out;
address_result:
    result = address;
    goto out;
store:
    ((RrSpuRegisterStorage *)D_800A47D8)->halves[index] = units;
    result = address;
out:
    return (s32)result;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A47D8 ((unsigned short *)D_800A47D8)

int func_8007F4B8(int a0, int a1) {
    unsigned short v = D_800A47D8[a0];
    if (a1 == -1) return v;
    return v << D_800A4800;
}

#undef D_800A47D8

void func_8007F4F4(int a0) {
    *D_800A47E8 &= 0xFFF8FFFF;
    if (a0) {
        *D_800A47E8 |= 0x30000;
    } else {
        *D_800A47E8 |= 0x50000;
    }
}
