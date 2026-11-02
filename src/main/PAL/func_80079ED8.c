extern int func_80096484();
extern int func_8009305C();
extern int func_80092D50();
extern char D_80011564[];
extern short D_8010EF98[];

void func_80079ED8(a0, a1, a2, a3, a4, a5)
char *a0;
int a1;
short a2;
short a3;
int a4;
int a5;
{
    int i;
    int *new_var;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x13;
    a0[3] = 1;
    func_80096484(a0 + 4, D_80011564, a1);
    D_8010EF98[0] = a2;
    D_8010EF98[2] = 0x10;
    D_8010EF98[1] = a3;
    D_8010EF98[3] = 1;
    func_8009305C(D_8010EF98, a0 + 0x60);
    func_80092D50(0);
    i = 0;
    do {
        D_8010EF98[0] = a4;
        D_8010EF98[1] = a5;
        D_8010EF98[2] = 4;
        D_8010EF98[3] = 0x10;
        /* (new_var = &a4) is a dead address-of that keeps gcc from hoisting the li 0x10 out of the loop (found by permuter) */
        func_8009305C(D_8010EF98, a0 + 0x80 + (*(new_var = &a4), i) * 0x80);
        func_80092D50(0);
        i++;
        a4 += 4;
    } while (i < 3);
}
