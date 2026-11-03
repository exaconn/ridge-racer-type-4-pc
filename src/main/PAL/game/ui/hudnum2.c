extern int D_800F4BF0;
extern char D_8010BF88[];
extern int func_8001FCC8(), func_80021568();

void func_80021684(int a0, int a1)
{
    char *b = D_8010BF88;
    int off = a1 * 0x14;
    char *t = b + off;
    short x = *(short *)(t + 8);
    short y = *(short *)(t + 10);
    int s1 = *(int *)(D_800F4BF0 * 4 + off + (int)b);
    int h, tn, on;
    h = a0 / 100;
    tn = (a0 / 10) - h * 10;
    on = a0 % 10;
    *(int *)0x1F800000 = func_8001FCC8(s1, func_80021568(s1, func_80021568(s1, func_80021568(s1, *(int *)0x1F800000, x, y, h, 0x7EFA), x + 8, y, tn, 0x7EFA), x + 0x10, y, on, 0x7EFA), 5);
}
