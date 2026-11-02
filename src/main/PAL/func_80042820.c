typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 a, b, c, d;
} R4;

typedef struct {
    char pad0[0x17];
    s8 b17;
    char pad18[4];
    u8 b1C;
    u8 b1D;
    char pad1E[0x2C - 0x1E];
} S2C;

extern int func_8001F5B4();
extern int func_8001FCC8();
extern int func_8001FD20();
extern int func_8001FF84();
extern int func_80020508();
extern void func_8002B814();

extern R4 D_8009FA1C[];
extern u8 D_8009FDF4[];
extern u8 D_8009FDF8[];
extern u8 D_8009FDFC[];
extern u8 D_8009FE00[];
extern u8 D_8009FE04[];
extern u8 D_8009FE08[];
extern u8 D_8009FE0C[];
extern S2C D_800F2E30[];
extern char *D_800F3AFC;
extern short D_800F4D3C;

int func_80042820(arg0, arg1, arg2)
int arg0;
int arg1;
int arg2;
{
    int sp28[3];
    char sp38[8];
    int sp40;
    int x;
    int y;
    int i;
    int j;
    char *p;

    x = *(short *)(D_800F3AFC + arg2 * 0x1C + 4);
    y = *(short *)(D_800F3AFC + arg2 * 0x1C + 6);
    sp40 = *(short *)(D_800F3AFC + arg2 * 0x1C + 0xC);
    y += 0x20;

    for (i = 0; i < 4; i++) {
        x += D_8009FDF4[i];
        arg1 = func_8001FF84(arg0, arg1, x, y, &D_8009FA1C[D_8009FDFC[i]], 0x7FF8, 0);
    }

    i = 7;
    p = &sp38[7];
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);

    y -= 8;
    for (i = 0; i < 3; i++) {
        x = *(short *)(D_800F3AFC + arg2 * 0x1C + 4) + 8;
        func_8002B814(sp38, i + 1, 0);
        arg1 = func_8001F5B4(arg0, arg1, x, y + D_8009FDF8[i + 1], sp38, 0x7FF8, 0, 0);
        if ((sp28[2] = D_800F2E30[i].b17) != -1) {
            sp28[0] = D_8009FE00[D_800F2E30[i].b1C & 3];
            sp28[1] = D_8009FE04[D_800F2E30[i].b1D & 3];
            sp28[2] = D_8009FE08[D_800F2E30[i].b17 / 2];
            for (j = 0; j < 3; j++) {
                x += D_8009FDF4[j + 1];
                arg1 = func_8001FF84(arg0, arg1, x, y + D_8009FDF8[i + 1], &D_8009FA1C[sp28[j]], 0x7FF8, 0);
                if (j == 2 && D_800F2E30[i].b17 < 4) {
                    x += 0xE;
                    arg1 = func_8001FF84(arg0, arg1, x, y + D_8009FDF8[i + 1], &D_8009FA1C[38], 0x7FF8, 0);
                }
            }
        } else {
            for (j = 0; j < 3; j++) {
                x += D_8009FDF4[j + 1];
                arg1 = func_8001FD20(arg0, arg1, x, y + D_8009FDF8[i + 1] + 8, D_8009FE0C[j], 2, 0xFF, 0xFF, 0xFF);
            }
        }
        if (i == D_800F4D3C) {
            arg1 = func_80020508(arg0, arg1, *(short *)(D_800F3AFC + arg2 * 0x1C + 4), y + D_8009FDF8[i + 1] - 4, sp40, 0x18, 0x20, 0x20, 0x20);
        }
    }

    arg1 = func_8001FCC8(arg0, arg1, 0x38);
    y += 0x1C;
    x = *(short *)(D_800F3AFC + arg2 * 0x1C + 4);
    arg1 = func_8001FD20(arg0, arg1, x, y, sp40, 2, 0xFF, 0xFF, 0xFF);
    y -= 0x1C;
    for (i = 0; i < 4; i++) {
        x = *(short *)(D_800F3AFC + arg2 * 0x1C + 4);
        for (j = 0; j < 3; j++) {
            x += D_8009FDF4[j + 1];
            arg1 = func_8001FD20(arg0, arg1, x, y + D_8009FDF8[i], 2, 0x10, 0xFF, 0xFF, 0xFF);
        }
    }

    return arg1;
}
