/* RODATA 0x80011148 0x14 */
typedef struct {
    char pad0[0x2670];
    int f2670;
    char pad1[0x2678 - 0x2674];
    int f2678;
    int f267C;
    int f2680;
    char pad2[0x269C - 0x2684];
    int f269C;
    char pad3[0x26A8 - 0x26A0];
    int f26A8;
    int f26AC;
    int f26B0;
    char pad4[0x26C0 - 0x26B4];
    unsigned short h26C0;
} B;
extern B D_800F4DB8;
extern char D_800F7414[];
extern unsigned int D_800F7430;
extern int func_80052B48();
extern int func_8008C0D8();
extern int func_8008C140();
extern int func_8008F274();

void func_80052F2C()
{
    int r;
    int pad[2];
    B *p;
    switch (D_800F7430) {
    case 0:
        if (func_8008C0D8(1, 0) == 0) break;
        if (D_800F4DB8.f269C != -1) break;
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 1:
        if (func_8008C140(0x1B, 0, 0)) {
            D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
            break;
        }
        if (func_8008F274(1) == 0x10) {
            D_800F4DB8.f267C = 1;
            D_800F4DB8.f2680 = -2;
            D_800F4DB8.f2670 = -1;
            D_800F4DB8.f2678 = 0;
        }
        break;
    case 2:
        r = func_8008C0D8(1, 0);
        if (r == 2) {
            D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
            func_80052B48(D_800F4DB8.h26C0);
        } else if (r == 5) {
            D_800F7430 = 1;
        }
        break;
    case 3:
        func_8008C140(1, 0, D_800F7414);
        p = (B *)(D_800F7414 - 0x265C);
        if (*((unsigned char *)p + 0x265C) & 0x20) {
            p->f2678 = p->f2678 + 1;
            break;
        }
        p->f26A8 = p->f26A8 + 1;
        if (p->f26A8 >= 0x169) {
            p->f26B0 = p->f26B0 + 1;
            if (p->f26B0 >= 0x21) p->f26B0 = 0x20;
            p->f2678 = 0;
        }
        break;
    case 4:
        D_800F4DB8.f2670 = -1;
        D_800F4DB8.f2678 = 0;
        D_800F4DB8.f269C = 1;
        D_800F4DB8.f2680 = 0;
        D_800F4DB8.f26A8 = 0;
        D_800F4DB8.f26AC = 0;
        D_800F4DB8.f26B0 = 0;
        break;
    }
}
