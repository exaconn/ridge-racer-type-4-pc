extern int func_80034E3C();
extern int func_8003EC88();
extern int D_800F3A38[];
extern short D_800F4CD0;
extern char *D_800F4D80;
void func_8002F100() {
    int n = D_800F4CD0 + func_80034E3C();
    int i = 0;
    if (n > 0) {
        do {
            if (i == 2) {
                if (D_800F3A38[0x6C / 4] == -1) *(short *)(D_800F4D80 + 0x68) = 1;
                else *(short *)(D_800F4D80 + 0x68) = 0;
            }
            func_8003EC88(i);
            i++;
        } while (i < n);
    }
}
