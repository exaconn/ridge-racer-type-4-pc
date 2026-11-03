/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

#define COPY8(dst_arr, src_arr) \
    { \
        u8 *d = (dst_arr); \
        u8 *s = (src_arr); \
        s32 n; \
        if (d != 0) { \
            for (n = 7; n != -1; n--) { \
                *d++ = *s++; \
            } \
        } \
    }

unsigned char *func_8008C5A0(i, p)
int i;
unsigned char *p;
{
  int a;
  int b;
  int c;
  int new_var;
  i += 150;
  a = i / 75;
  b = i % 75;
  new_var = a / 60;
  c = a % 60;
  p[2] = ((b / 10) * 16) + (b % 10);
  p[1] = ((c / 10) * 16) + (c % 10);
  p[0] = ((new_var / 10) * 16) + (new_var % 10);
  return p;
}
