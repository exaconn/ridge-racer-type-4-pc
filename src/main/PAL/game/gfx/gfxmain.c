
extern void func_800207D4();
extern void func_800439BC();
extern void func_800451E4();
extern void func_800452CC();
extern void func_8004FED4();
extern void func_80050028();
extern void func_80051538();
extern void func_800528A8();
extern void func_80052A30();
extern void func_80052B8C();
extern void func_80059E40();
extern void func_8005A90C();
extern int func_80096464();
extern int D_800A0C9C[];
extern int D_800AC650[];
extern char D_800AC6B0[];
extern int D_800AC6D8;
extern int D_800AC6EC;
extern int D_800ACBF8;
extern char *D_800ACC14;
extern signed char D_800AD4D8[];
extern short D_800AD520;
extern signed char D_800AD528[];
extern unsigned short D_800F3A40;
extern short D_800F4D44;
extern int D_800F765C;
extern int D_8010718C;
int func_80045868(char *arg0)
{
  int i;
  int s0;
  int *p;
  signed char *q;
  char *r;
  char *pk;
  char *pk2;
  char *ot;
  char *ot2;
  int v;
  int v1;
  int v2;
  int c;
  int *g;
  int *g2;
  int *g1;
  int *gB;
  int *gC;
  int *gL;
  int *gF;
  int *gD;
  int *gS;
  int *gT;
  s0 = 0;
  do
  {
    func_80050028(s0, 1);
    s0++;
  }
  while (s0 < 4);
  g1 = D_800AC650;
  if ((*((int *) (((char *) g1) + 0x90))) != 0)
  {
    func_800207D4(D_800ACC14 + 0x70, (*((int *) (((char *) g1) + 0x8C))) * 10, 0x25);
    v1 = (*((int *) (((char *) g1) + 0xA0))) - 8;
    *((int *) (((char *) g1) + 0xA0)) = v1;
    if (v1 < 0)
    {
      *((int *) (((char *) g1) + 0xA0)) = 0;
    }
    func_80051538(0, (short) (*((int *) (((char *) g1) + 0xA0))), (short) (*((int *) (((char *) g1) + 0xA0))));
    *((int *) (((char *) g1) + 0x8C)) = (*((int *) (((char *) g1) + 0x8C))) + 1;
  }
  else
  {
    v2 = (*((int *) (((char *) g1) + 0xA0))) + 8;
    *((int *) (((char *) g1) + 0xA0)) = v2;
    if (v2 >= 0x61)
    {
      *((int *) (((char *) g1) + 0xA0)) = 0x60;
    }
    func_80051538(0, (short) (*((int *) (((char *) g1) + 0xA0))), (short) (*((int *) (((char *) g1) + 0xA0))));
  }
  gB = D_800AC650;
  s0 = gB[0x88 / 4];
  if (s0 < 0xC)
  {
    if (s0 == 1)
    {
      func_8005A90C();
      if (gB[0x94 / 4] == s0)
      {
        func_80052A30(0);
      }
      {
        int two;
        s0 = 0;
        two = 2;
        q = D_800AD528;
        p = gB;
        do
        {
          s0++;
          p[0xA8 / 4] = q[0x2A];
          __asm__ volatile("" : "+r"(q) : "r"(p)); /* ASM_BARRIER: prevent CSE of q + 0x2A and prioritize p for a0 */
          q[0x2A] = two;
          q += 4;
          p++;
        }
        while (s0 < 2);
      }
    }
    {
      int x, d;
      x = D_800AC6D8 << 8;
      d = x / 12;
      s0 = 0x100;
      __asm__("" : "+r"(s0) : "r"(x)); /* ASM_BARRIER: keep 0x100 in s0 and prioritize x in v0 */
      s0 -= d;
    }
    func_80059E40(0x1C2);
    {
      int w, low, high;
      low = 0xFFFFFF;
      high = 0xFF000000;
      pk = *((char **) 0x1F800000);
      pk[3] = 3;
      pk[7] = 0x62;
      pk[4] = s0;
      pk[5] = s0;
      pk[6] = s0;
      ot = D_800ACC14;
      __asm__("" : "+r"(pk) : "r"(ot), "r"(low)); /* ASM_BARRIER: order ot load before pk load and prioritize low */
      w = *((int *) pk);
      *((short *) (pk + 0xC)) = 0x140;
      *((short *) (pk + 8)) = 0;
      *((short *) (pk + 0xA)) = 0;
      *((short *) (pk + 0xE)) = 0xF0;
      *((int *) pk) = (w & high) | ((*((int *) (ot + 0x70))) & low);
      *((int *) (ot + 0x70)) = ((*((int *) (ot + 0x70))) & high) | (((int) pk) & low);
      pk += 0x10;
      *((char **) 0x1F800000) = pk;
      pk[3] = 1;
      ot2 = D_800ACC14;
      __asm__("" : "+r"(pk) : "r"(ot2)); /* ASM_BARRIER: order ot2 load before pk load */
      w = *((int *) pk);
      *((int *) (pk + 4)) = 0xE1000245;
      *((int *) pk) = (w & high) | ((*((int *) (ot2 + 0x70))) & low);
      *((int *) (ot2 + 0x70)) = ((*((int *) (ot2 + 0x70))) & high) | (((int) pk) & low);
      *((char **) 0x1F800000) = pk + 8;
    }
  }
  gC = D_800AC650;
  if (gC[0x94 / 4] == 0)
  {
    if ((D_800F4D44 == 1) && (D_800AD4D8[D_800AD4D8[0x17] + 8] >= 3))
    {
      s0 = 1;
      gL = gC;
      gL[0x98 / 4] = 0;
      r = (((char *) gL[0x554 / 4]) + (D_800AD520 * 0x280)) + 0x28;
      do
      {
        v = *((int *) (r + 0x24));
        if (gL[0x98 / 4] < v)
        {
          gL[0x98 / 4] = v;
        }
        s0++;
        r += 0x28;
      }
      while (s0 < 0x10);
      gF = D_800AC650;
      gF[0x94 / 4] = 1;
      gF[0x98 / 4] = gF[0x98 / 4] * 5;
    }
    else
    {
      gC[0x94 / 4] = 2;
    }
  }
  gD = D_800AC650;
  switch (gD[0x94 / 4])
  {
    case 1:
      func_800452CC(arg0, gD[0x88 / 4], D_800AD520);
      if (D_800F3A40 & 0x800)
    {
      if (gD[0x88 / 4] >= 0x1A)
      {
        if (gD[0x90 / 4] == 0)
        {
          gD[0x98 / 4] = gD[0x88 / 4] + 0x19;
        }
      }
    }
      gS = D_800AC650;
      if ((gS[0x98 / 4] - gS[0x88 / 4]) < 0x1A)
    {
      if (gS[0x90 / 4] == 0)
      {
        gS[0x90 / 4] = 1;
        func_80052B8C(0, 8);
      }
    }
      gT = D_800AC650;
      gT[0x88 / 4]++;
      if (gT[0x88 / 4] >= gT[0x98 / 4])
    {
      gT[0x88 / 4] = gT[0x98 / 4];
      gT[0x9C / 4] = 1;
    }
      D_8010718C = 0;
      break;

    case 2:
      gD[0x88 / 4]++;
      if (gD[0x88 / 4] >= 0x49)
    {
      gD[0x9C / 4] = 1;
      *((unsigned short *) (arg0 + 0x1E4)) = (*((unsigned short *) (arg0 + 0x1E4))) | 0x100;
    }
      if (gD[0x9C / 4] == 0)
    {
      *((short *) (((char *) D_800AC650) + 0x30)) = 0;
      *((short *) (((char *) D_800AC650) + 0x32)) = 0;
      *((short *) (((char *) D_800AC650) + 0x34)) = -0x100;
      func_800439BC(arg0, ((char *) D_800AC650) + 0x30, ((char *) D_800AC650) + 0x60);
      *((short *) (((char *) D_800AC650) + 0x74)) = 0;
      *((int *) (((char *) D_800AC650) + 0x80)) = 0x1D6;
      *((short *) (((char *) D_800AC650) + 0x70)) = (((*((int *) (((char *) D_800AC650) + 0x88))) * 0x190) / 73) - 0x280;
      v = (*((short *) (((char *) D_800AC650) + 0x72)) = ((-((*((int *) (((char *) D_800AC650) + 0x88))) * 0xB00)) / 73) + 0x600);
      *((int *) (((char *) D_800AC650) + 0x7C)) = ((-((*((int *) (((char *) D_800AC650) + 0x88))) << 9)) / 73) + 0x400;
      if (((short) v) < (-0x400))
      {
        *((short *) (((char *) D_800AC650) + 0x72)) = -0x400;
      }
    }
    else
    {
      *((short *) (((char *) D_800AC650) + 0x30)) = 0;
      *((short *) (((char *) D_800AC650) + 0x32)) = 0;
      *((short *) (((char *) D_800AC650) + 0x34)) = -0x100;
      func_800439BC(arg0, ((char *) D_800AC650) + 0x30, ((char *) D_800AC650) + 0x60);
      *((short *) (((char *) D_800AC650) + 0x70)) = -0xF0;
      *((short *) (((char *) D_800AC650) + 0x72)) = -0x400;
      *((int *) (((char *) D_800AC650) + 0x7C)) = 0x200;
      *((short *) (((char *) D_800AC650) + 0x74)) = 0;
      *((int *) (((char *) D_800AC650) + 0x80)) = 0x1D6;
    }
      func_800451E4(D_800AC6B0);
      g2 = (int *) (D_800AC6B0 - 0x60);
      if (g2[0x88 / 4] >= 0x33)
    {
      g2[0x90 / 4] = 1;
    }
      break;

  }

  g = D_800AC650;
  if (g[0x9C / 4] != 0)
  {
    func_8004FED4();
    v = g[0x9C / 4];
    D_800ACBF8 = 1;
    if (v == 1)
    {
      if (g[0x94 / 4] == v)
      {
        if (D_800F765C == 0)
        {
          func_800528A8(0, (func_80096464() % 14) + 0xB);
        }
        else
        {
          func_800528A8(0, D_800A0C9C[D_800F765C - 1]);
        }
      }
      D_800AC6EC = 2;
    }
    s0 = 0;
    p = D_800AC650;
    q = D_800AD528;
    do
    {
      q[0x2A] = *(((unsigned char *) p) + 0xA8);
      p++;
      q += 4;
      s0++;
    }
    while (s0 < 2);
  }
  else
  {
    D_800ACBF8 = 0;
  }
  return D_800AC6EC;
}
