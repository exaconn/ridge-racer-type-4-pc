/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/layout.h"
#include "psyq/libpad/padpoll.h"
#include "psyq/libpad/padwait.h"
#include "rr/serial.h"
#include "psyq/libpad/padrecv.h"

typedef struct { char pad[0x3C]; char *f3c; } T;

typedef struct { char p0[0xC]; int fC; char p1[0x27]; unsigned char f37; } O;

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

extern int (*D_800ABC80)();
extern int D_800ABCE8;
extern int D_800ABCA8;
extern int D_800ABCB8;
extern int D_800ABCA4;
extern int (*D_800ABC98)();
extern int (*D_800ABC94)();
extern int D_800ABCB4;
extern int D_800ABCE0;
/* Partial transfer state; field purposes beyond this routine are still unknown. */
typedef struct {
    u8 pad00[0x37];
    u8 flag37;
    u8 pad38[4];
    u8 *packet;
} RrTransferState;
RR_ASSERT_SIZE(RrTransferState, 0x40);
RR_ASSERT_OFFSET(RrTransferState, flag37, 0x37);
extern s32 (*D_800ABC70)(RrTransferState *, s32);
extern void (*D_800ABC68)(s32);
extern void (*D_800ABC74)(Rec8009991C *);
extern void (*D_800ABC88)(Rec8009991C *);
extern void (*D_800ABC8C)(Rec8009991C *);
extern Rec8009991C *D_800ABC9C;
extern s32 D_800ABCC0[];
extern Unk800ABCE4 *D_800ABCE4;

void func_800996C4(T *a0);
void func_8009970C();
int func_800997E4();
s32 func_80099890(RrTransferState *state);
s32 func_8009991C(Rec8009991C *arg0);

void func_800996C4(T *a0) {
    int r = D_800ABC80(a0);
    char *p = a0->f3c;
    D_800ABCE8 = r;
    *p = 0;
    _padSioRW(a0, -2);
}

/* legacy typing of shared globals in this part of the unit */
#define func_80098788 ((int (*)())func_80098788)

void func_8009970C(o) O *o; {
    if (D_800ABCA8 == D_800ABCB8) {
        if (D_800ABCA4 != 0) {
            D_800ABC98();
            D_800ABC94();
        }
    }
    if (D_800ABCE8 != 0) {
        D_800ABC80(o->fC);
        D_800ABC80(o->fC + 0xF0);
    }
    if (o->f37 == 0) func_80098788(o, 0x42); else func_80098788(o, o->f37);
}

#undef func_80098788

/* legacy typing of shared globals in this part of the unit */
#define func_80098788 ((int (*)())func_80098788)

int func_800997E4(a0) unsigned char *a0; {
  int a1, v, t;
  if (D_800ABCE8 != 0) {
    D_800ABC80(*(int *)(a0 + 0xC) + 0x1E0);
    D_800ABC80(*(int *)(a0 + 0xC) + 0x2D0);
  }
  a1 = 0;
  if (a0[0x37] == 0) a1 = D_800ABCB4;
  v = func_80098788(a0, a1);
  if (v < 0) return v;
  t = (v & 0xF) * 2;
  D_800ABCE0 = t;
  if (t == 0) D_800ABCE0 = 0x20;
  return 0;
}

#undef func_80098788

s32 func_80099890(RrTransferState *state) {
    s32 mode = 0;
    s32 result;

    if ((state->packet[0] >> 4) == 8) {
        mode = state->flag37 == 0;
    }
    result = func_80098788(state, (u8)D_800ABC70(state, mode));
    if (result == 0x5A || result == 0 || result < 0) return result;
    return -4;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC70 (*(s32 (*(*))(Rec8009991C *, s32))&D_800ABC70)
#define _padSioRW ((s32 (*)(Rec8009991C *, u8))_padSioRW)
#define func_80098788 ((s32 (*)(Rec8009991C *, u8))func_80098788)

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
            setRC2wait(0x3C);
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
        res = _padSioRW(arg0, (u8)D_800ABC70(arg0, mode));
        if (res < 0) {
            return res;
        }
        setRC2wait(0x3C);
        if (func_80098A00() == 0) {
            return -3;
        }
        D_800ABCE0--;
    }

    while (--D_800ABCE0 > 0) {
        res = _padSioRW(arg0, (u8)D_800ABC70(arg0, mode));
        if (res < 0) {
            return res;
        }
        if (D_800ABCE4->unk0E != 0x22) {
            setRC2wait(0x3C);
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

#undef D_800ABC70
#undef _padSioRW
#undef func_80098788
