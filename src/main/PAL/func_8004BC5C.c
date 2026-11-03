typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 pad0[0x43];
    u8 f43;
    u8 f44;
    u8 f45;
    u8 f46;
    u8 f47;
    u8 f48;
    u8 f49;
    s16 f4A;
    s16 f4C;
    s16 f4E;
    s16 f50;
    s16 f52;
    u8 pad54[8];
} Pad4BC5C;

typedef struct {
    u8 pad0[0x28];
    struct {
        s8 f28;
        s8 f29;
        s8 f2A;
        s8 f2B;
    } w[2];
} Settings4BC5C;

extern Pad4BC5C D_800F3A38[];
extern Settings4BC5C D_800AD528;
extern s16 D_8009FFC0[];
extern s32 D_800AC5DC;

void func_8004BC5C(void) {
    s32 i;
    s32 j;
    u8 mask;
    s32 t1;
    u8 t0;
    u8 a0;
    Pad4BC5C *t;

    for (i = 0; i < 2; i++) {
        t = &D_800F3A38[i];
        for (j = 0; j < 2; j++) {
            mask = 1 << (j + 6);
            if (t->f48 & mask) {
                if (t->f44 & mask) {
                    t->f44 &= ~mask;
                } else {
                    t->f44 |= mask;
                    t->f48 &= ~mask;
                }
            } else if (t->f44 & mask) {
                s32 cond = (D_800AC5DC == 0x80) && !(t->f43 & mask);
                if (!cond) {
                    t->f44 &= ~mask;
                    t->f46 += t->f47;
                    t->f47 = 0;
                    t->f48 &= ~4;
                }
            }
        }
        t1 = t->f44 & 0xC0;
        t0 = t1;
        if (t->f49 >= 0x10) {
            t->f49 = 0xF;
        }
        a0 = (t->f49 * D_800AD528.w[i].f2A) / 2;
        if (t->f50 != 0) {
            if (t->f52 != 0) {
                if (__builtin_abs(t->f4C) < D_8009FFC0[D_800AD528.w[i].f29]) {
                    if (a0 != 0) {
                        t0 = t1 | (1 << (t->f52 + 3));
                    }
                    goto done;
                }
                t->f52 = 0;
            }
            if (a0 != 0) {
                t0 = t1 | 0x30;
            }
        } else {
            if (a0 != 0) {
                t0 = t1 | (1 << (t->f52 + 3));
            }
        }
    done:
        t->f44 = t0 | a0;
    }
}
