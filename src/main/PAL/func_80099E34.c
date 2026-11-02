/* GCC_VERSION: 2.7.2 */
void func_80099E34(a0) unsigned char *a0; {
  int i;
  unsigned char *p;
  unsigned char c;
  if (a0[0x49] != 0) {
    p = a0 + 0x5D;
    c = 0xFF;
    i = 5;
    a0[0x49] = 0;
    a0[0x46] = 0;
    *(short *)(a0 + 0xE6) = 0;
    *(int *)(a0 + 0x14) = 0;
    *(int *)(a0 + 0x18) = 0;
    a0[0xE3] = 0;
    a0[0xE4] = 0;
    *(short *)(a0 + 0xE6) = 0;
    a0[0xE9] = 0;
    a0[0xEA] = 0;
    *(int *)a0 = 0;
    *(int *)(a0 + 4) = 0;
    *(int *)(a0 + 8) = 0;
    do {
      *p = c;
      i--;
      p++;
    } while (i >= 0);
  }
}
