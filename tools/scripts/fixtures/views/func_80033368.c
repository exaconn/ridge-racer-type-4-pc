struct T { int pad[2]; int a; int b; int pad2[11]; };

extern int CurrentDrawBuf;

extern int DrawMapBitmapIso();

extern struct T *TrackNodes;

extern int TrackNodeCount;

extern short ReverseCourse;

extern int TrackNodesEnd;

extern int D_8010BFC8;

void func_80033368(int unusedSelection,int unusedMode) {
    struct { int x, y, z; } v;
    int i, r;
    struct T *t;
    int b;
    if (ReverseCourse != 0) {
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
    DrawMapBitmapIso(CurrentDrawBuf + 0x70, TrackNodesEnd, &v);
}
