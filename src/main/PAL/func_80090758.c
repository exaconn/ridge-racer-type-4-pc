extern int func_800916B8();
extern int func_80090604();
int func_80090758(a0)
int a0;
{
    int t, s, a;
    if (a0 == 0) return 0;
    t = 8 - func_800916B8(a0);
    if (t >= 0) {
        s = t >> 1;
        a = a0 >> (s * 2);
    } else {
        s = (t >> 1) + 1;
        a = a0 << -(s * 2);
    }
    s -= 6;
    if (s >= 0) return func_80090604(a) << s;
    return func_80090604(a) >> -s;
}
