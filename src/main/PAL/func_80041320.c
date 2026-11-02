
typedef struct 
{
  char pad[0x1C];
  char c;
  char pad2[0x17];
  short d;
  unsigned short e;
} E;
extern E *D_800FFC14;
extern unsigned short D_8009FDA0[];
void func_80041320(a0, a1, a2)
int a0;
int a1;
int a2;
{
  E *p;
  ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->e = D_8009FDA0[a1];
  if (a2 < (-0xFF))
  {
    ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->d = -a2;
    return;
  }
  switch (a2)
  {
    case 0:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x20;
      return;

    case 1:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x3E;
      return;

    case -1:
      return;

  }

}
