void func_8002B7B4(d, s, n) char *d; unsigned char *s; int n; {
    int i = 0;
    if (*s != 0) do { unsigned char c = *s; s++; d[i] = c; i++; } while (*s != 0);
    if (i < n) do { d[i] = 0x20; i++; } while (i < n);
    d[i] = 0;
}
