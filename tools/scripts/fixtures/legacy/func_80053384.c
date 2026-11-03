/* RODATA 0x80011178 0x14 */
extern struct {
    char pad0[0x2670];
    int f2670;
    char pad1[0x2678 - 0x2674];
    int f2678;
    char pad2[0x269C - 0x267C];
    int f269C;
    char pad3[0x26BC - 0x26A0];
    int f26BC;
    char pad4[0x26D0 - 0x26C0];
    char blk[0x1C];
} D_800F4DB8;
extern unsigned int D_800F7430;
extern int func_80052B48();
extern int func_8008C0D8();
extern int CdControl();
extern int func_80096434();

void func_80053384()
{
    int r;
    switch (D_800F7430) {
    case 0:
        D_800F4DB8.f269C = 4;
        D_800F4DB8.f26BC = 0;
        func_80052B48(0);
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 1:
        if (func_8008C0D8(1, 0) == 0) break;
        D_800F4DB8.f2678 = D_800F4DB8.f2678 + 1;
    case 2:
        if (CdControl(9, 0, 0) == 0) break;
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
        D_800F4DB8.f269C = -1;
        D_800F4DB8.f2670 = -1;
        D_800F4DB8.f2678 = 0;
        func_80096434(D_800F4DB8.blk, 0x1C);
        break;
    }
}
