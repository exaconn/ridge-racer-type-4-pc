/* RODATA 0x800106b0 0x1c */
extern int func_8001FD20();
extern int func_80036068();
extern int func_80092CB8();
extern int D_800AC5DC, D_800ACC14;
extern short D_800F3A26, D_800F4BE8, D_800F4C6A;
extern unsigned short D_800F4D8A;
extern int func_8001ECE8();
extern int func_8004F600();
extern int func_801150CC();
extern int func_80117C78();
extern int func_801154D8();
extern int func_80118634();
extern int func_80115388();
extern int D_800AC5DC, D_800F2DE4, D_8010C0E0;
extern char D_800ACC68;
extern short D_800AD50A, D_800F4D44;

void func_80036098(void);
void func_80036174(void);

void func_80036098(void) {
    if (D_800F4D8A == 0) {
        func_80092CB8(0);
        func_80036068();
        D_800AC5DC = 0x80;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        D_800F4D8A = 2;
    } else if (D_800F4D8A == 2) {
        D_800F4BE8 = 6;
        D_800F4C6A = 1;
        D_800F3A26 = 0x23;
        D_800F4D8A = 0;
    }
    *(int *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, *(int *)0x1F800000, 0, 0, 0x140, 0x100, 8, 8, 8);
}

void func_80036174(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    D_800F2DE4 = 0;
    D_8010C0E0 = 0;
    D_800ACC68 = 2;
    func_8004F600();
    switch (D_800F4D44) {
    case 1:
        func_801150CC();
        D_800AD50A = 0x130;
        break;
    case 2:
    case 3:
        func_80117C78();
        D_800AD50A = 0x13A;
        break;
    case 0:
        func_801154D8();
        D_800AD50A = 0x136;
        break;
    case 4:
        func_80118634();
        D_800AD50A = 0x13E;
        break;
    case 6:
        func_80115388();
        D_800AD50A = 0x153;
        break;
    default:
        D_800AD50A = 0x103;
        break;
    }
}
