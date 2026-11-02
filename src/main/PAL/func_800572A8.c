extern short *D_800A0F20[];
extern short *D_800A0F8C[];
extern char *D_800AC89C;
extern short D_800AD520;
extern int func_8004D290();
extern int func_80056F04();
void func_800572A8(a0, a1)
int a0;
int a1;
{
  unsigned short v[4];
  char w[3];
  short *s1;
  int i;
  int c;
  unsigned short ua;
  s1 = D_800A0F20[*((int *) (D_800AC89C + 0x13C))];
  do { c = a0 * 4;
  if (a0 >= 0x20) {
    if (a1 < a0) c = 0x80 - ((a0 - a1) * 4);
    else c = 0x80;
  } } while (0);
  i = 0; v[0] = 0x7C; v[2] = 0xC4; v[3] = 0xB0; v[1] = 0xB0; w[2] = c; w[1] = c; w[0] = c;
  do { func_8004D290(v, w, 1, 0); i += 1; v[1] += 1; v[3] += 1; } while (i < 2);
  i = 0; v[3] = 0x118; v[1] = 0x118; w[2] = c; w[1] = c; w[0] = c;
  do { func_8004D290(v, w, 1, 0); i += 1; v[1] += 1; v[3] += 1; } while (i < 2);
  ua = s1[2];
  if (s1[2] >= 0) {
    do {
      func_80056F04((short) ua, s1[0], s1[1], c, s1[3]);
      s1 += 4;
      ua = s1[2];
    } while (s1[2] >= 0);
  }
  s1 = D_800A0F8C[D_800AD520];
  ua = s1[2];
  if (s1[2] >= 0) {
    do {
      func_80056F04((short) ua, s1[0], s1[1], c, s1[3]);
      s1 += 4;
      ua = s1[2];
    } while (s1[2] >= 0);
  }
}
