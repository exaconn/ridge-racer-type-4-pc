typedef struct { char pad[0x49]; char f49; char pad2[8]; short f52; char pad3[8]; } T;
extern T D_800F3A38[];
void func_8004BAD4(int i, short a, char b) {
    T *t = &D_800F3A38[i];
    t->f52 = a;
    t->f49 = b;
}
