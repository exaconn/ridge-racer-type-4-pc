/* CC1_FLAGS: -mno-split-addresses */
extern char D_80011ECC[];
extern char *D_800ABAB8;
extern int func_80092DB8();
void func_80092ED4(int a, int b, int c, int d)
{
int v; char nv; nv = b & 0xFF; func_80092DB8(D_80011ECC, a); v = (((d & 0xFF) << 16) | ((c & 0xFF) << 8)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}
