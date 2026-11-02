extern int func_80078204();
extern int func_80096484();
extern int func_80096F24();
extern int func_80096F54();
extern int func_80096F64();
extern char D_800113E4[];
extern int D_8010EF78;

int func_80078610(a0, a1, a2, a3, a4, a5)
int a0, a1, a2, a3, a4, a5;
{
    int sp[8];
    int r;
    int v;
    int ret;
    func_80096484(sp, D_800113E4, a0, a1, a2);
    if (a5 == 1) {
        r = func_80096F24(sp, (a4 << 16) | 0x200);
        if (r == -1) {
            return -1;
        }
        func_80096F64(r);
    }
    v = a4 << 13;
    r = func_80096F24(sp, 0x8002);
    if (r != -1) {
        D_8010EF78 = r;
        func_80078204();
        while (func_80096F54(r, a3, v) == 0) {
        }
        ret = 0;
        func_80096F64(r);
    } else {
        ret = -1;
    }
    return ret;
}
