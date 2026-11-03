/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -mhard-float */
#include "types.h"

typedef struct {
    short left;
    short right;
} SpuVolume;

typedef struct {
    unsigned int voice;
    unsigned int mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned int addr;
    unsigned int loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
} SpuVoiceAttr;

typedef struct {
    s16 value00;
    u16 age02;
    u16 value04;
    u16 value06;
    u8 pad08[0x1D - 8];
    s8 flags1D;
    s16 field1E;
    u8 pad20[0x2A - 0x20];
    s16 field2A;
    u8 pad2C[0x36 - 0x2C];
} SpuVoiceRow36;

extern u16 D_8010F8F8;
extern u16 D_8010F8FA;
extern u16 D_8010F8FC;
extern u16 D_8010F8FE;
extern u16 D_8010F900;
extern u16 D_8010F902;
extern SpuVoiceRow36 D_8010F908[];
extern u8 D_8010FE18[];
extern void (*D_8010FE30)(s32);
extern u16 D_8010FE38[];
extern void (*D_8010FFB8)(s32);
extern s32 D_8010FFBC;
extern s32 D_8010FFC0[];
extern u16 D_80110000;
extern u16 D_80110002;
extern s8 D_801100FC;
extern s8 D_80110130;
extern void func_80084674(SpuVoiceAttr *);
extern void func_800874F4(s32, s32);
extern void func_800877E4(s32, s32);
extern void func_80087814(s32, s32);
extern void func_800879D4(s32, u16 *);

void func_80087054(void);
void func_80087064(void);

void func_80087054(void) {
    s32 value = 2;
    /* ASM_BARRIER: preserve the original assembler temporary as the address base. */
    register s16 *base __asm__("$1") = (s16 *)0x80110000U;
    /* PAL RAM mapping: D_801100A0 is base plus 0xA0. */
    /* ASM_BARRIER: retain the address and value registers for the return-slot store. */
    __asm__("" : "=r"(base), "=r"(value) : "0"(base), "1"(value));
    base[(0xA0) / 2] = value;
}

void func_80087064(void) {
    SpuVoiceAttr attr;
    s32 v;
    s32 mask_all;
    s32 bit;
    s32 *p;
    u8 hi;

    D_8010FFBC = (D_8010FFBC + 1) & 0xF;
    *(s32 *)((u8 *)D_8010FFC0 + (D_8010FFBC << 2)) = 0;
    p = D_8010FFC0;

    v = 0;
    if (D_801100FC > 0) {
        do {
            func_800879D4(v, &D_8010F908[v].value06);
            if (D_8010F908[v].value06 == 0) {
                D_8010FFC0[D_8010FFBC] |= (1 << v);
            }
            v++;
        } while (v < D_801100FC);
    }

    if (D_80110130 == 0) {
        mask_all = -1;
        p = D_8010FFC0;
        for (v = 0; v < 15; v++) {
            mask_all &= p[v];
        }
        v = 0;
        if (D_801100FC > 0) {
            do {
                bit = 1 << v;
                if (mask_all & bit) {
                    if (D_8010F908[v].flags1D == 2) {
                        if (v < 16) {
                            hi = 0;
                        } else {
                            bit = 0;
                            hi = 1 << (v - 16);
                        }
                        func_800874F4(0, (hi << 16) | (short)bit);
                    }
                    D_8010F908[v].flags1D = 0;
                }
                v++;
            } while (v < D_801100FC);
        }
    }

    D_8010F8F8 &= ~D_80110000;
    D_8010F8FA &= ~D_80110002;

    for (v = 0; v < 24; v++) {
        if (D_8010F908[v].field1E != 0) {
            D_8010FFB8(v);
        }
        if (D_8010F908[v].field2A != 0) {
            D_8010FE30(v);
        }
    }

    for (v = 0; v < 24; v++) {
        attr.mask = 0;
        attr.voice = 1 << v;
        if (D_8010FE18[v] & 1) {
            attr.mask = 3;
            attr.volume.left = D_8010FE38[v * 8];
            attr.volume.right = D_8010FE38[v * 8 + 1];
        }
        if (D_8010FE18[v] & 4) {
            attr.mask |= 0x10;
            attr.pitch = D_8010FE38[v * 8 + 2];
        }
        if (D_8010FE18[v] & 8) {
            attr.mask |= 0x80;
            attr.addr = D_8010FE38[v * 8 + 3] << 3;
        }
        if (D_8010FE18[v] & 0x10) {
            attr.mask |= 0x60000;
            attr.adsr1 = D_8010FE38[v * 8 + 4];
            attr.adsr2 = D_8010FE38[v * 8 + 5];
        }
        if (attr.mask != 0) {
            func_80084674(&attr);
        }
        D_8010FE18[v] = 0;
    }

    func_80087814(0, ((u8)D_80110002 << 16) | D_80110000);
    func_80087814(1, ((u8)D_8010F8FA << 16) | D_8010F8F8);
    func_800877E4(8, ((u8)D_8010F8FE << 16) | D_8010F8FC);
    func_800874F4(8, ((u8)D_8010F902 << 16) | D_8010F900);

    D_80110000 = 0;
    D_80110002 = 0;
    D_8010F8F8 = 0;
    D_8010F8FA = 0;
    D_8010F900 = 0;
    D_8010F902 = 0;
}
