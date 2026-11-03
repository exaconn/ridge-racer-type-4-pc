extern int func_800207D4();
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8003189C();
extern int func_80031CE4();
extern int func_8003A198();
extern int func_8003B398();
extern int func_8003FBA8();
extern int func_8004F24C();
extern int func_80053AF4();
extern int func_80053CB0();
extern int D_800ABEB0;
extern unsigned short D_800AC5D0;
extern int D_800ACC14;
extern int D_800ACC18;
extern struct { char pad[1]; signed char b1; } *D_800F2AB0;
extern int D_800F2DE4;
extern short D_800F3A26;
extern struct { char pad[8]; unsigned short h8; char pad2[0x1E]; int w28; } D_800F3A38;
extern unsigned short D_800F3A40;
extern struct { char pad[0x10]; short h10; char pad2[10]; } *D_800F3AFC;
extern unsigned short D_800F4C5C;
extern short D_800F4C6A;
extern int D_800F4D74;
extern unsigned short D_800F4D8A;
extern struct { char pad[6]; signed char b6; char pad2[0xF]; short h16; } *D_800FF68C;
extern int func_8001FD20();

void func_80031244(void);
void func_8003153C(void);

void func_80031244(void) {
    int s0;
    void *tab;
    int a1;
    D_800F2DE4 += 1;
    D_800ABEB0 += 1;
    if (func_8003FBA8(0) > 0) {
        func_8003189C();
    }
    func_80031CE4();
    if (D_800FF68C->b6 == 4 || (D_800F3A40 & 0x800)) {
        s0 = D_800F3AFC[D_800FF68C->h16].h10;
        if (s0 == 3) {
            if (D_800F3A38.w28 != 0 || (D_800F3A38.h8 & 0x800)) {
                func_8004F24C(1);
                func_8002AF8C(0, 1, 0);
                tab = D_800F3AFC;
                ((typeof(D_800F3AFC))tab)[D_800FF68C->h16].h10 = 4;
                D_800F2AB0->b1 = s0;
                D_800AC5D0 = 0;
                if (D_800F4C5C == 0x10F || D_800F4C5C == 0x12A || D_800F4C5C == 0x205) {
                    D_800AC5D0 = 0x40;
                    func_8003A198(tab);
                    D_800ACC18 = D_800F4D74;
                    func_8003B398(0);
                    func_80053AF4(0, (short)(D_800AC5D0 - 4));
                    D_800F4D8A = 1;
                } else if (D_800F4C5C == 0x111) {
                    D_800AC5D0 = 0x40;
                    func_80053AF4(0, 0x3C);
                    D_800F4D8A = 1;
                }
            }
        }
    }
    if (D_800F4D8A == 1) {
        if ((short)D_800AC5D0 > 0) {
            D_800AC5D0 -= 1;
        }
        func_800207D4(D_800ACC14 + 0x70, (0x40 - (short)D_800AC5D0) * 4, 0x45);
    }
    if (D_800F3AFC[D_800FF68C->h16].h10 == 0xB) {
        if (D_800F2AB0->b1 == 5) {
            if (D_800F4C5C == 0x10F || D_800F4C5C == 0x12A) {
                if (func_8003B398(1) != 1) goto done;
                func_80053CB0();
            if ((short)D_800AC5D0 <= 0) {
                unsigned c = D_800F4C5C;
                D_800F4D8A = 0;
                D_800F4C6A = c >> 8;
                D_800F3A26 = c & 0xFF;
            }
            } else if (D_800F4C5C == 0x205) {
                func_80053CB0();
            if ((short)D_800AC5D0 <= 0) {
                unsigned c = D_800F4C5C;
                D_800F4D8A = 0;
                D_800F4C6A = c >> 8;
                D_800F3A26 = c & 0xFF;
            }
            } else {
            if ((short)D_800AC5D0 <= 0) {
                unsigned c = D_800F4C5C;
                D_800F4D8A = 0;
                D_800F4C6A = c >> 8;
                D_800F3A26 = c & 0xFF;
            }
            }
        }
    }
done:
    func_8002A344(0x1000A);
}

void func_8003153C(void) {
    int *g = (int *)0x1F800000;
    int b = *g;
    *g = func_8001FD20(D_800ACC14 + 0xB6C, b, 0, 0, 0x140, 0x200, 0x20, 0x20, 0x20);
}
