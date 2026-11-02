extern int func_80020CD4();
extern char D_8009F008[];

void func_8002B5D0(a0, a1, b, a3, off)
int a0; int a1; int b; int a3; int off;
{
    if (b >= 0) {
        int m = b / 1000;
        int r = m % 60;
        int t;
        m = m / 60;
        t = b % 1000;
        D_8009F008[1] = m + 0x30;
        D_8009F008[6] = t / 100 + 0x30;
        D_8009F008[7] = t / 10 % 10 + 0x30;
        D_8009F008[8] = t % 10 + 0x30;
        D_8009F008[3] = r / 10 + 0x30;
        D_8009F008[4] = r % 10 + 0x30;
    } else {
        D_8009F008[1] = 0x3A;
        D_8009F008[3] = 0x3A;
        D_8009F008[4] = 0x3A;
        D_8009F008[6] = 0x3A;
        D_8009F008[7] = 0x3A;
        D_8009F008[8] = 0x3A;
    }
    func_80020CD4(a0, a1, D_8009F008 + off, a3);
}
