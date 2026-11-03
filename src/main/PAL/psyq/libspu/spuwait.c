/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libspu/spuwait.h"
extern int D_800A4848, D_800A480C, D_800A4840;
extern int func_80096EA4();
int func_8008B0A4(int a0) {
  int r;
  if (D_800A4848 == 1 || D_800A480C == 1) return 1;
  r = func_80096EA4(D_800A4840);
  if (a0 == 1) {
    if (r == 0) {
      while (func_80096EA4(D_800A4840) == 0) ;
    }
    r = 1;
    D_800A480C = r;
  } else if (r == 1) {
    D_800A480C = r;
  }
  return r;
}
