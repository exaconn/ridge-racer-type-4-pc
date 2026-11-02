struct B4 { char b[4]; };
extern char *D_800ABEA4;
extern char *D_800ACBE8;
extern int D_800ABECC;
extern short D_800AC53C;
extern short D_800ACC74;
extern int D_800F2EB8;
void func_800292A8(char *a)
{
    struct B4 sp;
    int i = 0;
    int n;
    register char *p asm("$8");
    D_800ACBE8 = a + 4;
    sp = *(struct B4 *)a;
    n = *(int *)(a + 0x804);
    D_800ABEA4 = p = a + 0x808;
    D_800F2EB8 = 0;
    D_800ABECC = n;
    if (n > 0) {
        int m = n;
        register char *q asm("$4") = p;
        do {
            D_800F2EB8 += *(short *)(q + 0x1C);
            q += 0x3C;
            i++;
        } while (i < m);
    }
    D_800AC53C = D_800F2EB8 / 1024;
    D_800ACC74 = (D_800F2EB8 >> 8) + 1;
}
