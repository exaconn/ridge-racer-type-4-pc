/* legacy typing of shared globals in this part of the unit */
#define func_8002B814 ((void (*)())func_8002B814)

#define D_8009FA1C (*(R4_1 (*)[])&D_8009FA1C)

#undef func_8002B814

#undef D_8009FA1C

/* legacy typing of shared globals in this part of the unit */
#define func_8002B814 ((void (*)())func_8002B814)

#undef func_8002B814

extern int func_8002B814();

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
