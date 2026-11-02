
typedef struct 
{
  short a;
  short b;
  short c;
  short d;
  short e;
  short f;
  short g;
  short h;
  short i;
  char pad[10];
} E;
extern int D_800F2ECC;
extern E *D_800F3AFC;
void func_8003D228(a0, a1, a2, a3, a4, a5, a6)
int a0;
int a1;
short a2;
int a3;
unsigned short a4;
unsigned short a5;
unsigned short a6;
{
  int s;
  unsigned int z;
  if (a0 < D_800F2ECC)
  {
    D_800F3AFC[a0].a = a6;
    D_800F3AFC[a0].b = 0;
    s = (short) a6;
    if (s < 0)
    {
      a6 = 0;
    }
    else
    {
      z = s == 0;
      a6 = 0x16;
      if (z)
      {
        a4 = 0x80;
      }
    }
    D_800F3AFC[a0].d = a2 - a6;
    D_800F3AFC[a0].f = a2 - a6;
    D_800F3AFC[a0].c = a1;
    D_800F3AFC[a0].e = a1;
    D_800F3AFC[a0].g = a3;
    D_800F3AFC[a0].h = a4 + a6;
    D_800F3AFC[a0].i = a5;
  }
}
