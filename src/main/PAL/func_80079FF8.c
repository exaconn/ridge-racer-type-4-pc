extern int func_80092D50();
extern int func_8009305C();
extern int func_80096484();
extern char D_80011564[];
extern char D_80011568[];
extern short D_8010EFA0[];
typedef struct { char b[0x80]; } B;

void func_80079FF8(a0, a1, a2, a3, a4, a5, a6)
unsigned char *a0;
int a1;
short a2;
unsigned short a3;
int a4;
int a5;
B *a6;
{
    int i;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x11;
    a0[3] = 1;
    func_80096484(a0 + 4, D_80011564, a1);
    a0[0x50] = 1;
    a0[0x51] = 0;
    func_80096484(a0 + 0x52, D_80011564, D_80011568);
    D_8010EFA0[0] = a2;
    D_8010EFA0[2] = 0x10;
    D_8010EFA0[1] = a3;
    D_8010EFA0[3] = 1;
    func_8009305C(D_8010EFA0, a0 + 0x60);
    func_80092D50(0);
    i = 0;
    do {
        D_8010EFA0[0] = a4;
        D_8010EFA0[1] = a5;
        D_8010EFA0[2] = 4;
        D_8010EFA0[3] = 0x10;
        func_8009305C(D_8010EFA0, a0 + 0x80 + i * 0x80);
        func_80092D50(0);
        i++;
        a4 += 4;
    } while (i <= 0);
    *(B *)(a0 + 0x100) = *a6;
    *(B *)(a0 + 0x180) = *a6;
    *(B *)(a0 + 0x200) = *a6;
}
