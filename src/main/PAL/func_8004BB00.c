typedef struct { char pad[0x49]; char f49; char pad2[6]; short f50; short f52; char pad3[8]; } T;
extern T D_800F3A38[];
extern int func_8004BB84();
void func_8004BB00(int i) {
    T *t = &D_800F3A38[i];
    t->f50 = 1;
    func_8004BB84(i, 2);
    t->f49 = 0xF;
}
