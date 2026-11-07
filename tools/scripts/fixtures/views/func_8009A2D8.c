/* GCC_VERSION: 2.7.2 */

extern char PadChannelRecordArray[];

int _padChannelToPort(a0) char *a0; {
  int i; int r; char *p; i = 0; r = 0x10; p = PadChannelRecordArray; for (; i < 2; i++) { if (a0 == p) return r; r += 0x10; p += 0xF0; } return 0xFF;
}
