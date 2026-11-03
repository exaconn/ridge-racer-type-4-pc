#include "types.h"
#include "rr/vehicle_drive.h"
#include "rr_asm.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/track_projection.h"

#define I(p, o) (*(s32 *)((char *)(p) + (o)))
#define H(p, o) (*(s16 *)((char *)(p) + (o)))
#define U(p, o) (*(u16 *)((char *)(p) + (o)))

typedef struct { s16 vx, vy, vz, pad; } w3t1_SV;

typedef struct { s32 m[8]; w3t1_SV sv; w3t1_SV out; w3t1_SV rot; } w3t1_F;

void func_80048A6C(char *a0);

void func_800486C4(RrDriveVehicleView *arg0) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s16 temp_a0_3;
    /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
    register s16 temp_v0_3 __asm__("$2");
    /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
    register s32 var_s0 __asm__("$16");
    s16 var_v1_2;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v1_3;
    s32 var_a0_2;
    s32 var_a0_3;
    u16 temp_a0;
    u16 temp_v1;
    /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
    register RrDriveControlView *temp_s1 __asm__("$17");

    temp_s1 = &arg0->control1F4;
    arg0->previous1D6 = (u16) arg0->heading1D4;
    temp_a1 = func_800924D4(arg0->control1F4.directionX30, arg0->control1F4.directionZ34);
    temp_v1 = *(volatile u16 *)&arg0->heading1D4;
    temp_a0 = temp_v1 & 0xFFF;
    arg0->heading1D4 = temp_a0;
    if (temp_v1 & 0x800) {
        arg0->heading1D4 = (u16) (temp_a0 | 0xF000);
    }
    /* ASM_BARRIER: Reload the normalized heading after its wire-halfword publication. */
    __asm__("" ::: "memory");
    {
        /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
        register s32 heading __asm__("$2") = arg0->heading1D4;
        /* ASM_BARRIER: Retain the initialized intermediate at the register phase boundary. */
        __asm__("" :: "r"(heading));
        var_s0 = heading - temp_a1;
    }
    /* ASM_BARRIER: Retain the initialized intermediate at the register phase boundary. */
    __asm__("" :: "r"(var_s0));
    var_s0 &= 0xFFF;
    if (var_s0 & 0x800) {
        var_s0 |= ~0xFFF;
    }
    {
        /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
        register s32 value __asm__("$2");
        /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
        register s32 channel __asm__("$3");
        /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
        register s32 steering __asm__("$4");
        value = func_80047CE0((RrRaceVehicle *)arg0);
        steering = var_s0;
        temp_s1->limit6E = value;
        channel = temp_s1->error92;
        value = (s32)((u32)value << 16);
        var_s0 = value >> 4;
        temp_s1->error92 = steering;
        value = steering - channel;
        if (steering < 0) steering += 7;
        /* ASM_BARRIER: Retain the initialized intermediate at the register phase boundary. */
        __asm__("" :: "r"(value));
        channel = value;
        /* ASM_BARRIER: Retain the initialized intermediate at the register phase boundary. */
        __asm__("" :: "r"(channel));
        value = steering >> 3;
        steering = -value;
        if (channel < 0) channel += 3;
        value = channel >> 2;
        value = steering - value;
        temp_v0_3 = value;
    }
    var_v1_2 = temp_v0_3;
    if (temp_v0_3 > 0) {
        if (temp_s1->limit6E < temp_v0_3) {
            var_v1_2 = (s16) (u16) temp_s1->limit6E;
        }
    } else if (temp_v0_3 < -temp_s1->limit6E) {
        var_v1_2 = -(s16) (u16) temp_s1->limit6E;
    }
    var_a0_2 = -0xF000;
    temp_v0_4 = (s32) (temp_s1->filtered90 + var_v1_2) / 2;
    temp_s1->filtered90 = (s16) temp_v0_4;
    temp_s1->steering8C = (u16) temp_v0_4;
    temp_v1_3 = -(s16) temp_v0_4 * 0x90;
    arg0->lateral178 = temp_v1_3;
    if ((temp_v1_3 < -0xF000) || (var_a0_2 = 0xF000, ((temp_v1_3 > 0xF000) != 0))) {
        arg0->lateral178 = var_a0_2;
    }
    if (arg0->input1D8 > 0) {
        if (arg0->lock1B8 == 0) {
            arg0->heading1D4 = (u16) (arg0->heading1D4 + temp_s1->steering8C);
        }
    }
    temp_a1_2 = func_80047DF8((char *)arg0, temp_s1->laneTimer38, &sp18, &sp1C, &sp20);
    temp_s1->dragFlag46 = 0;
    if (temp_s1->shiftDir62 != 0) {
        temp_s1->gear68 = (u16) (temp_s1->gear68 + (u16) temp_s1->shiftDir62);
        if (temp_s1->shiftDir62 > 0) {
            temp_s1->holdFlag48 = 16;
        } else {
            temp_s1->holdFlag48 = 5;
        }

    }
    if (temp_s1->holdFlag48 > 0) {
        temp_s1->holdFlag48 = (s16) ((u16) temp_s1->holdFlag48 - 1);
        temp_s1->accel50 = 0;
    } else if (temp_a1_2 < var_s0) {
        temp_s1->accel50 = (s32) temp_s1->gearRatios100[temp_s1->gear68];
    } else if (arg0->input1D8 != 0) {
        /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
        register s32 half_limit __asm__("$2") = var_s0 >> 1;
        temp_a0_2 = (s32)((u32)temp_a1_2 - (u32)var_s0);
        temp_s1->accel50 = 0;
        if (half_limit < temp_a0_2) {
            temp_s1->dragFlag46 = 1;
            {
            /* ASM_BARRIER: Keep this initialized drive value in its original register phase. */
            register s32 fraction __asm__("$2") = (var_s0 * 2) / 3;
            if ((fraction < temp_a0_2) && (temp_s1->steeringEnabled124 != 0) && (temp_s1->steering94.mode00 == 0)) {
                func_80047FBC((RrRaceVehicle *)arg0, temp_a1_2);
            }
            }
        }
    } else {
        temp_s1->accel50 = (s32) temp_s1->gearRatios100[0];
    }
    if (temp_s1->accel50 > 0) {
        var_a0_3 = arg0->rotation50;
        if (var_a0_3 < -0xC8) {
            var_a0_3 = -0xC8;
        } else if (var_a0_3 >= 0x65) {
            var_a0_3 = 0x64;
        }
        temp_v0_5 = (s32)((u32)temp_s1->accel50 * (u32)(var_a0_3 + 0x3E8)) / 1000;
        temp_s1->accel50 = temp_v0_5;
        if (temp_v0_5 < 0) {
            temp_s1->accel50 = 0;
        }
    }
    temp_a0_3 = temp_s1->attenuation4A;
    if (temp_a0_3 > 0) {
        temp_s1->accel50 = (s32)((u32)temp_s1->accel50 * (u32)(0x10-temp_a0_3)) / 16;
    }
    if (temp_s1->accel50 == 0) {
        temp_s1->accelerating54 = 0;
    } else {
        temp_s1->accelerating54 = 1;
    }
    func_800483D8((RrRaceVehicle *)arg0);
}

