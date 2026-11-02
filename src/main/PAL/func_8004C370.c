typedef struct { char p0; unsigned char f1; char p2[6]; unsigned short f8; char pa[0x52]; } T;
typedef struct { short a[8], b[8], c[8]; } U;
extern T D_800F3A38[];
extern U D_800F2F78[];
int func_8004C370(int i) {
    T *t = &D_800F3A38[i];
    switch (t->f1) {
    case 0x41:
    case 0x73:
    default:
        return D_800F3A38[i].f8 & D_800F2F78[i].a[6];
    case 0x23:
        return t->f8 & D_800F2F78[i].b[6];
    case 0xE3:
        return t->f8 & D_800F2F78[i].c[6];
    }
}
