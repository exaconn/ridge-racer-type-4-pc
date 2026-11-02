#include "types.h"
#include "rr_asm.h"

typedef struct {
    s32 m[8];
} Mat8;

typedef struct {
    s32 x, y, z, w;
} Vec4;

typedef struct {
    u8 pad00[0x10];
    s32 x10, y14, z18;
    u8 pad1C[0x50 - 0x1C];
    s32 rot50, rot54, rot58, pad5C;
    s32 x60, y64, z68, pad6C;
    s32 x70, y74, z78, pad7C;
    s16 ang80, ang82, ang84;
    u8 pad86[0xC8 - 0x86];
    Vec4 rotC8;
    u8 padD8[0x178 - 0xD8];
    s32 lat178, unk17C;
    u8 pad180[0x1A8 - 0x180];
    s16 unk1A8, unk1AA;
    u8 pad1AC[0x1E4 - 0x1AC];
    u16 flags1E4;
    s16 slot1E6;
    u8 pad1E8[0x23A - 0x1E8];
    s16 drag23A;
    u8 pad23C[0x2A2 - 0x23C];
    s16 unk2A2;
} Vehicle2D1B4;

typedef struct {
    s16 w0;
    u16 w2;
    u16 w4;
    u8 pad6[4];
    u8 bA;
} Bounds2D1B4;

typedef struct {
    u16 pad0;
    u16 flags2;
    u8 pad4[0x2C - 4];
} LightEntry2C;

s32 func_80013904(Vehicle2D1B4 *, s32);
s32 func_80014A98(Vec4 *, Mat8 *, s32, s32);
void func_80015F68(Vehicle2D1B4 *, Vec4 *, Mat8 *, Mat8 *, s32, s32, s32);
s32 func_80039494(Vehicle2D1B4 *);
void func_8005F0D0(s32, s32, s32, s32);
void func_8006E030(Mat8 *, s32);
void func_8006E09C(Mat8 *, s32);
void func_8006E108(Mat8 *, s32);
void func_8006EE3C(s32, Vec4 *, Mat8 *);
void func_800701C4(s32);
void func_80090914(void *, Mat8 *, Mat8 *);
void func_80090CA4(Mat8 *, Mat8 *);
void func_80090DB4(Mat8 *, Mat8 *);

extern u8 D_8009F02C[];
extern u8 D_8009F034[];
extern u8 D_8009F03C[];
extern u8 D_8009F044[];
extern s32 D_800ABEC4;
extern s16 D_800ABED0[];
extern u16 D_800ABEF0;
extern s32 D_800F2FD8;
extern Bounds2D1B4 *D_800F3A08[];
extern LightEntry2C *D_800F4D90;

