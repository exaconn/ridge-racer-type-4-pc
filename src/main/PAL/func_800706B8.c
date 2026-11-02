extern int func_80095BB4(a0);
extern int func_80095A44();
extern unsigned short D_800F4CE0;

struct P { int w; int x; short a, b, c, d; };

struct P *func_800706B8(struct P *a0) {
    
    int s1 = *(int *)0x1F800004;
    func_80095BB4(a0);
    a0->a = 0x60;
    a0->c = 0x80;
    ((char *)a0)[4] = 0;
    ((char *)a0)[5] = 0;
    ((char *)a0)[6] = 0;
    a0->d = 0x20;
    a0->b = D_800F4CE0 - 2;
    func_80095A44(s1, a0++);
    return a0;
}
