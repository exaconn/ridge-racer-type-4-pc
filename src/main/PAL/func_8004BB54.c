typedef struct { char pad[0x49]; char f49; char pad2[6]; short f50; short f52; char pad3[8]; } T;
extern T D_800F3A38[];
void func_8004BB54(int i) {
    T *t = &D_800F3A38[i];
    t->f50 = 0;
    t->f49 = 0;
    t->f52 = 0;
}
