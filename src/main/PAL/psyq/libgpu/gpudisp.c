/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
extern unsigned char D_800ABAC2[];
extern void (*D_800ABABC)();
extern char D_80011E64[];
extern struct { char pad[0x10]; int (*f)(); } *D_800ABAB8;
extern char D_80011E80[];
extern int func_800959B8();

int func_80092C58();
void func_80092CB8();

int func_80092C58(a0) int a0; {
 int o;
 if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011E64, a0);
 o = *(int *)(D_800ABAC2 + 0xA);
 *(int *)(D_800ABAC2 + 0xA) = a0;
 return o; }

void func_80092CB8(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) D_800ABABC(D_80011E80, a0);
 if (a0 == 0) func_800959B8(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }
