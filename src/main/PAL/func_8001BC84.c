extern short D_8009E874[];
struct Ot { int x; int next; };
struct Pk { char pad[3]; char len; int a, b, c, d; };

void func_8001BC84(int a0, int a1)
{
    struct Pk *p = *(struct Pk **)0x1F800000;
    int col = (((-a1) >> 3) & 3) | 0x1BA4F0D0;
    short *t = D_8009E874;
    int i = 11;
    int *g;
    do {
        if (a0 & 1) {
            p->a = 0x65000000; p->b = *t | 0xA80000; p->d = 0x100004; p->c = col;
            g = (int *)0x1F800000;
            *(int *)p = ((struct Ot *)g[1])->next;
            p->len = 4;
            ((struct Ot *)g[1])->next = (int)p;
            p++;
            ((char *)g[1])[7] = 0;
        }
        a0 >>= 1;
        i--;
        t++;
    } while (i > 0);
    *(struct Pk **)0x1F800000 = p;
}
