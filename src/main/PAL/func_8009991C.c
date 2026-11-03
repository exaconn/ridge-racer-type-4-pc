/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/layout.h"

typedef struct Rec8009991C {
    u8 pad00[0xC];
    struct Rec8009991C *unk0C;
    u8 pad10[0x27];
    u8 unk37;
    u8 pad38[4];
    u8 *unk3C;
    u8 pad40[4];
    u8 unk44;
    u8 pad45[0xF0 - 0x45];
} Rec8009991C;

typedef struct {
    u8 unk00;
    u8 pad01[0xD];
    u16 unk0E;
} Unk800ABCE4;

extern void (*D_800ABC68)(s32);
extern s32 (*D_800ABC70)(Rec8009991C *, s32);
extern void (*D_800ABC74)(Rec8009991C *);
extern void (*D_800ABC88)(Rec8009991C *);
extern void (*D_800ABC8C)(Rec8009991C *);
extern Rec8009991C *D_800ABC9C;
extern s32 D_800ABCA8;
extern s32 D_800ABCB4;
extern s32 D_800ABCC0[];
extern s32 D_800ABCE0;
extern Unk800ABCE4 *D_800ABCE4;

extern s32 func_80098578(Rec8009991C *, u8);
extern s32 func_80098788(Rec8009991C *, u8);
extern s32 func_80098A00(void);
extern void func_80098A90(void);
extern void func_8009A7F4(s32);

s32 func_8009991C(Rec8009991C *arg0) {
    s32 i;
    s32 res;
    s32 idx;
    Rec8009991C *ptr;
    s32 mode;
    s32 b;

    D_800ABC74(arg0);
    mode = 0;
    if (D_800ABCB4 != 0 && ((b = arg0->unk3C[0]) >> 4) == 8) {
        mode = (arg0->unk37 == 0);
    }

    if (mode != 0) {
        for (i = -1; i < 4; i++) {
            if (--D_800ABCE0 <= 0) {
                break;
            }
            if (i >= 0) {
                D_800ABC74(&arg0->unk0C[i]);
            }
            res = func_80098788(arg0, (u8)D_800ABC70(arg0, 1));
            if (res < 0) {
                return res;
            }
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) {
                return -3;
            }
        }
    }

    ptr = 0;
    idx = (D_800ABCA8 == 0);
    while (D_800ABCE0 >= 2) {
        if (D_800ABCC0[idx] < 0) {
            break;
        }
        if (D_800ABCC0[idx] > 0) {
            ptr = &D_800ABC9C[idx].unk0C[D_800ABCC0[idx]] - 1;
            D_800ABC88(ptr);
            {
                void *a0r;
            }
        }
        switch (D_800ABCC0[idx]) {
        case 4:
            D_800ABCC0[idx] = 3;
            break;
        case 3:
            D_800ABC88(ptr - 1);
            D_800ABCC0[idx] = 1;
            break;
        case 0:
        case 1:
            ptr = &D_800ABC9C[idx];
            D_800ABC88(ptr);
            D_800ABC8C(ptr);
            D_800ABCC0[idx] = -1;
            break;
        }
        res = func_80098578(arg0, (u8)D_800ABC70(arg0, mode));
        if (res < 0) {
            return res;
        }
        func_8009A7F4(0x3C);
        if (func_80098A00() == 0) {
            return -3;
        }
        D_800ABCE0--;
    }

    while (--D_800ABCE0 > 0) {
        res = func_80098578(arg0, (u8)D_800ABC70(arg0, mode));
        if (res < 0) {
            return res;
        }
        if (D_800ABCE4->unk0E != 0x22) {
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) {
                res = -3;
                goto end;
            }
        }
    }

    func_80098A90();
    arg0->unk3C[arg0->unk44++] = D_800ABCE4->unk00;
    D_800ABC68(0);
    res = 0;
end:
    return res;
}
