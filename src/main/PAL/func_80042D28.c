extern void func_8002B814();
extern char D_8010C150[];

char *func_80042D28(int a, int b, int c) {
    int i = 7;
    char *p = D_8010C150 + i;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    func_8002B814(D_8010C150, a, c);
    D_8010C150[c + 1] = 0x2F;
    func_8002B814(D_8010C150 + c + 2, b, c);
    return D_8010C150;
}
