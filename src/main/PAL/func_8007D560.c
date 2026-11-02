extern int func_8003EC88();
extern char *D_800F4D80;
extern short D_800FFC18;
void func_8007D560(int a0, int a1) {
 int n, i;
 if (D_800FFC18 == 0) n = 4; else if (D_800FFC18 == 6) n = 3; else n = 2;
 i = 0;
 if (n != 0) {
  do {
   func_8003EC88(i + 0x22);
   if (a1 == 1) {
    if (i == a0) *(D_800F4D80 + i*0x24 + 0x4D4) = 2;
    else *(D_800F4D80 + i*0x24 + 0x4D4) = 0;
   } else {
    signed char *p = (signed char *)(i*0x24 + (int)D_800F4D80) + 0x4C8;
    if (p[0xC] == 2) p[0xC] = 1;
   }
   i++;
  } while (i < n);
 }
}
