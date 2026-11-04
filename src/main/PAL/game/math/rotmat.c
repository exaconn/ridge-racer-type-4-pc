#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "psyq/libgte/gtemat7.h"
#include "rr/matrix_workspace.h"
#include "rr/menu_overlay_runtime.h"

extern short D_800AC0A4;
extern unsigned short D_800AD598[];
extern char D_800FF690[];
extern char D_800FF6B8[];
extern char D_800FF6D8[];
extern void func_80090914();
extern short D_800A22E4[];

void func_8006E030(short *a0, int a1);
void func_8006E09C(void *arg0, M2C_UNK arg1);
void func_8006E108(void *arg0, M2C_UNK arg1);
void func_8006E174(void *arg0);
void func_8006E300(int *a0);
s32 func_8006E368(int a0, int a1);

void func_8006E030(short *a0, int a1) {
    short s = func_80090424(a1);
    short c = func_800904F4(a1);
    a0[1] = -s;
    a0[0] = c;
    a0[4] = c;
    a0[2] = 0;
    a0[3] = s;
    a0[5] = 0;
    a0[6] = 0;
    a0[7] = 0;
    a0[8] = 0x1000;
}

/* legacy typing of shared globals in this part of the unit */
#define func_80090424 ((int (*)())func_80090424)
#define func_800904F4 ((int (*)())func_800904F4)

void func_8006E09C(void *arg0, M2C_UNK arg1) {
    s16 temp_s1;
    s16 temp_v0;

    temp_s1 = func_80090424(arg1);
    temp_v0 = func_800904F4(arg1);
    M2C_FIELD(arg0, s16 *, 4) = (s16) -temp_s1;
    M2C_FIELD(arg0, s16 *, 0) = temp_v0;
    M2C_FIELD(arg0, s16 *, 2) = 0;
    M2C_FIELD(arg0, s16 *, 6) = 0;
    M2C_FIELD(arg0, s16 *, 8) = 0x1000;
    M2C_FIELD(arg0, s16 *, 0xA) = 0;
    M2C_FIELD(arg0, s16 *, 0xC) = temp_s1;
    M2C_FIELD(arg0, s16 *, 0xE) = 0;
    M2C_FIELD(arg0, s16 *, 0x10) = temp_v0;
}

#undef func_80090424
#undef func_800904F4

/* legacy typing of shared globals in this part of the unit */
#define func_80090424 ((int (*)())func_80090424)
#define func_800904F4 ((int (*)())func_800904F4)

void func_8006E108(void *arg0, M2C_UNK arg1) {
    s16 temp_s1;
    s16 temp_v0;

    temp_s1 = func_80090424(arg1);
    temp_v0 = func_800904F4(arg1);
    M2C_FIELD(arg0, s16 *, 0) = 0x1000;
    M2C_FIELD(arg0, s16 *, 2) = 0;
    M2C_FIELD(arg0, s16 *, 4) = 0;
    M2C_FIELD(arg0, s16 *, 6) = 0;
    M2C_FIELD(arg0, s16 *, 8) = temp_v0;
    M2C_FIELD(arg0, s16 *, 0xA) = (s16) -temp_s1;
    M2C_FIELD(arg0, s16 *, 0xC) = 0;
    M2C_FIELD(arg0, s16 *, 0xE) = temp_s1;
    M2C_FIELD(arg0, s16 *, 0x10) = temp_v0;
}

#undef func_80090424
#undef func_800904F4

void func_8006E174(void *arg0) {
    M2C_FIELD(arg0, s16 *, 0) = 0x1000;
    M2C_FIELD(arg0, s16 *, 2) = 0;
    M2C_FIELD(arg0, s16 *, 4) = 0;
    M2C_FIELD(arg0, s16 *, 6) = 0;
    M2C_FIELD(arg0, s16 *, 8) = 0x1000;
    M2C_FIELD(arg0, s16 *, 0xA) = 0;
    M2C_FIELD(arg0, s16 *, 0xC) = 0;
    M2C_FIELD(arg0, s16 *, 0xE) = 0;
    M2C_FIELD(arg0, s16 *, 0x10) = 0x1000;
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
}

void func_8006E1AC(void) {
    char sp10[32];
    int *g = (int *)SCRATCHPAD;
    func_8006E09C(SCRATCH_ROT_MATRIX, g[7]);
    func_8006E09C(D_800FF690, g[7]);
    func_8006E174(D_800FF6B8);
    func_8006E09C(D_800FF6D8, g[7]);
    func_8006E108(sp10, g[6]);
    func_80090DB4(sp10, SCRATCH_ROT_MATRIX);
    func_80090DB4(sp10, D_800FF690);
    func_80090DB4(sp10, D_800FF6B8);
    func_8006E030(sp10, g[8]);
    func_80090DB4(sp10, SCRATCH_ROT_MATRIX);
    func_80090DB4(sp10, D_800FF690);
    func_80090DB4(sp10, D_800FF6B8);
    func_8006E09C(sp10, 0x800);
    func_80090914(sp10, SCRATCH_ROT_MATRIX, D_800AD598);
    func_8006E108(sp10, D_800AC0A4 * 2);
    func_80090DB4(sp10, D_800AD598);
    D_800AD598[0] = -D_800AD598[0];
    D_800AD598[1] = -D_800AD598[1];
    D_800AD598[2] = -D_800AD598[2];
    func_800910B4(SCRATCH_ROT_MATRIX);
}

void func_8006E300(int *a0) {
    char sp10[32];
    func_8006E09C((SCRATCHPAD + 0x108), a0[1]);
    func_8006E108(sp10, a0[0]);
    func_80090DB4(sp10, (SCRATCHPAD + 0x108));
    func_8006E030(sp10, a0[2]);
    func_80090DB4(sp10, (SCRATCHPAD + 0x108));
}

int func_8006E368(int a0, int a1)
{
    int r;
    if (a0 == 0) {
        r = 0;
        if (a1 != 0) {
            r = -0x400;
            if (a1 > 0) {
                return 0x400;
            }
            goto ret;
        }
        goto ret;
    }
    if (a0 > 0) {
        if (a1 >= 0) {
            if (a0 < a1) {
                return 0x400 - D_800A22E4[(a0 << 10) / a1];
            }
            r = D_800A22E4[(a1 << 10) / a0];
ret:
            return r;
        }
        a1 = -a1;
        if (a0 < a1) {
            return D_800A22E4[(a0 << 10) / a1] - 0x400;
        }
        return -D_800A22E4[(a1 << 10) / a0];
    }
    a0 = -a0;
    if (a1 >= 0) {
        if (a0 < a1) {
            return D_800A22E4[(a0 << 10) / a1] + 0x400;
        }
        return 0x800 - D_800A22E4[(a1 << 10) / a0];
    }
    a1 = -a1;
    if (a0 < a1) {
        return 0xC00 - D_800A22E4[(a0 << 10) / a1];
    }
    return D_800A22E4[(a1 << 10) / a0] + 0x800;
}
