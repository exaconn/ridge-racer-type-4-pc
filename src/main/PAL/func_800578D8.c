
extern int func_8005642C();
extern int func_80057750();
extern int func_8002E828();
extern int func_80055C84();
struct Q
{
  char pad0[0xA0];
  int fA0;
  char pad1[0x100 - 0xA4];
  int f100;
  int f104;
  int f108;
};
extern struct 
{
  char pad[0x24C];
  struct Q *p;
} D_800AC650;
extern struct 
{
  char pad[0x10E];
  unsigned short f10E;
} D_800ABEF8;
extern struct 
{
  char pad[8];
  short f8;
} *D_800F3A08;
extern unsigned short D_800F3A40;
int func_800578D8(int a0)
{
  int sp10[2];
  int s1;
  s1 = 4;
  D_800AC650.p->f100 = (D_800AC650.p->fA0 - s1) - (D_800F3A08->f8 + ((short) (((short) D_800ABEF8.f10E) / 3)));
  s1 = func_8005642C(&D_800ABEF8, D_800AC650.p->f104, a0, sp10);
  func_80057750();
  func_8002E828(2, 0x1F800028);
  func_80055C84();
  D_800AC650.p->f104 = D_800AC650.p->f104 + 1;
  D_800AC650.p->f108 = D_800AC650.p->f108 + 1;
  if (D_800AC650.p->f104 >= 0x322)
  {
    D_800AC650.p->f104 = 0x321;
  }
  if (D_800AC650.p->f108 >= 0x516)
  {
    D_800AC650.p->f108 = 0x515;
  }
  if (D_800F3A40 & 0x800)
  {
    if (D_800AC650.p->f104 >= 0x33)
    {
      s1 = 1;
    }
  }
  return s1;
}
