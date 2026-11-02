
typedef unsigned char u8;
typedef unsigned short u16;
extern int func_80025FBC();
extern int func_8004BAD4();
extern int func_800904F4();
extern int func_80096464();
extern char D_800ABEF8[];
extern int D_800AD568;
extern char D_800ADAD8[];
extern char *D_800F39C8[];
extern char D_800F3A38[];
extern int D_800FF6B0;
void func_80022D50(arg0, arg1)
char *arg0;
int arg1;
{
  /* ASM_BARRIER: pin s1 to $17 */
  register char *s1 asm("$17");
  char *a2;
  char *q;
  int v6;
  int v7;
  short h;
  int sw;
  int p0;
  int p1;
  int d0;
  int d1;
  int d2;
  int r0;
  int r1;
  int r2;
  int a1;
  int v;
  int s2;
  int s0;
  int a0;
  int c;
  int d;
  int t;
  int t2;
  int v0_3;
  s2 = 0x1800;
  s1 = arg0 + 0x1F4;
  if (D_800FF6B0 < 2)
  {
    *((int *) (((char *) s1) + 0xC)) = 0;
    *((int *) (((char *) arg0) + 0x178)) = 0;
  }
  else
    if (D_800FF6B0 < 4)
  {
    q = D_800ADAD8;
    a2 = (arg1 * 4) + q;
    a1 = *((u8 *) (((char *) a2) + 3));
    sw = a1 & 0xE0;
    if (sw == 0x20)
    {
      s2 = ((*((short *) (((char *) (*((char **) (((char *) s1) + 0x1C)))) + 8))) * 0x1400) / 1000;
      if (D_800AD568 != 0)
      {
        p1 = a1 & 1;
        p0 = a1 & 2;
      }
      else
      {
        p0 = a1 & 1;
        p1 = a1 & 2;
      }
      if (p0 != 0)
      {
        t2 = *((int *) (((char *) s1) + 0xC));
        *((short *) (((char *) s1) + 0x44)) = 1;
        if (t2 <= 0)
        {
          if ((-s2) < t2)
          {
            *((int *) (((char *) s1) + 0xC)) = t2 - 0x500;
          }
        }
        else
        {
          *((int *) (((char *) s1) + 0xC)) = 0;
        }
      }
      else
        if (p1 != 0)
      {
        t2 = *((int *) (((char *) s1) + 0xC));
        *((short *) (((char *) s1) + 0x44)) = -1;
        if (t2 < 0)
        {
          *((int *) (((char *) s1) + 0xC)) = 0;
        }
        else
          if (t2 < s2)
        {
          *((int *) (((char *) s1) + 0xC)) = t2 + 0x500;
        }
      }
      else
      {
        *((short *) (((char *) s1) + 0x44)) = 0;
        *((int *) (((char *) s1) + 0xC)) = (*((int *) (((char *) s1) + 0xC))) / 2;
      }
      *((int *) (((char *) arg0) + 0x178)) = -(*((int *) (((char *) s1) + 0xC)));
    }
    else
      if (sw == 0x40)
    {
      s2 = ((*((short *) (((char *) (*((char **) (((char *) s1) + 0x1C)))) + 8))) * 0x1800) / 1000;
      a0 = (s2 * (*((signed char *) (((char *) a2) + 0)))) / 127;
      d0 = D_800AD568;
      *((int *) (((char *) s1) + 0xC)) = a0;
      if (d0 != 0)
      {
        *((int *) (((char *) s1) + 0xC)) = -a0;
      }
      r0 = *((int *) (((char *) s1) + 0xC));
      if (r0 < 0)
      {
        *((short *) (((char *) s1) + 0x44)) = 1;
      }
      else
        if (r0 > 0)
      {
        *((short *) (((char *) s1) + 0x44)) = -1;
      }
      else
      {
        *((short *) (((char *) s1) + 0x44)) = 0;
      }
      *((int *) (((char *) arg0) + 0x178)) = -(*((int *) (((char *) s1) + 0xC)));
    }
    else
      if (sw == 0x60)
    {
      s2 = ((*((short *) (((char *) (*((char **) (((char *) s1) + 0x1C)))) + 8))) * 0x1800) / 1000;
      a0 = (s2 * (*((signed char *) (((char *) a2) + 0)))) / 127;
      d1 = D_800AD568;
      *((int *) (((char *) s1) + 0xC)) = a0;
      if (d1 != 0)
      {
        *((int *) (((char *) s1) + 0xC)) = -a0;
      }
      r1 = *((int *) (((char *) s1) + 0xC));
      if (r1 < 0)
      {
        *((short *) (((char *) s1) + 0x44)) = 1;
      }
      else
        if (r1 > 0)
      {
        *((short *) (((char *) s1) + 0x44)) = -1;
      }
      else
      {
        *((short *) (((char *) s1) + 0x44)) = 0;
      }
      *((int *) (((char *) arg0) + 0x178)) = -(*((int *) (((char *) s1) + 0xC)));
    }
    else
      if (sw == 0x80)
    {
      s2 = ((*((short *) (((char *) (*((char **) (((char *) s1) + 0x1C)))) + 8))) * 0x1400) / 1000;
      a0 = (s2 * (*((signed char *) (((char *) a2) + 0)))) / 127;
      d2 = D_800AD568;
      *((int *) (((char *) s1) + 0xC)) = a0;
      if (d2 != 0)
      {
        *((int *) (((char *) s1) + 0xC)) = -a0;
      }
      r2 = *((int *) (((char *) s1) + 0xC));
      if (r2 < 0)
      {
        *((short *) (((char *) s1) + 0x44)) = 1;
      }
      else
        if (r2 > 0)
      {
        *((short *) (((char *) s1) + 0x44)) = -1;
      }
      else
      {
        *((short *) (((char *) s1) + 0x44)) = 0;
      }
      *((int *) (((char *) arg0) + 0x178)) = -(*((int *) (((char *) s1) + 0xC)));
    }
    else
    {
      *((int *) (((char *) s1) + 0xC)) = 0;
    }
  }
  else
  {
    int s0_else;
    /* ASM_BARRIER: pin t5 to a1 for first call argument */
    register int t5 asm("$5");
    t5 = (*((short *) (((char *) arg0) + 0x1EA))) << 0xB;
    t5 += 0x800;
    s0_else = func_80025FBC(*((int *) (((char *) arg0) + 0x54)), t5 - (*((short *) (((char *) arg0) + 0x1EC)))) << 8;
    a0 = 0x1000 - func_800904F4(((*((int *) (((char *) arg0) + 0xF4))) << 8) / 96);
    if ((*((short *) (((char *) arg0) + 0x1D8))) < 0x320)
    {
      a0 = a0 * 8;
    }
    else
    {
      a0 = a0 * 6;
    }
    if ((*((short *) (((char *) arg0) + 0x1D8))) >= 0x51)
    {
      if ((*((short *) (((char *) arg0) + 0x1EA))) != 0)
      {
        if ((*((int *) (((char *) arg0) + 0xF4))) < 0)
        {
          a0 = -a0;
        }
      }
      else
        if ((*((int *) (((char *) arg0) + 0xF4))) > 0)
      {
        a0 = -a0;
      }
      a0 = a0 + s0_else;
    }
    else
    {
      *((int *) (((char *) s1) + 0xC)) = 0;
      a0 = 0;
    }
    if (a0 < (-0x1400))
    {
      a0 = -0x1400;
    }
    if (!(a0 < 0x1401))
    {
      a0 = 0x1400;
    }
    *((int *) (((char *) s1) + 0xC)) = a0;
    *((int *) (((char *) arg0) + 0x178)) = -a0;
  }
  if ((*((short *) (((char *) arg0) + 0x1B8))) == 0)
  {
    t = *((short *) (((char *) s1) + 0x28));
    if ((t > 0) && (t < (*((short *) (((char *) s1) + 0x2A)))))
    {
      a0 = 0xC8;
    }
    else
    {
      if (((*((short *) (((char *) arg0) + 0x19E))) == 1) && ((*((int *) (((char *) s1) + 0xA4))) == 0))
      {
        /* ASM_BARRIER: pin h_sh to v0 for sll 16 */
        register int h_sh asm("$2");
        h_sh = (((short) (*((u16 *) (((char *) s1) + 0x36)))) / 3) << 16;
        a0 = (h_sh >> 16) / 30;
      }
      else
      {
        /* ASM_BARRIER: block hoist of a0 = 0 into bne delay slot */
        __asm__ volatile("");
        a0 = 0;
      }
    }
    a1 = *((short *) (((char *) arg0) + 0x1D8));
    if ((a1 < 0x100) && ((*((int *) (((char *) s1) + 0xA4))) == 0))
    {
      d = ((((*((int *) (((char *) s1) + 0xC))) * ((*((short *) (((char *) s1) + 0x36))) + a0)) / 256) * a1) / 0x10000;
      *((int *) (((char *) s1) + 0x9C)) = (*((int *) (((char *) s1) + 0x9C))) + d;
    }
    else
      if ((a1 < 0x200) && ((*((int *) (((char *) s1) + 0xA4))) == 4))
    {
      d = ((((*((int *) (((char *) s1) + 0xC))) * (*((short *) (((char *) s1) + 0x36)))) / 256) * a1) / 0x20000;
      *((int *) (((char *) s1) + 0x9C)) = (*((int *) (((char *) s1) + 0x9C))) + d;
    }
    else
    {
      d = ((*((int *) (((char *) s1) + 0xC))) * ((*((short *) (((char *) s1) + 0x36))) + a0)) / 0x10000;
      *((int *) (((char *) s1) + 0x9C)) = (*((int *) (((char *) s1) + 0x9C))) + d;
    }
  }
  *((int *) (((char *) s1) + 0x9C)) = (*((int *) (((char *) s1) + 0x9C))) & 0xFFF;
  if ((arg0 == D_800ABEF8) || (arg0 == (D_800ABEF8 + 0x320)))
  {
    /* ASM_BARRIER: pin p4 to a0 for D_800F3A38 base */
    register char *p4 asm("$4");
    p4 = D_800F3A38;
    *((short *) ((p4 + ((arg1 & 1) * 0x5C)) + 0x50)) = 1;
    s0 = *((short *) (((char *) arg0) + 0x1D8));
    t = (*((short *) (((char *) D_800F39C8[arg1]) + 0x1F0))) * 8;
    if (t < s0)
    {
      s0 = t;
    }
    a0 = *((int *) (((char *) s1) + 0xC));
    if ((a0 >> 3) != 0)
    {
      s2 = (((s0 * 0x14) / t) * a0) / s2;
    }
    else
    {
      s2 = 0;
    }
    c = *((int *) (((char *) s1) + 0xA4));
    if (c == 0)
      goto L_not2;
    if (c != 2)
    {
    L_not2:
      if ((*((short *) (((char *) (*((char **) (((char *) s1) + 0x1C)))) + 0xA))) > 0)
      {
        v6 = *((short *) (((char *) arg0) + 0x1DE));
        if (v6 > 0)
        {
          s2 += ((int) (((-v6) >> 3) + (((unsigned) (-v6)) >> 31))) >> 1;
        }
      }
    }
    else
    {
      s0 = func_80025FBC(*((short *) (((char *) arg0) + 0x1D4)), *((int *) (((char *) arg0) + 0x54)));
      if (s0 >= 0x201)
      {
        s0 = 0x200;
      }
      else
        if (s0 < (-0x200))
      {
        s0 = -0x200;
      }
      s2 = (s2 * func_800904F4(s0 * 2)) / 0x1000;
    }
    s0 = 2;
    if (s2 <= 0)
    {
      s0 = 1;
      s2 = -s2;
    }
    if ((*((short *) (((char *) arg0) + 0x1B8))) != 0)
    {
      s2 = 0;
    }
    if (!(s2 < 0xC))
    {
      s2 -= func_80096464() & 3;
    }
    if (!(s2 < 0x10))
    {
      s2 = 0xF;
    }
    if ((*((u8 *) (((char *) arg0) + 0x1A5))) >= 0x10U)
    {
      if ((*((u8 *) (((char *) arg0) + 0x1A4))) != 1)
      {
        /* ASM_BARRIER: pin v7_reg to v0 for lh 0x1A2 */
        register int v7_reg asm("$2");
        v7_reg = *((short *) (((char *) arg0) + 0x1A2));
        if (v7_reg < 0)
        {
          v = v7_reg;
          /* ASM_BARRIER: force copy to a0 in delay slot before negu a0,a0 */
          __asm__("" : "+r"(v));
          v = -v;
        }
        else
        {
          v = v7_reg;
        }
        if (v >= 0x96)
        {
          s2 = 0xF;
        }
        else
        {
          s2 = v / 10;
        }
        s0 = 1;
        if (((*((u8 *) (((char *) arg0) + 0x1A5))) & 7) >= 4U)
        {
          s0 = 2;
        }
      }
    }
    else
      if ((*((u8 *) (((char *) arg0) + 0x1A7))) != 0)
    {
      s2 = 0xD;
      s0 = 1;
      if ((*((short *) (((char *) arg0) + 0x1B6))) > 0)
      {
        s0 = 2;
      }
    }
    if ((*((short *) (((char *) arg0) + 0x1D8))) == 0)
    {
      s0 = 0;
      s2 = 0xF;
    }
    if (D_800FF6B0 < 4)
    {
      func_8004BAD4(arg1 & 1, s0, s2);
    }
  }
}
