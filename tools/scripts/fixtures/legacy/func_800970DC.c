/* GCC_VERSION: 2.7.2 */
#include "rr/counters.h"
int func_800970DC(a0) unsigned int a0; {
  int v = a0 & 0xFFFF;
  if (v >= 3) return 0;
  *(short *)((u8 *)D_800ABC34 + v * 16) = 0; return 1;
}