void func_8002D1B4(Vehicle2D1B4 *arg0) {
    Mat8 sp20;
    Mat8 sp40;
    Mat8 sp60;
    Mat8 sp80;
    s16 spA0[4];
    Mat8 spA8[6];
    Vec4 sp168[5];
    s32 slot_half;
    s32 slot_idx;
    s32 s5_val;
    s32 v0_drag;
    s32 s4_val;
    s32 i;
    u8 a1_val;

    slot_half = arg0->slot1E6 >> 1;
    func_800701C4(slot_half);
    s5_val = func_80013904(arg0, func_80039494(arg0));
    func_8006E09C(&sp80, -arg0->rot54);
    func_8006E108(&sp40, -arg0->rot50);
    func_8006E030(&sp60, -arg0->rot58);
    func_80090CA4(&sp40, &sp60);
    func_80090CA4(&sp80, &sp40);
    func_80090914(D_800ABED0, &sp80, &sp60);
    RR_GTE_SETLLM(&sp60);
    func_8006E09C(&sp20, -arg0->rot54);
    func_80090DB4((Mat8 *)0x1F800028, &sp20);
    func_8006E108(&spA8[0], arg0->unk1A8 - arg0->rot50);
    func_8006E030(&sp60, arg0->unk1AA - arg0->rot58);
    func_80090CA4(&spA8[0], &sp60);
    func_80090DB4(&sp20, &spA8[0]);
    sp168[0] = arg0->rotC8;
    sp168[1].x = arg0->x70 + arg0->x60;
    sp168[1].y = arg0->y74 + arg0->y64;
    sp168[1].z = arg0->z78 + arg0->z68;
    func_8006E108(&spA8[1], -arg0->ang80);
    func_8006E030(&sp60, -arg0->ang84);
    func_80090CA4(&spA8[1], &sp60);
    func_80090DB4(&sp20, &spA8[1]);
    sp168[2].x = arg0->x10 + arg0->x60;
    sp168[2].y = arg0->y14 + arg0->y64;
    sp168[2].z = arg0->z18 + arg0->z68;
    {
        /* ASM_BARRIER: load unk17C into v0 before copying to s4_val */
        register s32 v0_tmp asm("v0") = arg0->unk17C;
        /* ASM_BARRIER: preserve v0 load before move to s4 */
        __asm__ volatile("" : "+r"(v0_tmp));
        s4_val = v0_tmp;
    }
    if (s4_val & 0x1000) {
        s4_val = -s4_val;
    }
    if (D_800F3A08[slot_half]->bA == 1) {
        func_8006E09C(&spA8[3], s4_val);
    } else {
        func_8006E108(&spA8[3], s4_val);
    }
    spA8[2] = sp40;
    func_80090DB4(&sp20, &spA8[2]);
    func_80090DB4(&spA8[2], &spA8[3]);
    func_8006E09C(&spA8[4], arg0->lat178 / 12);
    func_8006E108(&spA8[5], s4_val);
    func_80090DB4(&spA8[2], &spA8[4]);
    func_80090DB4(&spA8[4], &spA8[5]);
    RR_GTE_SETROT(&sp80);
    for (i = 0; i < 2; i++) {
        /* ASM_BARRIER: pin v0_w to v0 to match branch delay slot and load scheduling */
        register s32 v0_w asm("v0") = D_800F3A08[slot_half]->w0;
        if (i & 1) {
            v0_w = -v0_w;
        }
        spA0[0] = v0_w * 4;
        spA0[1] = D_800F3A08[slot_half]->w2 * 4;
        spA0[2] = -D_800F3A08[slot_half]->w4 * 4;
        RR_GTE_MVMVA_ROT_V0_NOCV(spA0);
        RR_GTE_STORE_IR(&sp168[i + 3]);
        sp168[i + 3].x += sp168[2].x;
        sp168[i + 3].y += sp168[2].y;
        sp168[i + 3].z += sp168[2].z;
    }
    s5_val = (s16)(D_800F4D90[s5_val].flags2 | D_800ABEF0);
    slot_idx = arg0->slot1E6 + D_800ABEC4;
    if (arg0->flags1E4 & 0x10) {
        v0_drag = arg0->unk2A2;
    } else {
        v0_drag = arg0->drag23A;
    }
    if (v0_drag > 0) {
        s5_val |= 2;
    }
    func_80015F68(arg0, &sp168[1], &spA8[1], &sp80, slot_idx, s5_val, 0);
    for (i = 0; i < 8; i++) {
        u8 s0_mode = D_8009F034[i];
        if (s0_mode != 1 || func_80014A98(&sp168[D_8009F03C[i]], &spA8[D_8009F044[i]], D_8009F02C[i], 0) != s0_mode) {
            func_8006EE3C(0x1F80007C, &sp168[D_8009F03C[i]], &spA8[D_8009F044[i]]);
            *(s32 *)0x1F800078 = 0;
            a1_val = 1;
            if (D_8009F02C[i] < D_800F2FD8) {
                a1_val = D_8009F02C[i];
            }
            func_8005F0D0(0x1F800000, a1_val, i != 0, 0);
        }
    }
}
