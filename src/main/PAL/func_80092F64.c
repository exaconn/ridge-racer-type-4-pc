/* CC1_FLAGS: -mno-split-addresses */
extern char D_80011ED8[];
extern char *D_800ABAB8;
extern int func_80092DB8();
void func_80092F64(int a, int b, int c, int d)
{
int v; char nv; int t, u, k; nv = b & 0xFF; func_80092DB8(D_80011ED8, a); u = (d & 0xFF) << 16; t = (c & 0xFF) << 8; k = 0x80000000; v = (u | (t | k)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}
