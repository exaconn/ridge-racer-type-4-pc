extern int func_8004E3B0();
extern struct { char pad[0x56C]; int a, b, c, d; } D_800AC650;
extern struct { char pad[0x10]; int f10; char pad2[0x19CC - 0x14]; int f19cc; } D_800F4DB8;
extern short D_800F4D44;
extern short D_800F3826;
extern short D_800AD508;
extern int D_800F6784;
extern int D_800ACBC8;
extern int D_800F4DC8;

/* RODATA 0x800110a0 0x1c */
void func_8004E414(void) {
    switch (D_800F4D44) {
    case 6:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800F4DB8.f10 = 0;
        D_800AC650.d = D_800F3826 + 1;
        break;
    case 0:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800AC650.d = -1;
        D_800F4DB8.f10 = 0;
        break;
    default:
    case 1:
    case 5:
        D_800F4DB8.f19cc = 0;
        D_800AC650.a = 4;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800AC650.d = 0;
        D_800F4DB8.f10 = 0;
        break;
    case 2:
    case 3:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = func_8004E3B0(1);
        D_800AC650.d = -1;
        D_800F4DB8.f10 = 1;
        break;
    case 4:
        D_800F6784 = 1;
        D_800AC650.a = 5;
        if (D_800AD508 != 0) {
            D_800AC650.b = func_8004E3B0(0);
            D_800AC650.c = func_8004E3B0(1);
        } else {
            D_800AC650.b = func_8004E3B0(2);
            D_800AC650.c = func_8004E3B0(3);
        }
        D_800ACBC8 = -1;
        D_800F4DC8 = 1;
        break;
    }
}
