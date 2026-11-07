/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char D_800ABAC2[];
extern void (*GPU_printf)();
extern struct { char pad[0x10]; int (*f)(); } *GpuFuncTable;
extern char D_80011E80[];
extern int GPU_memset();
void SetDispMask(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) GPU_printf(D_80011E80, a0);
 if (a0 == 0) GPU_memset(p + 0x6A, -1, 0x14);
 GpuFuncTable->f(a0 ? 0x3000000 : 0x3000001); }
