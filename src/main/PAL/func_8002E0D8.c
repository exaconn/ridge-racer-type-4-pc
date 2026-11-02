/* RODATA 0x80010488 0x1c */
typedef struct { char pad[0x1E4]; unsigned short f1E4; short f1E6; short f1E8; } T;
extern T *D_800FFC20[];
extern short D_800F4D44;
extern short D_800AC59C;
extern short D_80107178;
extern short D_800AD508;
extern int D_800ABEC4;
extern int func_8002D1B4();
extern int func_8002D784();
extern int func_80015F68();

void func_8002E0D8(a0) int a0; {
    int z; 
    int i, s2, y;
    T **p;
    D_800ABEC4 = 0;
    switch (a0) {
    case 0:
        z = 0;
        if (!(D_800FFC20[0]->f1E4 & 0x100)) {
            func_8002D1B4(D_800FFC20[0], z);
        } else {
            __asm__("" : "=r"(y) : "0"(z)); /* ASM_BARRIER: hide that z==0 from cse so the else-arm copies a1 (move a2,a1) while the then-arm keeps the single hoisted a1=0 */
            func_80015F68(D_800FFC20[0], y, y, y, 0, 0, 0);
        }
    case 1:
        if (D_800F4D44 != 4) {
            for (i = 1; i < D_800AC59C; i++) {
                if (D_800FFC20[i]->f1E8 != -1) func_8002D784(D_800FFC20[i], a0);
            }
        } else {
            if (D_80107178 == 2) {
                { char *bb = (char *)D_800FFC20; func_8002D784(*(T **)(bb + (D_800AD508 << 3)), a0); }
            }
        }
        break;
    case 3:
    case 5:
        D_800ABEC4 = 1;
    case 2:
    case 4:
        s2 = 3;
        if (D_800F4D44 == 4) s2 = 4;
        if (D_80107178 == 2) s2 = 0;
        if (D_800AC59C > 0) {
            i = 0;
            p = D_800FFC20;
            do {
                if (p[0]->f1E8 != -1) {
                    if (a0 - 2 == i) {
                        z = 0;
                        if (!(p[0]->f1E4 & 0x100)) {
                            func_8002D1B4(p[0], z);
                        } else {
                            __asm__("" : "=r"(y) : "0"(z)); /* ASM_BARRIER: hide that z==0 from cse so the else-arm copies a1 (move a2,a1) while the then-arm keeps the single hoisted a1=0 */
                            func_80015F68(p[0], y, y, y, p[0]->f1E6 + D_800ABEC4, 0, 0);
                        }
                    } else {
                        func_8002D784(p[0], s2);
                    }
                }
                p++;
                i++;
            } while (i < D_800AC59C);
        }
        break;
    case 6:
        if (D_800AC59C > 0) {
            i = 0;
            do {
                if (D_800FFC20[i]->f1E8 != -1) func_8002D784(D_800FFC20[i], 2);
                i++;
            } while (i < D_800AC59C);
        }
        break;
    }
}
