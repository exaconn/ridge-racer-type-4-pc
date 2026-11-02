extern int D_8010718C;
typedef struct { char pad[0xF0]; int f; char pad2[0x320 - 0xF4]; } S;
extern S D_800ABEF8[];
extern int func_80026998();

int func_8006F188(int *a0, int a1) {
    if (D_8010718C == 0) {
        return func_80026998(a0[0], a0[2]);
    }
    return D_800ABEF8[a1].f;
}
