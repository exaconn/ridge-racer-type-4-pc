
extern void func_80095A44();
extern void func_80095AF4();
extern void func_80095B24();
extern void func_80095B94();
extern unsigned char D_8009ED38[];
extern unsigned char D_8009EDF8[];
char *func_8001F5B4(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
int arg0;
char *arg1;
int arg2;
int arg3;
unsigned char *arg4;
int arg5;
int arg6;
int arg7;
{
  int x;
  int y;
  int c;
  int h;
  int new_var;
  char *p;
  char *q;
  h = arg7 >> 1;
  if ((*arg4) != 0)
  {
    p = arg1;
    do
    {
      c = (*arg4) - 0x20;
      arg4++;
      if (c != 0)
      {
        x = D_8009ED38[c * 2] << 3;
        y = D_8009ED38[(c * 2) + 1] << 4;
        func_80095B94(arg1);
        if (h == 0)
        {
          func_80095B24(arg1, 1);
        }
        else
        {
          p[4] = h;
          p[5] = h;
          p[6] = h;
        }
        func_80095AF4(arg1, arg7 & 1);
        *((short *) (p + 8)) = arg2;
        *((unsigned short *) (p + 0xA)) = arg3;
        *((short *) (p + 0x10)) = 8;
        p[0xC] = x;
        p[0xD] = y;
        *((short *) (p + 0x12)) = 0x10;
        *((unsigned short *) (p + 0xE)) = arg5;
        q = p;
        p += 0x14;
        arg1 += 0x14;
        func_80095A44(arg0, q);
      }
      if (arg6 == 1)
      {
        arg2 = (arg2 + (new_var = 2)) + D_8009EDF8[((x / 8) + ((y / 16) * 32)) % 54];
      }
      else
        if (arg6 == 2)
      {
        arg2 = arg2 + D_8009EDF8[((x / 8) + ((y / 16) * 32)) % 54];
      }
      else
      {
        arg2 += 6;
      }
    }
    while (*arg4 != 0);
  }
  return arg1;
}
