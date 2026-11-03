extern int func_8002A344();
extern int func_8003EC88();
extern int func_800416B4();
extern int func_800417EC();
extern int D_8009FDA4;
extern unsigned char D_800AD520;
extern unsigned D_800F2DE4;
extern int D_800F3A60;
extern char *D_800F3AFC;
extern char *D_800F4D80;
extern signed char *D_800FF68C;
void func_80041588()
{
    int a2;
    D_800F2DE4 += 1;
    if (D_800FF68C[4] >= 0x22) {
        func_8003EC88(0);
        func_8002A344(0x10002);
    } else {
        func_8002A344(0x10003);
    }
    if (func_800416B4(D_8009FDA4) != 0) {
        if (*(short *)(D_800F3AFC + 0x10) == 3) {
            func_800417EC();
            return;
        }
    }
    if (D_800FF68C[4] == 1) {
        if (D_800F3A60 == 0) {
            if (D_800F2DE4 < 0x1F5) return;
        }
        a2 = D_800FF68C[6];
        if (a2 == 4) {
            D_800FF68C[4] = D_800AD520 + 0x22;
            D_800F4D80[0x16] = D_800AD520;
            *(short *)(D_800F3AFC + 0x10) = a2;
        }
    }
}
