typedef struct { int a[8]; } S32B;
extern S32B D_8010E710;

void func_8007064C(void) {
    int *sp = (int *)0x1F800000;
    sp[7] -= 0x800;
    *(S32B *)0x1F800028 = D_8010E710;
}
