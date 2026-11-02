/* CC1_FLAGS: -mno-split-addresses */
extern int func_80098788();
extern int (*D_800ABC80)();
extern int D_800ABCB4, D_800ABCE0, D_800ABCE8;
int func_800997E4(a0) unsigned char *a0; {
  int a1, v, t;
  if (D_800ABCE8 != 0) {
    D_800ABC80(*(int *)(a0 + 0xC) + 0x1E0);
    D_800ABC80(*(int *)(a0 + 0xC) + 0x2D0);
  }
  a1 = 0;
  if (a0[0x37] == 0) a1 = D_800ABCB4;
  v = func_80098788(a0, a1);
  if (v < 0) return v;
  t = (v & 0xF) * 2;
  D_800ABCE0 = t;
  if (t == 0) D_800ABCE0 = 0x20;
  return 0;
}
