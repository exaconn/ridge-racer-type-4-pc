/* GCC_VERSION: 2.7.2 */
extern int *D_800ABC30;
extern int D_800ABC38[];
int func_80097078(a0) unsigned int a0; {
  int v = a0 & 0xFFFF;
  int *p = D_800ABC30;
  p[1] |= D_800ABC38[v];
  return v < 3;
}
