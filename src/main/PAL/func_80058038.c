extern int *func_80057A44();
extern int *func_80057C50();
extern int *func_80057E3C();

void func_80058038(int arg0, int arg1, int arg2) {
    int *g = (int *)0x1F80007C;
    int *a;
    int v;
    int t;
    g[0x54 / 4] = arg2;
    g[0x58 / 4] = *(int *)0x1F800004;
    t = arg0 + arg1 * 0xC;
    g[0] = *(int *)(t + 4);
    a = *(int **)(t + 0xC);
    v = *a;
    a++;
    if (v != 0) {
        do {
            g[1] = v >> 16;
            switch (v & 0xFFFF) {
            case 0:
                a = func_80057E3C(a, g);
                break;
            case 12:
                a = func_80057A44(a, g);
                break;
            case 8:
                a = func_80057C50(a, g);
                break;
            }
            v = *a;
            a++;
        } while (v != 0);
    }
}
