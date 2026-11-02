extern int func_80078204();
extern int func_80096484();
extern int func_80096F24();
extern int func_80096F44();
extern int func_80096F64();
extern char D_800113E4[];
extern int D_8010EF78;

int func_80078490(a0, a1, a2, a3, a4)
int a0, a1, a2, a3, a4;
{
    int sp[8];
    int r;
    int v;
    int ret;
    func_80096484(sp, D_800113E4, a0, a1, a2);
    v = a4 << 13;
    r = func_80096F24(sp, 0x8001);
    if (r != -1) {
        D_8010EF78 = r;
        func_80078204();
        while (func_80096F44(r, a3, v) == 0) {
        }
        ret = 0;
        func_80096F64(r);
    } else {
        ret = -1;
    }
    return ret;
}
