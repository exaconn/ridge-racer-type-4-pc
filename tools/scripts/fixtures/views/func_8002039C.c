#include "rr/textured_triangle.h"

#include "m2c_macros.h"

/* legacy typing of shared globals in this part of the unit */
#define func_8001FCC8 (*(u8 *(*)(u32 *,u8 *,u16))&func_8001FCC8)

#undef func_8001FCC8

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)(u32 *, u32 *))func_80095A44)

#undef func_80095A44

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((int (*)())func_80095A44)

#define func_80095B24 ((int (*)())func_80095B24)

#undef func_80095A44

#undef func_80095B24

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((int (*)())func_80095A44)

#define func_80095B24 ((int (*)())func_80095B24)

#undef func_80095A44

#undef func_80095B24

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)(u32 *,u32 *))func_80095A44)

#undef func_80095A44

extern void func_80095B94(void *),func_80095AF4(void *,s32),func_80095A44(u32 *,u32 *);

/* extern */
void func_80095AF4(void *, s32);

/* extern */
void func_80095B24(void *, M2C_UNK);

/* extern */
void func_80095B74(void *);

/* extern */
struct S32 *func_8002039C(u32 *arg0, struct S32 *arg1, void *arg2, void *arg3, void *arg4, void *arg5, u16 arg6, u16 arg7, u16 arg8, u16 arg9) {
    func_80095B74(arg1);
    M2C_FIELD(arg1, u16 *, 8) = (u16) M2C_FIELD(arg2, u16 *, 0);
    M2C_FIELD(arg1, u16 *, 0xA) = (u16) M2C_FIELD(arg3, u16 *, 0);
    M2C_FIELD(arg1, u16 *, 0x10) = (u16) M2C_FIELD(arg2, u16 *, 4);
    M2C_FIELD(arg1, u16 *, 0x12) = (u16) M2C_FIELD(arg3, u16 *, 4);
    M2C_FIELD(arg1, u16 *, 0x18) = (u16) M2C_FIELD(arg2, u16 *, 8);
    M2C_FIELD(arg1, u16 *, 0x1A) = (u16) M2C_FIELD(arg3, u16 *, 8);
    M2C_FIELD(arg1, u8 *, 0xC) = (u8) M2C_FIELD(arg4, u8 *, 0);
    M2C_FIELD(arg1, u8 *, 0xD) = (u8) M2C_FIELD(arg5, u8 *, 0);
    M2C_FIELD(arg1, u8 *, 0x14) = (u8) M2C_FIELD(arg4, u8 *, 4);
    M2C_FIELD(arg1, u8 *, 0x15) = (u8) M2C_FIELD(arg5, u8 *, 4);
    M2C_FIELD(arg1, u8 *, 0x1C) = (u8) M2C_FIELD(arg4, u8 *, 8);
    M2C_FIELD(arg1, u8 *, 0x1D) = (u8) M2C_FIELD(arg5, u8 *, 8);
    func_80095AF4(arg1, arg8 & 0xFFFF);
    if ((arg9 & 0xFFFF) != 0x80) {
        func_80095B24(arg1, 0);
        M2C_FIELD(arg1, s8 *, 4) = (s8) arg9;
        M2C_FIELD(arg1, s8 *, 5) = (s8) arg9;
        M2C_FIELD(arg1, s8 *, 6) = (s8) arg9;
    } else {
        func_80095B24(arg1, 1);
    }
    M2C_FIELD(arg1, u16 *, 0x16) = arg6;
    M2C_FIELD(arg1, u16 *, 0xE) = arg7;
    func_80095A44(arg0, (u32 *)arg1++);
    return arg1;
}
