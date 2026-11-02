/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char D_800ABAC2[];
extern void (*D_800ABABC)();
extern char D_80011E64[];
int func_80092C58(a0) int a0; {
 int o;
 if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011E64, a0);
 o = *(int *)(D_800ABAC2 + 0xA);
 *(int *)(D_800ABAC2 + 0xA) = a0;
 return o; }
