/* GCC_VERSION: 2.7.2 */
extern signed char D_80110100[]; extern char *D_801100F8;
unsigned char func_80089ED4(a0, a1) char *a0; char *a1; {
  unsigned char n = 0; signed char i = 0;
  if (D_80110100[0] > 0) {
    do {
      int ie = i;
      char *t = (char *)(((D_80110100[7] << 4) + ie) * 32 + (int)D_801100F8);
      if (!(t[6] > D_80110100[2]) && !(t[7] < D_80110100[2])) {
        a1[n] = t[0x16];
        a0[n++] = i;
      }
      i++;
    } while (i < D_80110100[0]);
  }
  return n; }
