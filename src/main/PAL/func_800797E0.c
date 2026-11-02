/* RODATA 0x800114a0 0xc4 */
typedef struct {
    int w[31];
} Struct7C;

typedef struct {
    int w[16];
} Struct40;

typedef struct {
    char b[0x2C];
} Struct2C;

typedef struct {
    char b[4];
} Struct4;

typedef struct {
    char pad0[0x24];
    Struct4 f24[9];
} Struct48;

typedef struct {
    int w[4];
} Struct10;

typedef struct {
    int w[3];
} StructC;

typedef struct {
    unsigned char pad0[0x280];
    Struct7C f280;
    Struct40 f2FC;
    Struct4 f33C[9];
    Struct2C f360[3];
    int f3E4[2][8][3];
    Struct10 f4A4[2][8][5];
    Struct10 f9A4[2][8][5];
    int fEA4[0x210];
    unsigned char pad16E4[0x3C0];
    int f1AA4;
} SaveBuf;

typedef struct {
    unsigned char pad0[0x280];
    int f280;
    unsigned char pad284[0x7C];
    unsigned char f300[2][0x840];
} SaveBuf11;

typedef struct {
    unsigned char b[0x840];
} Struct840;

extern int func_8002B75C();
extern int func_8004C0EC();

extern Struct48 D_800ABE08;
extern int D_800AC5D4;
extern SaveBuf *D_800ACC0C;
extern int D_800ACC28[2][8];
extern Struct40 D_800AD528;
extern Struct10 D_800AD5C8[2][8][5];
extern Struct2C D_800F2E30[3];
extern int D_800F3838[2][8][3];
extern Struct840 *D_800F3910;
extern Struct840 *D_800F3918;
extern Struct7C D_800F3920;
extern int D_800F3B28[];
extern int D_800F4CE8[2][8];
extern int D_800F4D38;
extern short D_800F4D3C;
extern short D_800F4D44;
extern Struct10 D_800FF700[2][8][5];
extern Struct2C D_8010B258[3];
extern Struct48 D_8010BFD8[2];
extern StructC D_8010C088[];
extern StructC D_80118420[];
extern Struct48 D_80118458[2];
extern int D_801272F0[];

int func_800797E0(int arg0) {
    int a2 = 0;
    SaveBuf *s3;
    SaveBuf11 *p;
    int a1;
    int s0;
    int t0;
    int k;
    int v10;
    int v11;

    switch (arg0) {
    case 0:
    case 1:
    case 0x30:
    default:
        s3 = D_800ACC0C;
        a1 = 0;
        for (s0 = 0; (unsigned int)s0 < 0x6A9; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            D_800F3920 = s3->f280;
            D_800AD528 = s3->f2FC;
            for (s0 = 0; s0 < 3; s0++) {
                if (arg0 == 0x30) {
                    D_800F2E30[s0] = s3->f360[s0];
                } else {
                    D_8010B258[s0] = s3->f360[s0];
                }
            }
            for (s0 = 0; s0 < 9; s0++) {
                D_800ABE08.f24[s0] = s3->f33C[s0];
            }
            for (s0 = 0; s0 < 2; s0++) {
                for (t0 = 0; t0 < 8; t0++) {
                    for (k = 0; k < 3; k++) {
                        D_800F3838[s0][t0][k] = s3->f3E4[s0][t0][k];
                    }
                    for (k = 0; k < 5; k++) {
                        D_800FF700[s0][t0][k] = s3->f4A4[s0][t0][k];
                        D_800AD5C8[s0][t0][k] = s3->f9A4[s0][t0][k];
                    }
                    D_800ACC28[s0][t0] = D_800FF700[s0][t0][0].w[3];
                    D_800F4CE8[s0][t0] = D_800AD5C8[s0][t0][0].w[3];
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                D_800F3B28[s0] = s3->fEA4[s0];
            }
            func_8004C0EC();
            a2 = 1;
        }
        break;
    case 2:
        s3 = D_800ACC0C;
        a1 = 0;
        for (s0 = 0; (unsigned int)s0 < 0x6A9; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            for (s0 = 0; s0 < 9; s0++) {
                if (D_800F4D44 == 2 || D_800F4D44 == 3) {
                    D_80118458[D_800AC5D4 & 1].f24[s0] = s3->f33C[s0];
                    func_8002B75C(&D_80118420[D_800AC5D4 & 1], &s3->f280);
                } else if (D_800F4D44 == 4) {
                    D_8010BFD8[D_800AC5D4 & 1].f24[s0] = s3->f33C[s0];
                    func_8002B75C(&D_8010C088[D_800AC5D4], &s3->f280);
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                *(int *)((D_800AC5D4 & 1) * 0x840 + (s0 << 2) + (int)D_800F3B28) = s3->fEA4[s0];
            }
            a2 = 1;
        }
        break;
    case 0x10:
        v10 = *(int *)D_800ACC0C;
        D_801272F0[D_800F4D3C] = v10;
        a2 = 1;
        break;
    case 0x11:
        v11 = (p = (SaveBuf11 *)D_800ACC0C)->f280;
        D_800F4D38 = D_801272F0[D_800F4D3C] = v11;
        for (t0 = 0; t0 < 2; t0++) {
            for (s0 = 0; s0 < 0x840; s0++) {
                if (t0 == 0) {
                    D_800F3918->b[s0] = p->f300[0][s0];
                } else {
                    D_800F3910->b[s0] = p->f300[t0][s0];
                }
            }
        }
        a2 = 1;
        break;
    }
    return a2;
}
