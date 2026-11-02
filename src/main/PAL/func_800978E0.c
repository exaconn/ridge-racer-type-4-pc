/* GCC_VERSION: 2.7.2 */
extern char *(*D_800ABC7C)();
int func_800978E0() {
  char *p = D_800ABC7C();
  int a;
  if (p[0x37] == 0 && p[0x38] == 0) {
    if (p == *(char **)(p + 0x10) || p[0x39] == 0) {
      if (**(char **)(p + 0x30) == 0) return p[0x49];
    }
  }
  a = p[0x49];
  switch (a) {
  case 3: return 1;
  case 2: return 1;
  case 6: return 4;
  }
  return p[0x49];
}
