extern int func_80096EA4();
extern int D_8010EF68, D_8010EF6C, D_8010EF70, D_8010EF74;

int func_8007825C(void) {
    for (;;) {
        if (func_80096EA4(D_8010EF68) == 1) return 1;
        if (func_80096EA4(D_8010EF6C) == 1) return 2;
        if (func_80096EA4(D_8010EF70) == 1) return 3;
        if (func_80096EA4(D_8010EF74) == 1) return 4;
    }
}
