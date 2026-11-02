#include "rr/gpu.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    u8 unk4[24];
    u8 unk1C[24];
    u16 unk34;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
} RrRow60;

struct O {
    char pad[0x70];
    u32 addr:24;
    u32 len:8;
};

extern RrSpritePacket *func_8001FCC8();
extern s32 func_80090424();
extern s32 func_800904F4();
extern u8 D_8009ED38[];
extern struct O *D_800ACC14;
extern RrRow60 *D_800FFC14;

#define ROW(i) ((RrRow60 *)((s32)D_800FFC14 + (i) * 0x3C))

void func_80040CB4(s32 a0) {
    RrSpritePacket *s4;
    s32 fp;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    u8 *sp1C;
    u16 s5;
    u16 sp20;
    s32 s2;
    s32 s0;
    s32 a3;
    s32 s1;
    s16 d34;
    s32 v1;

    s4 = *(RrSpritePacket **)0x1F800000;
    fp = ROW(a0)->unk38;
    sp1C = ROW(a0)->unk1C;
    sp10 = ROW(a0)->unk3A;
    s5 = ROW(a0)->unk36;
    /* ASM_BARRIER: keep fp and s7 increments before sp1C reload in outer loop latch */
    for (s2 = 0; *sp1C != 0; fp += 0xD, s2++, ({ __asm__("" : "=m"(*s4) : "r"(fp), "r"(s2 * 0x61)); })) {
        s0 = *sp1C - 0x20;
        sp1C++;
        if (s0 != 0) {
            s0 = D_8009ED38[s0 * 2] + (D_8009ED38[s0 * 2 + 1] << 5);
            a3 = 0;
            sp14 = (s0 % 21) * 12;
            sp18 = (s0 / 21) * 24;
            d34 = ROW(a0)->unk34;
            s1 = ROW(a0)->unk4[s2];
            {
                s32 ext = d34;
                if (ROW(a0)->unk3 >= s2) {
                    /* ASM_BARRIER: keep sign-extension of d34 before unk3 >= s2 branch */
                    __asm__("" : : "r"(ext));
                    if (d34 == 1) {
                        if (s2 & 1) {
                            s5 = 0x2638;
                            sp20 = 0x2639;
                        } else {
                            sp20 = 0x2638;
                            s5 = 0x2639;
                        }
                        if (func_80090424(s1 << 6) > 0) {
                            a3 = s1 * 4;
                        } else {
                            a3 = 0xFF - (s1 - 0x20) * 4;
                        }
                        if (s1 >= 0x40) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 == 3) {
                        sp20 = 0x2639;
                        s5 = 0x2638;
                        a3 = (func_800904F4(((s1 << 5) - s2 * 0x61) & 0xFFF) * 0x4A) / 4096 + 0x4A;
                        if (s1 >= 0x80) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 == 2) {
                        s5 = 0x2638;
                        a3 = (func_800904F4(((s1 << 5) - s2 * 0x61) & 0xFFF) * 0x4A) / 4096 + 0x4A;
                        if (s1 >= 0x80) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 >= 0) {
                        s0 = d34 >> 8;
                        v1 = func_80090424(ROW(a0)->unk0 << 6) / 256;
                        a3 = v1 + 0x80;
                        if (s0 - 1 == s2) {
                            a3 = v1 + 0xE4;
                            s5 = 0x2638;
                        } else {
                            s5 = ROW(a0)->unk36;
                            if (s1 < 0x15) {
                                a3 = (func_80090424((s1 * 0x555) / 20) * 0x94) / 4096;
                                ROW(a0)->unk4[s2]++;
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
            for (s0 = 0; s0 < (ROW(a0)->unk34 & 1) + 1; s0++) {
                s4->len = 4;
                s4->code = 0x64;
                s4->x = fp;
                s4->y = sp10;
                s4->w = 0xC;
                s4->h = 0x18;
                s4->u = sp14;
                s4->v = sp18;
                if (s0 == 1) {
                    s4->clut = sp20;
                    a3 = 0x94 - a3;
                } else {
                    s4->clut = s5;
                }
                s4->r = a3;
                s4->g = a3;
                s4->b = a3;
                s4->code |= 2;
                s4->addr = D_800ACC14->addr;
                D_800ACC14->addr = (u32)s4;
                s4++;
                /* ASM_BARRIER: count one more reg in inner loop so loop_optimize does not hoist 0x64 */
                __asm__("" : "=r"(v1));
            }
        }
    }
    *(RrSpritePacket **)0x1F800000 = func_8001FCC8((char *)D_800ACC14 + 0x70, s4, 0x2E);
}