void func_80048A6C(char *a0) {
    w3t1_F f;
    char *s1 = a0;
    char *s0 = s1 + 0x1F4;
    s32 v0, v1, a1;
    register s32 a __asm__("$4"); /* ASM_BARRIER: pin a to $4 to reproduce a0/a1 register split of the original */
    a1 = I(s0, 0x50);
    if (a1 != 0) {
        v1 = H(s0, 0x44);
        if (v1 > 0) {
            a = H(s0, 0x11C);
            v0 = a;
            if (a < 0) v0 = -v0;
            v1 = v0 - v1;
            v1 = (a1 * v1) / v0;
            if (a > 0) {
                v0 = H(s0, 0x11E);
                v0 = (v1 * v0) / 1024;
                v1 = I(s0, 0x4C);
                v1 += a1 + v0;
                I(s0, 0x4C) = v1;
            } else {
                I(s0, 0x4C) = I(s0, 0x4C) + v1;
            }
            U(s0, 0x44) = U(s0, 0x44) - 1;
        } else {
            I(s0, 0x4C) = I(s0, 0x4C) + a1;
        }
        H(s1, 0x23A) = 0;
        v0 = I(s0, 0x4C);
        v1 = I(s0, 0xF8);
        if (v1 < v0) I(s0, 0x4C) = v1;
    } else {
        if (H(s0, 0x48) == 0) {
            if (H(s0, 0x46) != 0) {
                I(s0, 0x4C) = (I(s0, 0x4C) * 94) / 100;
            } else {
                I(s0, 0x4C) = (I(s0, 0x4C) * 99) / 100;
            }
        }
    }
    U(s1, 0x1DA) = U(s1, 0x1D8);
    H(s1, 0x1D8) = I(s0, 0x4C) / 256;
    f.rot.vx = 0;
    f.rot.vy = U(s1, 0x1D4);
    f.rot.vz = 0;
    func_80091844(&f.rot, f.m);
    f.sv.vx = 0;
    f.sv.vy = 0;
    f.sv.vz = H(s1, 0x1D8) / 16;
    RR_GTE_SETROT(f.m);
    RR_GTE_MVMVA_ROT_V0_NOCV(&f.sv);
    RR_GTE_STORE_IR_S16(&f.out);
    v0 = I(s0, 0xFC);
    v1 = H(s0, 0x68);
    H(s0, 0x62) = 0;
    if (v1 < v0 - 1) {
        if (H(s1, 0x1D8) > (s32)*(u16 *)((char *)s0 + (v1 << 1) + 0x10C)) {
            H(s0, 0x62) = 1;
            H(s0, 0x64) = 1;
        }
    }
    v0 = H(s0, 0x68);
    if (v0 > 0) {
        v0 = v0 - 1;
        v1 = *(u16 *)((char *)s0 + (v0 << 1) + 0x10C) - 0x24;
        if (v1 < 0) v1 = 0;
        if (H(s1, 0x1D8) < v1) {
            v1 = U(s0, 0x56);
            H(s0, 0x62) = -1;
            H(s0, 0x64) = -1;
            v0 = (0x2800 - v1) / 4;
            H(s0, 0x66) = v0;
            if ((s16)v0 < 0) H(s0, 0x66) = 0;
        }
    }
    I(s1, 0x30) = f.out.vx;
    I(s1, 0x38) = f.out.vz;
    func_800468F0(s1);
}
