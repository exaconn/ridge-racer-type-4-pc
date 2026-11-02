extern int func_8005599C();
extern int func_80058768();
extern int func_8006EE3C();
extern int func_800701C4();
extern int func_80090F74();
extern struct { char pad[0x3D8]; char *p; } D_800AC650;
extern int D_800F2FD8;

void func_8005C024(void)
{
    char sp10[32];
    char sp30[32];
    int sp50[3];
    int v;

    func_8005599C(D_800AC650.p + 0xC8, D_800AC650.p + 0x20, sp10, sp30);
    v = *(int *)(D_800AC650.p + 0xE0);
    sp50[2] = v;
    sp50[1] = v;
    sp50[0] = v;
    func_80090F74(sp10, sp50);
    func_800701C4(7);
    func_8006EE3C(0x1F80007C, D_800AC650.p + 0xC8, sp10);
    *(int *)0x1F800078 = 0;
    func_80058768(0x1F800000, D_800F2FD8 < 1, 0, 0x100);
}
