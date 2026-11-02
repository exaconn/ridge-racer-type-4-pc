typedef struct { int a, b, c, d, e, f, g, h; } w3g6_32;
typedef struct { int x, y; } __attribute__((packed)) w3g6_U;
extern w3g6_32 D_800A3B00;
extern w3g6_U D_800A3B20[];
extern w3g6_U D_800A3B28[];
extern w3g6_U D_800A3B30[];
extern int D_800A3B38[];
extern int D_800ABE98;
extern w3g6_32 D_800AD570;
extern short D_800AD5BC[];
extern w3g6_U D_800F38F8[];
extern short D_800F3914;
extern int D_800F4D48;
extern int D_800F4D68;
extern int D_800F4D84;
extern int D_80107194;
extern int D_801093F8;

void func_8006E63C(void)
{
    int *t;
    int *b;
    int i;
    D_800AD570 = D_800A3B00;
    D_800F38F8[0] = D_800A3B20[0];
        D_800F38F8[1] = D_800A3B28[0];
    D_800F38F8[2] = D_800A3B30[0];
    D_800AD5BC[0] = 0x1000;
    D_800AD5BC[1] = 0x1000;
    D_800AD5BC[2] = 0x1000;
    D_800ABE98 = 0x13;
    D_801093F8 = 0x1D;
    D_80107194 = 0x1C;
    b = D_800A3B38;
    i = D_800F3914;
    t = b + i * 3;
    D_800F4D84 = t[0];
    D_800F4D68 = t[1];
    D_800F4D48 = t[2];
}
