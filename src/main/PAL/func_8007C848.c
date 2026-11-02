typedef struct { char pad[4]; unsigned char b; char pad2[3]; } E8;
extern E8 D_800A4258[]; extern unsigned char D_800A44A8[]; extern short D_800F3A1C; extern unsigned char D_800F3A24[]; extern short D_800FFC00;
void func_8007C848(void) {
 E8 *e = D_800A4258; int s = D_800F3A1C;
 unsigned char *q = D_800A44A8 + e[s].b*4;
 int x = q[0], y = q[1];
 unsigned c = D_800F3A24[D_800FFC00];
 register int d asm("$3"); /* ASM_BARRIER: pin reload value to v1 */
 if (c < 0x10 || x == (d = *(volatile unsigned char *)&D_800F3A24[D_800FFC00])) { D_800F3A1C = y; return; }
 if (d == 0x14 && s == 2) D_800F3A1C = 8;
}
