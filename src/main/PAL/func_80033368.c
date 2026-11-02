extern int func_800587E4();
struct T { int pad[2]; int a; int b; int pad2[11]; };
extern struct T *D_800ABEA4;
extern int D_800ABECC;
extern int D_800ACC14;
extern short D_800AD522;
extern int D_800F2EC4;
extern int D_8010BFC8;

void func_80033368(int unusedSelection,int unusedMode) {
    struct { int x, y, z; } v;
    int i, r;
    struct T *t;
    int b;
    if (D_800AD522 != 0) {
        i = (D_8010BFC8 >> 2) % D_800ABECC;
    } else {
        int n = D_800ABECC;
        r = (D_8010BFC8 >> 2) % n;
        r++;
        i = n - r;
    }
    b = (int)D_800ABEA4;
    t = (struct T *)(i * 0x3C + b);
    v.x = t->a;
    v.z = t->b;
    D_8010BFC8 += 1;
    func_800587E4(D_800ACC14 + 0x70, D_800F2EC4, &v);
}
