extern int func_8006F1EC();
extern int func_8006F260();
extern int D_800F3820;

int func_8006F288(int a0) {
    int t = func_8006F1EC(0x1F800008, a0);
    return *(int *)(((t * 8) + func_8006F260(0x1F800008)) * 4 + D_800F3820);
}
