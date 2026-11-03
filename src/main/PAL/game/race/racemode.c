typedef struct { char p0[0x1E0]; int a; unsigned char b; char p1[0x13B]; } S2;
extern S2 D_800ABEF8[];
extern int D_800AC3F8;
void func_800337A8(S2 *a0) {
    D_800ABEF8[1] = *a0;
    *(short *)&a0->b = a0->b;
    D_800AC3F8 = a0->a + 2;
}
