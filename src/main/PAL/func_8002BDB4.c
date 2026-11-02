extern char D_8009F020[];

void func_8002BDB4(int t)
{
    int a, b, c;
    /* ASM_BARRIER: pins the D_8009F020 base to $v0 (original allocates it before the digit temporaries) */
    register char *p asm("$2");

    a = t / 1000;
    b = a / 60;
    c = a % 60;
    a = t % 1000;
    p = D_8009F020;
    p[0] = b / 10 + '0';
    p[1] = b % 10 + '0';
    p[3] = c / 10 + '0';
    p[4] = c % 10 + '0';
    p[6] = a / 100 + '0';
    p[7] = a / 10 % 10 + '0';
    p[8] = a % 10 + '0';
}
