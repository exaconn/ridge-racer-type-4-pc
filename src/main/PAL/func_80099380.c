extern int (*D_800ABC84)();
extern int func_800993E8();
extern int func_80099404();
int func_80099380(a0, a1)
char *a0; int a1;
{
    int r;
    if (D_800ABC84(a0, a1) == 0) {
        /* ASM_BARRIER: keep separate li 1 for return value (original has two li 1) */
        __asm__ volatile("" : "=r"(r) : "0"(1));
        a0[0x46] = 1;
        *(int *)(a0 + 0x14) = (int)func_800993E8;
        *(int *)(a0 + 0x20) = a1;
        *(int *)(a0 + 0x18) = (int)func_80099404;
        return r;
    }
    return 0;
}
