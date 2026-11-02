struct P { int tag; unsigned char r, g, b, code; short x, y, w, h; };
extern int func_80095A44();
extern int func_80095B24();
extern int func_80095BD4();
struct P *func_80020588(int a0, struct P *a1, short a2, short a3, int a4, int a5, int a6, int a7, int a8) {
    func_80095BD4(a1);
    func_80095B24(a1, 1);
    a1->x = a2;
    a1->y = a3;
    a1->w = a4;
    a1->h = a5;
    a1->r = a6;
    a1->g = a7;
    a1->b = a8;
    func_80095A44(a0, a1++);
    return a1;
}
