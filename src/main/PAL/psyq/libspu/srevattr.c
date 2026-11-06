/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -fno-rerun-cse-after-loop */
#include "rr/spu.h"
#include "psyq/libspu/smutil.h"

typedef struct {
    u32 mask;
    s32 mode;
    s16 depth_left;
    s16 depth_right;
    s32 delay;
    s32 feedback;
} SpuReverbAttr;

typedef struct {
    s32 mode;
    s16 depth_left;
    s16 depth_right;
    s32 delay;
    s32 feedback;
} RevAttr;

extern RevAttr D_800A485C;
extern s32 D_800A4854;
extern s32 D_800A48C8[];
extern RrSpuRegisterPatch D_800A4E78[];

extern void func_8007F3D0();
extern s32 SpuClearReverbWorkArea();
extern void func_80086084();

/* Byte copy of one reverb register preset (inlined at each use). */
static __inline__ void copy(u8 *dst, u8 *src, s32 n) {
    for (n--; n != -1; n--) {
        *dst++ = *src++;
    }
}

s32 func_80085BA4(SpuReverbAttr *attr) {
    RrSpuRegisterPatch patch;
    s32 mask;
    s32 all;
    s32 mode_set;
    s32 delay_set;
    s32 fb_set;
    s32 rev_on;
    s32 clear_mode;
    u32 mode;
    /* ASM_BARRIER: pin the start-address table base to $s1 as in the original allocation */
    register s32 *tbl asm("$17");

    rev_on = 0;
    mode_set = 0;
    delay_set = 0;
    clear_mode = 0;
    mask = attr->mask;
    fb_set = 0;
    all = (mask == 0);
    patch.mask = 0;

    if (all || (mask & 1)) {
        mode = attr->mode;
        if (mode & 0x100) {
            mode &= ~0x100;
            clear_mode = 1;
        }
        if (mode >= 10) {
            return -1;
        }
        tbl = D_800A48C8;
        if (_SpuIsInAllocateArea_(*(s32 *)((u8 *)D_800A48C8 + (mode << 2))) != 0) {
            return -1;
        }
        mode_set = 1;
        D_800A485C.mode = mode;
        D_800A4854 = tbl[D_800A485C.mode];
        copy((u8 *)&patch, (u8 *)&D_800A4E78[D_800A485C.mode], 0x44);
        switch (D_800A485C.mode) {
        case 7:
            D_800A485C.feedback = 0x7F;
            D_800A485C.delay = 0x7F;
            break;
        case 8:
            D_800A485C.feedback = 0;
            D_800A485C.delay = 0x7F;
            break;
        default:
            D_800A485C.feedback = 0;
            D_800A485C.delay = 0;
            break;
        }
    }

    /* Nested tests keep gcc from folding the 7..8 range check into one compare. */
    if ((all || (mask & 8)) && D_800A485C.mode < 9) if (D_800A485C.mode >= 7) {
        s32 d;
        delay_set = 1;
        if (!mode_set) {
            copy((u8 *)&patch, (u8 *)&D_800A4E78[D_800A485C.mode], 0x44);
            patch.mask = 0x0C011C00;
        }
        D_800A485C.delay = attr->delay;
        patch.values[10] = ((D_800A485C.delay << 13) / 127) - patch.values[0];
        d = (D_800A485C.delay << 12) / 127;
        patch.values[11] = d - patch.values[1];
        patch.values[12] = patch.values[13] + d;
        patch.values[16] = patch.values[17] + d;
        patch.values[26] = patch.values[28] + d;
        patch.values[27] = patch.values[29] + d;
    }

    if ((all || (mask & 0x10)) && D_800A485C.mode < 9) if (D_800A485C.mode >= 7) {
        fb_set = 1;
        if (!mode_set) {
            if (!delay_set) {
                copy((u8 *)&patch, (u8 *)&D_800A4E78[D_800A485C.mode], 0x44);
                patch.mask = 0x80;
            } else {
                patch.mask |= 0x80;
            }
        }
        D_800A485C.feedback = attr->feedback;
        patch.values[7] = (D_800A485C.feedback * 0x8100) / 127;
    }

    if (mode_set) {
        rev_on = (D_800A47D8->spu_control >> 7) & 1;
        if (rev_on) {
            D_800A47D8->spu_control &= ~0x80;
        }
    } else {
        if (all || (mask & 2)) {
            ((volatile u16 *)D_800A47D8)[0x184 / 2] = attr->depth_left;
            D_800A485C.depth_left = attr->depth_left;
        }
        if (all || (mask & 4)) {
            ((volatile u16 *)D_800A47D8)[0x186 / 2] = attr->depth_right;
            D_800A485C.depth_right = attr->depth_right;
        }
        goto skip;
    }
    ((volatile u16 *)D_800A47D8)[0x184 / 2] = 0;
    ((volatile u16 *)D_800A47D8)[0x186 / 2] = 0;
    D_800A485C.depth_left = 0;
    D_800A485C.depth_right = 0;
skip:

    if (mode_set || delay_set || fb_set) {
        func_80086084(&patch);
    }

    if (clear_mode) {
        SpuClearReverbWorkArea(D_800A485C.mode);
    }

    if (mode_set) {
        func_8007F3D0(0xD1, D_800A4854, 0);
        if (rev_on) {
            D_800A47D8->spu_control |= 0x80;
        }
    }

    return 0;
}
