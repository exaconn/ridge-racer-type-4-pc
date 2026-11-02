typedef struct { char pad[0x1C]; short f1c; char pad2[0x3E]; } T;
extern T D_800F3A38[];
extern int func_80097C58();
void func_8004C098(int i) {
    if (D_800F3A38[i].f1c != 0) {
        func_80097C58(i * 16, 1, 0);
    }
}
