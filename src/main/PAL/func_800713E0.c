extern int func_8006F1EC();
extern int func_8006F260();
extern unsigned char *D_8010E798;

int func_800713E0(void) {
    int a = func_8006F1EC(0x1F800008, 0);
    return (D_8010E798 + a * 8)[func_8006F260(0x1F800008)];
}
