/* GCC_VERSION: 2.7.2 */
extern int D_800ABDE8, D_80114724, D_80114728;
int func_8009D2E0() {
    int v = D_800ABDE8;
    int r;
    if (v == 4) goto c4;
    if (v < 5) {
        if (v == 2) goto c2;
        r = 0;
        goto end;
    }
    if (v == 5) goto c5;
    r = 0;
    goto end;
c2:
    r = 0x5E;
    goto end;
c4:
    r = D_80114724;
    goto end;
c5:
    r = D_80114728;
end:
    return r;
}
