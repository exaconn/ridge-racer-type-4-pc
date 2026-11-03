#include "rr/gpu.h"

extern s32 func_80093C24();
extern s32 func_80095A44();
extern s32 func_80095AF4();
extern s32 func_80095B24();
extern s32 func_80095B94();
extern u8 D_8009ED38[];
extern char *D_800ACC14;
extern unsigned char D_8009EDF8[];

void func_8001F40C();
char *func_8001F5B4();

void func_8001F40C(a0, a1, a2)
s32 a0, a1;
u8 *a2;
{
    RrSpritePacket *s3, *s2;
    s32 c, a, v, row, col, u, vv;
    s3 = *(RrSpritePacket **)0x1F800000;
    s2 = s3;
    for (; *a2 != 0; a0 += 10) {
            a = *a2 - 0x20;
            a2++;
            if (a != 0) {
                a = D_8009ED38[a * 2] + (D_8009ED38[a * 2 + 1] << 5);
                row = a / 21;
                col = a % 21;
                u = col * 12;
                vv = row * 24 + 0x24;
                func_80095B94(s3);
                s2->w = 12;
                s2->h = 24;
                func_80095B24(s3, 1);
                func_80095AF4(s3, 1);
                s2->x = a0;
                s2->y = a1;
                s2->u = u;
                s2->v = vv;
                s2->clut = 0x5B20;
                func_80095A44(D_800ACC14 + 0x70, s2++);
                s3++;
            }
    }
    func_80093C24(s3, 0, 1, 0x58, 0);
    func_80095A44(D_800ACC14 + 0x70, s3);
    *(char **)0x1F800000 = (char *)s3 + 0xC;
}

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
