/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char D_800ABAC2[];
extern void (*D_800ABABC)();
extern struct { char pad[0x10]; int (*f)(); } *D_800ABAB8;
extern char D_80011E80[];
extern int func_800959B8();
void func_80092CB8(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) D_800ABABC(D_80011E80, a0);
 if (a0 == 0) func_800959B8(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }
