
/* RODATA 0x80010740 0x1c */
typedef struct 
{
  char p0[0x1A0];
  unsigned short s1A0;
  char p1A2[5];
  unsigned char s1A7;
  char p1A8[0x1B8 - 0x1A8];
  short s1B8;
  char p1BA[0x1D4 - 0x1BA];
  short s1D4;
  char p1D6[2];
  short s1D8;
  char p1DA[0x1E4 - 0x1DA];
  unsigned short s1E4;
  short s1E6;
  short s1E8;
  char p1EA[2];
  short s1EC;
  char p1EE[2];
  unsigned char s1F0[4];
  char p1F4[0x224 - 0x1F4];
  short s224;
  char p226[0x24C - 0x226];
  int s24C;
  char p250[0x28C - 0x250];
  int s28C;
  char p290[4];
  int s294;
  char p298[0x2EC - 0x298];
  int s2EC;
} C;
extern void (*D_8009F7A0[])();
extern unsigned char D_8009F7E0[];
extern char D_800ABEF8[];
extern short D_800AC59C;
extern unsigned short D_800F3914;
extern C *D_800FFC20[];
extern int func_80022B64();
extern int func_80025F84();
extern int func_80029660();
extern int func_8002BF34();
extern int func_8002C1D4();
extern int func_8004A170();
extern int func_8004A474();
extern int func_800904F4();
void func_80037D88(arg0)
unsigned int arg0;
{
  int i;
  int j;
  int s4;
  int s0;
  int s5;
  int s6;
  int new_var;
  C *c;
  int *g;
  g = (int *) 0x1F80007C;
  s6 = 0;
  s5 = ((int) (D_800F3914 << 16)) >> 17;
  switch (arg0)
  {
    case 0:
    default:
      func_80029660(D_800ABEF8, 0);
      func_80022B64(D_800ABEF8, 0);
      break;

    case 1:
    case 6:
      func_80029660(D_800ABEF8, 0);
      func_80022B64(D_800ABEF8, 0);
      s6 = 1;
      func_8004A170();
      break;

    case 2:
      func_80029660(D_800ABEF8, 0);
      func_80029660(D_800ABEF8 + 0x320, 1);
      func_80022B64(D_800ABEF8, 0);
      func_80022B64(D_800ABEF8 + 0x320, 1);
      break;

    case 3:
      func_80029660(D_800ABEF8, 0);
      func_80029660(D_800ABEF8 + 0x320, 1);
      func_80022B64(D_800ABEF8, 0);
      func_80022B64(D_800ABEF8 + 0x320, 1);
      s6 = 1;
      func_8004A170();
      break;

    case 4:
      for (i = 0; i < D_800AC59C; i++)
      {
        func_80029660(D_800FFC20[i], i);
      }
      for (i = 0; i < D_800AC59C; i++)
      {
        if (D_800FFC20[i]->s1E8 != (-1))
        {
          func_80022B64(D_800FFC20[i], i);
        }
      }
      break;

    case 5:
      s6 = 1;
      func_8004A170();
      break;
  }

  for (i = 0; i < 7; i++)
  {
    for (*g = 0; *g < D_800AC59C; (*g)++)
    {
      if (D_800FFC20[*g]->s1E8 != (-1))
      {
        D_8009F7A0[i](D_800FFC20[*g]);
      }
    }
  }
  for (i = 0; i < D_800AC59C; i++)
  {
    if (D_800FFC20[i]->s1E8 != (-1))
    {
      if (D_800FFC20[i]->s1A0 & 7)
      {
        func_8002C1D4(2, D_800FFC20[i]);
        func_8002BF34(D_800FFC20[i], (short) D_800FFC20[i]->s1A0);
      }
      if (((short) D_800FFC20[i]->s1A0) != 0)
      {
        s4 = ((func_80025F84(0x800 - D_800FFC20[i]->s1EC, D_800FFC20[i]->s1D4) - 0x400) * 3) / 5;
        c = D_800FFC20[i];
        if (c->s1D8 >= 0xA1)
        {
          if (c->s1E4 & 0x10)
          {
            if (c->s1D8 >= 0x2F9)
            {
              c->s224 = 0x80;
              c = D_800FFC20[i];
            }
            s0 = (c->s1D8 * 0x3A) / (c->s28C + 1);
            c->s24C -= 0x1388;
            new_var = func_800904F4(s4, c);
            c = D_800FFC20[i];
            c->s294 = (c->s294 * (0x64 - ((s0 * new_var) >> 12))) / 100;
          }
          else
          {
            s0 = (c->s1D8 * 0x1E) / ((c->s2EC >> 8) + 1);
          }
          new_var = func_800904F4(s4, c);
          c = D_800FFC20[i];
          c->s1D8 = (c->s1D8 * (0x64 - ((s0 * new_var) / 4096))) / 100;
        }
      }
      if (D_800FFC20[i]->s1D8 >= 0x321)
      {
        if (D_800FFC20[i]->s1B8 == 0)
        {
          for (j = 0; j < 4; j++)
          {
            if (D_800FFC20[i]->s1F0[j] != 0)
            {
              D_800FFC20[i]->s1D8 -= D_8009F7E0[s5];
              if (D_800FFC20[i]->s1F0[j] == 2)
              {
                D_800FFC20[i]->s1D8 += D_8009F7E0[s5];
              }
              if (s5 == 2 || D_800FFC20[i]->s1A7 == 0)
              {
                D_800FFC20[i]->s1A7 = 0xA;
              }
            }
          }
        }
      }
    }
  }
  for (i = 7; i < 0xB; i++)
  {
    for (*g = 0; *g < D_800AC59C; (*g)++)
    {
      if (D_800FFC20[*g]->s1E8 != (-1))
      {
        D_8009F7A0[i](D_800FFC20[*g]);
      }
    }
  }
  if (s6 == 1)
  {
    func_8004A474();
  }
}
