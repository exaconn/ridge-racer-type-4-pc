typedef struct { char pad[0x54]; int f54; char pad2[4]; } T;
extern T D_800F3A38[];
int func_8004B70C(int i) {
    return D_800F3A38[i].f54;
}
