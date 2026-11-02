/* CC1_FLAGS: -mno-split-addresses */
typedef struct { char pad[0x3C]; char *f3c; } T;
extern int (*D_800ABC80)();
extern int D_800ABCE8;
extern int func_80098578();
void func_800996C4(T *a0) {
    int r = D_800ABC80(a0);
    char *p = a0->f3c;
    D_800ABCE8 = r;
    *p = 0;
    func_80098578(a0, -2);
}
