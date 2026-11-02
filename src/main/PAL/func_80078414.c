extern char D_8010ECD0[];
int func_80078414(int a0, unsigned char *a1) {
    int t1 = 0, t2 = 0;
    char *t0;
    int a3, v1;
    unsigned char a2;
    char *b = D_8010ECD0;
    if (a0 <= 0) goto end;
    t0 = b;
outer:
    a3 = 0;
    v1 = a3;
top:
    a2 = a1[v1];
    if (a2 == 0) goto l64;
    if (a2 != (unsigned char)t0[v1]) goto l74;
    v1++;
    if (v1 < 0x15) goto top;
l64:
    t1++;
    if (a3 != 0) goto l7c;
    t2 = 1;
    goto end;
l74:
    a3++;
    goto l64;
l7c:
    t0 += 0x28;
    if (t1 < a0) goto outer;
end:
    return t2;
}
