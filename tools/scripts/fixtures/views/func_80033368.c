struct T { int pad[2]; int a; int b; int pad2[11]; };

extern int D_800ACC14;

extern int func_800587E4();

extern struct T *TrackNodes;

extern int TrackNodeCount;

extern short D_800AD522;

extern int D_800F2EC4;

extern int D_8010BFC8;

void func_80033368(int unusedSelection,int unusedMode) {
    struct { int x, y, z; } v;
    int i, r;
    struct T *t;
    int b;
    if (D_800AD522 != 0) {
        i = (D_8010BFC8 >> 2) % TrackNodeCount;
    } else {
        int n = TrackNodeCount;
        r = (D_8010BFC8 >> 2) % n;
        r++;
        i = n - r;
    }
    b = (int)TrackNodes;
    t = (struct T *)(i * 0x3C + b);
    v.x = t->a;
    v.z = t->b;
    D_8010BFC8 += 1;
    func_800587E4(D_800ACC14 + 0x70, D_800F2EC4, &v);
}
