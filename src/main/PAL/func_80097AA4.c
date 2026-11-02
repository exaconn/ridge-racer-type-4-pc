/* CC1_FLAGS: -mno-split-addresses */
/* RODATA 0x800120d8 0x14 */
extern unsigned char *(*D_800ABC7C)();
int func_80097AA4(a0, a1, a2) int a0, a1, a2; {
  unsigned char *p = D_800ABC7C();
  unsigned char *q;
  if (a1 < 0) return p[0xE9];
  if (a1 < p[0xE9]) {
    q = *(unsigned char **)(p + 4) + a1 * 5;
    switch (a2) {
    case 1: return q[0];
    case 2: return q[1];
    case 3: return q[2];
    case 4: return q[3];
    case 5: return q[4];
    }
  }
  return 0;
}
