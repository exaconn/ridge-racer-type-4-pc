
extern int func_8001F5B4();
extern int func_8001FCC8();
extern int func_8001FF84();
extern unsigned char D_8009FBEC[];
extern char D_800AD528[];
typedef struct 
{
  char p[0x14];
  short s;
} S1;
typedef struct 
{
  char p[0x10];
  int w;
} S3;
typedef struct 
{
  char p[0x2B];
  signed char c;
} S2;
extern char D_800F3A38[];
extern char *D_800F3AFC;
extern int D_800F4CB8;
void func_80042DBC(a0, a1, a2)
int a0;
int a1;
int a2;
{
  char sp[8];
  int i;
  char *p;
  int s3;
  int s4;
  /* ASM_BARRIER: pin a0 copy to $s5 to match original save-reg allocation */
  register int aa asm("$21");
  int x;
  int nv;
  int y;
  int s2;
  int idx;
  int v1;
  int r;
  char *v0;
  char *g;
  aa = a0;
  g = D_800F3AFC;
  v0 = (char *) ((a2 * 0x1C) + ((int) g));
  x = *((short *) (v0 + 4));
  y = *((short *) (v0 + 6));
  s3 = x + 8;
  s4 = y + 0x24;
  i = 7;
  p = sp + 7;
  do
  {
    *p = 0;
    i--;
    p--;
  }
  while (i >= 0);
  idx = (D_800F4CB8 - 1) & 1;
  if (idx < 0)
  {
    idx = 0;
  }
  v1 = ((S3 *) (D_800F3A38 + (idx * 0x5C)))->w;
  switch (v1)
  {
    case 0:

    default:
      s2 = *((short *) (D_800AD528 + (idx * 4)));
      break;

    case 1:
      s2 = ((S1 *) ((idx * 16) + ((int) D_800AD528)))->s;
      break;

    case 2:
      s2 = ((S2 *) ((idx * 4) + ((int) D_800AD528)))->c;
      break;

  }

  r = func_8001FF84(aa, a1, s3, s4, D_8009FBEC, 0x7FF8, 0);
  sp[0] = s2 + 0x41;
  nv = s3 + 2;
  func_8001FCC8(aa, func_8001F5B4(aa, r, nv + D_8009FBEC[2], s4, sp, 0x7FF8, 0, 0), 0x38);
  /* ASM_BARRIER: keeps s3 live to the end (original save-reg order) */
  __asm__ volatile("" : : "r"(s3));
}
