extern int func_8001FD20();
extern int D_800ACC14;

void func_8003153C(void) {
    int *g = (int *)0x1F800000;
    int b = *g;
    *g = func_8001FD20(D_800ACC14 + 0xB6C, b, 0, 0, 0x140, 0x200, 0x20, 0x20, 0x20);
}
