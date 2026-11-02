extern int func_8008D730();
extern int func_8008D820();
extern int func_8008D86C();
int func_8008BFD0(a0)
    int a0;
{
    if (a0 == 2) {
        func_8008D820();
        return 1;
    }
    if (func_8008D86C() != 0) return 0;
    if (a0 == 1) {
        int r = func_8008D730();
        if (r != 0) return 0;
    }
    return 1;
}
