extern int func_80096434();
extern int *D_8010E8D8;
extern int *D_8010E8DC;
extern int D_8010E8E0;
extern int D_8010ECA8;
extern int *D_8010ECAC;
extern int *D_8010ECB0;
extern int D_8010ECB4;

int func_80077D08(int *a, int b) {
    int n = *a;
    int s;
    a++;
    D_8010E8DC = a;
    D_8010ECB0 = a;
    D_8010E8E0 = b;
    D_8010ECB4 = b;
    a = a + n * 12;
    D_8010ECA8 = n;
    D_8010E8D8 = a;
    s = n * 0x2C;
    D_8010ECAC = a;
    func_80096434(b, s);
    return s;
}
