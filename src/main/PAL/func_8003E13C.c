/* RODATA 0x800108e8 0x28 */
typedef struct {
    short x, y;
    unsigned short page, clut;
    unsigned char u, v, w, h;
} SpriteDesc;

typedef struct {
    signed char f00;
    signed char f01;
    signed char f02;
    unsigned char f03;
    char f04;
    unsigned char f05;
    unsigned char f06;
    unsigned char f07;
    short x;
    short y;
    short w;
    short h;
    unsigned short page;
    unsigned short clut;
    unsigned char u;
    unsigned char v;
    short pad16;
    int f18;
} MenuRow1C;

extern int func_8001FCC8();
extern void *func_8001FD20();
extern void *func_80020074();
extern void *func_8002018C();
extern void *func_80020508();
extern void func_8003DC9C();
extern int func_8003DCEC();
extern void func_80043608();

extern int D_800ACC14;
extern MenuRow1C *D_800F4D8C;

int func_8003E13C(int arg0) {
    SpriteDesc sp30;
    SpriteDesc sp40;
    MenuRow1C *base;
    MenuRow1C *p;
    int s7;
    unsigned int s5;
    int v0;
    void *a1;

    base = D_800F4D8C;
    p = (MenuRow1C *)((arg0 * 0x1C) + (int)base);
    sp40.page = sp30.page = p->page;
    sp40.clut = sp30.clut = p->clut;
    sp40.x = sp30.x = p->x;
    sp40.y = sp30.y = p->y;
    sp30.y += p->h / 2 - 10;
    sp40.u = sp30.u = p->u;
    sp40.w = sp30.w = p->w;
    sp30.v = p->v + p->h;
    sp30.h = 0x10;
    sp40.v = p->v;
    sp40.h = p->h;

    s7 = p->f02;
    s5 = p->f03;

    switch (s7) {
    case -2:
        D_800F4D8C[arg0].f03 = 0;
        D_800F4D8C[arg0].f02 = -4;
        D_800F4D8C[arg0].f00 = 0;
        break;
    case -3:
        break;
    case -4:
        func_8003DC9C(D_800F4D8C[arg0].f07, D_800F4D8C[arg0].x, D_800F4D8C[arg0].y, D_800F4D8C[arg0].w, D_800F4D8C[arg0].h, 0, 0);
        D_800F4D8C[arg0].f02 = -5;
        break;
    case -5:
        if (func_8003DCEC(D_800F4D8C[arg0].f07) != 0) {
            D_800F4D8C[arg0].f02 = -1;
        }
        break;
    case -6:
        D_800F4D8C[arg0].f03 = 0;
        D_800F4D8C[arg0].f02 = -7;
    case -7:
        func_8003DCEC(D_800F4D8C[arg0].f07);
        if (D_800F4D8C[arg0].f03 >= 0x14) {
            D_800F4D8C[arg0].f02 = -1;
            D_800F4D8C[arg0].f03 = 0;
        } else {
            D_800F4D8C[arg0].f03++;
        }
        break;
    case -1:
        a1 = *(void **)0x1F800000;
        v0 = ((unsigned short)s5 * 0x7C) / 10;
        *(void **)0x1F800000 = (void *)func_8001FCC8(
            D_800ACC14 + 0x74,
            func_80020508(D_800ACC14 + 0x74, a1, D_800F4D8C[arg0].x, D_800F4D8C[arg0].y, D_800F4D8C[arg0].w, D_800F4D8C[arg0].h, v0, v0, v0),
            0x45
        );
        if ((unsigned short)s5 >= 10) {
            D_800F4D8C[arg0].f02 = 0;
            D_800F4D8C[arg0].f03 = 0;
        } else {
            D_800F4D8C[arg0].f03++;
        }
        func_8003DCEC(D_800F4D8C[arg0].f07);
        break;
    case 0:
        a1 = *(void **)0x1F800000;
        if ((unsigned short)s5 >= 3) {
            a1 = func_8002018C(D_800ACC14 + 0x70, a1, &sp30, 1, 0xFF, 3);
        }
        *(void **)0x1F800000 = func_8001FD20(
            D_800ACC14 + 0x74,
            a1,
            D_800F4D8C[arg0].x,
            D_800F4D8C[arg0].y,
            D_800F4D8C[arg0].w,
            D_800F4D8C[arg0].h,
            8, 8, 8
        );
        if ((unsigned short)s5 >= 0x15) {
            D_800F4D8C[arg0].f02 = 1;
            D_800F4D8C[arg0].f03 = 0;
        } else {
            D_800F4D8C[arg0].f03++;
        }
        if (s5 == 1) {
            func_80043608(D_800F4D8C[arg0].f18 + ((D_800F4D8C[arg0].f06 * D_800F4D8C[arg0].f00) << 11));
        }
        break;
    case 1:
        a1 = func_8002018C(D_800ACC14 + 0x70, *(void **)0x1F800000, &sp30, 1, (0x80 - s5) * 2, 2);
        a1 = func_8002018C(D_800ACC14 + 0x70, a1, &sp40, 1, s5, 1);
        *(void **)0x1F800000 = func_8001FD20(
            D_800ACC14 + 0x7C,
            a1,
            D_800F4D8C[arg0].x,
            D_800F4D8C[arg0].y,
            D_800F4D8C[arg0].w,
            D_800F4D8C[arg0].h,
            8, 8, 8
        );
        if (s5 >= 0x80) {
            D_800F4D8C[arg0].f03 = 0;
            D_800F4D8C[arg0].f02 = 2;
        } else {
            D_800F4D8C[arg0].f03 += 8;
        }
        break;
    case 2:
    default:
        a1 = *(void **)0x1F800000;
        if (D_800F4D8C[arg0].f00 >= 0) {
            a1 = func_80020074(
                D_800ACC14 + 0x70,
                a1,
                D_800F4D8C[arg0].x,
                D_800F4D8C[arg0].y,
                D_800F4D8C[arg0].w,
                D_800F4D8C[arg0].h,
                D_800F4D8C[arg0].u,
                D_800F4D8C[arg0].v,
                D_800F4D8C[arg0].page,
                D_800F4D8C[arg0].clut,
                0,
                0x80
            );
        }
        *(void **)0x1F800000 = func_8001FD20(
            D_800ACC14 + 0x74,
            a1,
            D_800F4D8C[arg0].x,
            D_800F4D8C[arg0].y,
            D_800F4D8C[arg0].w,
            D_800F4D8C[arg0].h,
            8, 8, 8
        );
        if (s5 >= D_800F4D8C[arg0].f05 && D_800F4D8C[arg0].f01 >= 2) {
            D_800F4D8C[arg0].f03 = 0;
            D_800F4D8C[arg0].f02 = 0;
            D_800F4D8C[arg0].f00 = ++D_800F4D8C[arg0].f00 % D_800F4D8C[arg0].f01;
        } else {
            D_800F4D8C[arg0].f03++;
        }
        break;
    }

    return s7 >= 0;
}
