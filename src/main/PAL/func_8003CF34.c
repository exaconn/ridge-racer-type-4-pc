typedef struct { unsigned short a, b, c, d; } W8;
typedef struct { unsigned short a, b, c; } W6;
typedef struct { unsigned char a, b, c, d; } B4;
extern int func_80091114();
extern int func_80091274();
extern unsigned char D_8009F924[];
extern B4 D_8009F928[];
extern W8 D_8009F934[];
extern W6 D_800ABED0[];

void func_8003CF34(int a0)
{
    short sp[3][3];
    int pad[2];
    int i;
    int x;

    for (i = 0; i < 3; i++) {
        D_800ABED0[i].a = D_8009F934[i].a;
        D_800ABED0[i].b = D_8009F934[i].b;
        D_800ABED0[i].c = D_8009F934[i].c;
        sp[0][i] = D_8009F928[i].a << 4;
        sp[1][i] = D_8009F928[i].b << 4;
        sp[2][i] = D_8009F928[i].c << 4;
    }
    x = D_8009F924[0] * a0;
    {
        int y = D_8009F924[1] * a0;
        x >>= 8;
        y >>= 8;
        func_80091274(x, y, (D_8009F924[2] * a0) >> 8);
    }
    func_80091114(sp);
}
