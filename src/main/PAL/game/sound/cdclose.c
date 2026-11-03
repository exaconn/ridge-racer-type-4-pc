/* RODATA 0x80011160 0x14 */
extern struct {
    char pad0[0x2670];
    int f2670;
    char pad1[0x2678 - 0x2674];
    int f2678;
    char pad2[0x2688 - 0x267C];
    int f2688;
    char pad3[0x2694 - 0x268C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26B8 - 0x26A0];
    int f26B8;
} D_800F4DB8;
extern unsigned int D_800F7430;
extern int func_80052ADC();
extern int func_8008C0D8();
extern int func_8008C140();

void func_80053120()
{
    int r;
    switch (D_800F7430) {
    case 0:
        D_800F4DB8.f269C = 2;
        func_80052ADC(0);
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 1:
        if (func_8008C0D8(1, 0) == 0) break;
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 2:
        if (func_8008C140(9, 0, 0) == 0) break;
        goto inc;
    case 3:
        r = func_8008C0D8(1, 0);
        if (r == 2) {
    inc:
            D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
        } else if (r == 5) {
            D_800F7430 = 1;
        }
        break;
    case 4:
        D_800F4DB8.f2670 = -1;
        D_800F4DB8.f2678 = 0;
        if (D_800F4DB8.f26B8 == 1) D_800F4DB8.f2694 = D_800F4DB8.f2688;
        break;
    }
}
