/* GCC_VERSION: 2.7.2 */
#include "rr/channel_advance.h"

int _padChannelToPort();
char *_padPortToChannel();

int _padChannelToPort(a0) char *a0; {
  int i; int r; char *p; i = 0; r = 0x10; p = PadChannelRecordArray; for (; i < 2; i++) { if (a0 == p) return r; r += 0x10; p += 0xF0; } return 0xFF;
}

char *_padPortToChannel(a0) int a0; {
    char *p = PadChannelRecordArray;
    if (a0 & 0xF0) p += 0xF0;
    return p;
}
