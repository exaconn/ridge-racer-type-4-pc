/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "psyq/libsnd/vmmain.h"
#include "rr/channels.h"

void _SsVmSeKeyOn();
int _SsVmSeKeyOff(int a0, int a1, unsigned short a2);
void KeyOnCheck(void);

void _SsVmSeKeyOn(a0, a1, a2, a3, a4, a5)
int a0;
int a1;
int a2;
int a3;
int a4;
int a5;
{
  int new_var;
  int y;
  int x;
  int v;
  int r;
  x = a4 & 0xFFFF;
  y = a5 & 0xFFFF;
  if (x == y)
  {
    v = 0x40;
    r = a4;
  }
  else
    if (((unsigned) y) < (x))
  {
    new_var = (y << 6) / x;
    r = a4;
    v = new_var;
  }
  else
  {
    r = a5;
    v = 0x7F - ((x << 6) / y);
  }
  _SsVmKeyOn(0x21, (short) a0, (short) a1, a2 & 0xFFFF, r & 0xFFFF, v & 0xFFFF);
}

int _SsVmSeKeyOff(int a0, int a1, unsigned short a2) { return _SsVmKeyOff(0x21, (short)a0, (short)a1, a2); }

void KeyOnCheck(void) {

}
