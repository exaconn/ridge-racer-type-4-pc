void SetDrawMove(int *p, short *s, unsigned short a2, int a3) {
    char c = 5;
    if (s[2] == 0 || s[3] == 0) {
        c = 0;
    }
    p[1] = 0x1000000;
    p[2] = 0x80000000;
    ((char*)p)[3] = c;
    p[3] = *(int*)s;
    p[4] = (a3 << 16) | a2;
    p[5] = *(int*)(s+2);
}
