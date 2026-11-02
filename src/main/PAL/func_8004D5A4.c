typedef struct { char pad[0x550]; int f550; int f554; int f558; int f55c; } T;
extern T D_800AC650;
void func_8004D5A4(int a) {
    T *t = &D_800AC650;
    t->f554 = a;
    a += 0xA00;
    t->f558 = a;
    a += 0x20;
    t->f55c = a;
    a += 0x200;
    t->f550 = a;
}
