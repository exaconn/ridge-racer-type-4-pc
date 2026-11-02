extern char *(*D_800ABC7C)();
int func_800979AC(a0, a1, a2) int a0, a1, a2; {
  char *p = D_800ABC7C();
  switch (a1) {
  case 1: return *(unsigned char *)(p + 0xE8);
  case 2: return *(unsigned short *)(p + 0xE6);
  case 3: return *(unsigned char *)(p + 0xE4);
  case 4:
    if (a2 < 0) return *(unsigned char *)(p + 0xE3);
    if (a2 < *(unsigned char *)(p + 0xE3)) return (*(unsigned short **)p)[a2];
    return 0;
  case 100: return *(int *)(p + 0x4C);
  }
  return 0;
}
