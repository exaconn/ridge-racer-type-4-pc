extern int D_8009DF98, D_80109440, D_80109444, D_80109448, D_8010944C, D_80109450, D_80109454;
extern int func_80012210();

int func_80013514(int a0, int a1, int a2, int a3, int a4)
{
    int r;
    register int p0 asm("$17") = a0;
    register int p1 asm("$16") = a1;
    register int p2 asm("$18") = a2;
    register int p3 asm("$19") = a3;
    if (D_8009DF98 != 0) {
        return 0;
    }
    D_80109440 = func_80012210();
    r = 1;
    D_80109444 = a4;
    D_80109448 = p1;
    D_8010944C = p0;
    D_80109450 = p2;
    D_80109454 = p3;
    D_8009DF98 = r;
    return r;
}
