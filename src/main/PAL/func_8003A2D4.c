/* RODATA 0x80010774 0x4c */
extern int func_8001ECE8();
extern int func_80092CB8();
extern int func_80036068();
extern int func_8002A604();
extern int func_8003A1B4();
extern int func_8003A198();
extern int func_80059314();
extern int func_8005CA88();
extern int D_800F2DE4;
extern int D_800AC5DC;
extern int D_800F4D40;
extern unsigned short D_800F4BE8;
extern int D_800ACC18;
extern unsigned short D_800FFC10;
extern int D_800F4D70;
extern short D_800F4D44;
extern int D_800102F0;
extern signed char D_800AD4D8[];
extern short D_800F3826;
extern short D_800F4C6A;
extern short D_800F3A26;
extern short D_800F4D8A;

void func_8003A2D4() {
    int a0, a1, a3;
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800ACC18 = D_800F4D40;
    a3 = 0;
    switch (D_800F4BE8) {
    case 0:
        func_8001ECE8(0, 0, 0, 1);
        D_800FFC10 = 1;
        D_800ACC18 = D_800F4D70;
        goto l428;
    case 10:
        if (D_800F4D44 == 5) goto five;
        func_8001ECE8(0, 0, 0, 1);
        D_800FFC10 = 1;
        D_800ACC18 = D_800102F0;
        goto l428;
    five:
        func_8001ECE8(0, 0, 0, 2);
        D_800FFC10 = 0;
        goto l428;
    case 6:
        func_80092CB8(0);
        func_80036068();
        a3 = 1;
    case 1:
    case 4:
        func_8001ECE8(8, 8, 8, a3 | 2);
        D_800FFC10 = 2;
        goto l418;
    default:
        func_8001ECE8(0, 0, 0, 2);
        D_800FFC10 = 0;
    }
l418:
    func_8002A604();
    func_8003A1B4();
l428:
    func_8003A198();
    if (D_800FFC10 == 1) {
        func_80092CB8(0);
        a1 = 0;
        switch (D_800F4D44) {
        case 0: a0 = 8; break;
        case 1:
            a0 = D_800AD4D8[0x17];
            a1 = 3 - D_800AD4D8[a0 + 8];
            break;
        case 2:
        case 3: a0 = 9; break;
        case 4: a0 = 10; break;
        case 6: a0 = D_800F3826 + 0xB; break;
        default:
            a0 = 0;
            a1 = a0;
            D_800FFC10 = 3;
            break;
        }
        if (D_800FFC10 == 1) func_80059314(a0, a1);
    } else if (D_800FFC10 == 0) {
        func_8005CA88();
    }
    D_800F4C6A = 1;
    D_800F3A26 = 0x24;
    D_800F4D8A = 0;
}
