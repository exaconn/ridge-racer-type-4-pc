typedef struct {
    short s0, s2, s4, s6;
    int s8, sC;
    char pad10[0x2C];
} S3C;

typedef struct {
    short x, y;
    int z;
} P8;

typedef struct {
    char pad0[0x38];
    short s38;
    short s3A;
    short s3C;
    char pad3E[0x2C];
    short s6A;
    short s6C;
    char pad6E[0x1A];
    int w88;
} Sub1F4;

typedef struct {
    char pad0[0x20];
    int w20;
    int w24;
    int w28;
    char pad2C[0xC4];
    int wF0;
    char padF4[0x7C];
    short s170;
    char pad172[0x82];
    Sub1F4 sub1F4;
} Vehicle;

extern int D_800ABECC;
extern S3C *D_800ABEA4;
extern P8 *D_800ACB9C;
extern void func_80047230();

void func_80047A50(Vehicle *a0) {
    int sp18[4];
    int sp28[4];
    int sp38[4];
    Sub1F4 *a2;
    int a3;
    int idx0;
    int idx1;
    S3C *t1;
    S3C *t2;
    P8 *p0;
    P8 *p1;
    int v0;

    a2 = &a0->sub1F4;
    a3 = 0x400 - a0->s170;
    idx0 = a0->wF0 - a3 / 1024;
    if (idx0 < 0) {
        idx0 += D_800ABECC;
    }
    idx1 = idx0 - 1;
    if (idx1 < 0) {
        idx1 += D_800ABECC;
    }
    t1 = &D_800ABEA4[idx0];
    t2 = &D_800ABEA4[idx1];
    /* ASM_BARRIER: keep track pointer calculations before a3 mask and s6A branch */
    __asm__ volatile("");
    a3 &= 0x3FF;
    if (a2->s6A == 0) {
        switch (a2->s38) {
        case 0:
            p0 = &D_800ACB9C[idx0];
            sp18[0] = t1->s8 + p0->x;
            sp18[2] = t1->sC + p0->y;
            p1 = &D_800ACB9C[idx1];
            sp28[0] = t2->s8 + p1->x;
            sp28[2] = t2->sC + p1->y;
            sp38[0] = (sp28[0] - sp18[0]) * a3 / 1024 + sp18[0];
            sp38[2] = (sp28[2] - sp18[2]) * a3 / 1024 + sp18[2];
            break;
        case 10:
            func_80047230(t1, t2, a2->s3A, a3, sp38);
            break;
        }
    } else {
        v0 = a2->s6C >= 0 ? a2->s6C : -a2->s6C;
        if (v0 < 0x10) {
            a2->s6A = 0;
            a2->s3A = a2->s3C;
            if (a2->w88 == 3) {
                a2->w88 = 0;
            }
        } else {
            if (a2->s6C > 0) {
                a2->s6C -= 14;
                if (a2->s6C < 0) {
                    a2->s6C = 0;
                }
            } else {
                a2->s6C += 14;
                if (a2->s6C > 0) {
                    a2->s6C = 0;
                }
            }
            a2->s3A = a2->s3C - a2->s6C;
            if (a2->s3A < -800) {
                a2->s3A = -800;
            }
            if (a2->s3A > 800) {
                a2->s3A = 800;
            }
            a2->s6C = a2->s3C - a2->s3A;
        }
        func_80047230(t1, t2, a2->s3A, a3, sp38);
    }
    *(short *)((char *)a0 + 0x224) = sp38[0] - a0->w20;
    *(short *)((char *)a0 + 0x228) = sp38[2] - a0->w28;
}
