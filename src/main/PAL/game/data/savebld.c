/* RODATA 0x80011458 0x48 */
typedef struct {
    int w[31];
} Struct7C;

typedef struct {
    int w[16];
} Struct40;

typedef struct {
    char pad0[0x10];
    short f10;
    char pad12[0x1A];
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

extern Struct48 D_800ABE08;
extern SaveBuf *D_800ACC0C;
extern Struct2C D_800AD4D8;
extern Struct40 D_800AD528;
extern Struct10 D_800AD5C8[2][8][5];
extern int D_800F3838[2][8][3];
extern Struct7C D_800F3920;
extern Struct840 *D_800F399C;
extern Struct840 *D_800F39A0;
extern int D_800F3B28[];
extern int D_800F4D38;
extern unsigned short D_800F4D3C;
extern Struct10 D_800FF700[2][8][5];
extern Struct2C D_8010B258[3];

void func_80079200(int arg0) {
    SaveBuf *s3;
    SaveBuf11 *p;
    int a3;
    int t0;
    int a1;
    int t1;

    switch (arg0) {
    case 0:
    case 2:
    case 4:
    default:
        for (a3 = 0xA0; a3 < 0x800; a3++) {
            ((int *)D_800ACC0C)[a3] = 0;
        }
        s3 = D_800ACC0C;
        s3->f280 = D_800F3920;
        s3->f2FC = D_800AD528;
        for (a3 = 0; a3 < 9; a3++) {
            s3->f33C[a3] = D_800ABE08.f24[a3];
        }
        for (a3 = 0; a3 < 3; a3++) {
            D_8010B258[a3].f10 = 0;
            t1 = 0;
            for (t0 = 0; (unsigned int)t0 < 0x16; t0++) {
                t1 += ((short *)&D_8010B258[a3])[t0];
            }
            D_8010B258[a3].f10 = t1;
            s3->f360[a3] = D_8010B258[a3];
        }
        if (arg0 == 3) {
            D_800AD4D8.f10 = 0;
            t1 = 0;
            for (t0 = 0; (unsigned int)t0 < 0x16; t0++) {
                t1 += ((short *)&D_800AD4D8)[t0];
            }
            D_800AD4D8.f10 = t1;
            s3->f360[(short)((short)D_800F4D3C % 3)] = D_800AD4D8;
        }
        for (a3 = 0; a3 < 2; a3++) {
            for (t0 = 0; t0 < 8; t0++) {
                for (a1 = 0; a1 < 3; a1++) {
                    s3->f3E4[a3][t0][a1] = D_800F3838[a3][t0][a1];
                }
                for (a1 = 0; a1 < 5; a1++) {
                    s3->f4A4[a3][t0][a1] = D_800FF700[a3][t0][a1];
                    s3->f9A4[a3][t0][a1] = D_800AD5C8[a3][t0][a1];
                }
            }
        }
        for (a3 = 0; a3 < 0x210; a3++) {
            s3->fEA4[a3] = D_800F3B28[a3];
        }
        t1 = 0;
        for (a3 = 0; (unsigned int)a3 < 0x6A9; a3++) {
            t1 += ((int *)s3)[a3];
        }
        s3->f1AA4 = t1;
        break;
    case 6:
        s3 = D_800ACC0C;
        for (a3 = 0; a3 < 0x210; a3++) {
            s3->fEA4[a3] = D_800F3B28[a3];
        }
        /* fallthrough */
    case 17:
        p = (SaveBuf11 *)D_800ACC0C;
        p->f280 = D_800F4D38;
        for (t0 = 0; t0 < 2; t0++) {
            for (a3 = 0; a3 < 0x840; a3++) {
                if (t0 == 0) {
                    p->f300[0][a3] = D_800F39A0->b[a3];
                } else {
                    p->f300[t0][a3] = D_800F399C->b[a3];
                }
            }
        }
        break;
    }
}
