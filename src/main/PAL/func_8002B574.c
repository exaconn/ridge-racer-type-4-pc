int func_8002B574(int a, int b)
{
    int q = a / 25;
    a -= q * 25;
    a = a * 40 + b;
    return q * 1000 + a;
}
