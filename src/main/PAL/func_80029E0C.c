extern int D_800AC5A8;
extern short D_800AC5D0;
extern int D_800F2EB8;
extern int D_800F3A30;
extern int D_800F3A60;
extern int D_800FF6B0;
extern int D_80107188;
extern int func_8004FC18();

void func_80029E0C(a0) char *a0; {
    char *a2 = a0 + 0x1F4;
    int a1 = *(short *)(a2 + 0xB6);
    if (*(int *)(a0 + 0x188) + *(int *)(a0 + 0x184) >= a1 * D_800F2EB8) {
        unsigned short u = *(unsigned short *)(a2 + 0xB6);
        if (D_800F3A30 >= a1) {
            *(short *)(a2 + 0xB6) = u + 1;
            if (D_800F3A30 < (short)(u + 1)) {
                if (D_800FF6B0 < 4) {
                    if (D_800AC5D0 == -1) {
                        D_800FF6B0 = 4;
                        D_800AC5D0 = 0x4B;
                        func_8004FC18(0, 0x47);
                    }
                }
            }
        }
    }
    if (D_800AC5A8 >= D_80107188 - 0x4B && D_800AC5D0 == -1) {
        D_800AC5D0 = 0x25;
        func_8004FC18(0, 0x21);
    } else if (D_800F3A60 != 0) {
        if (D_800AC5D0 == -1) {
            D_800AC5D0 = 0x25;
            func_8004FC18(0, 0x21);
        }
    }
}
