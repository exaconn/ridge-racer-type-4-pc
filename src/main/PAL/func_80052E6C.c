
extern struct 
{
  char pad[0x26C6];
  short h26C6;
  unsigned short h26C8;
  unsigned short h26CA;
  unsigned short h26CC;
} D_800F4DB8;
extern int func_80052ADC();
extern int func_80052AB8();
void func_80052E6C()
{
  int d = D_800F4DB8.h26C6;
  int t;
  int v;
  int from;
  unsigned c;
  int to;
  if (d > 0)
  {
    c = D_800F4DB8.h26C8;
    if (((short) D_800F4DB8.h26C8) < d)
    {
      t = c + 1;
      to = D_800F4DB8.h26CA;
      from = D_800F4DB8.h26CC;
      v = (to - from) << 12;
      v = (v / d) * ((short) t);
      D_800F4DB8.h26C8 = t;
      if (v < 0)
      {
        v += 0xFFF;
      }
      v = (v >> 12) + from;
      func_80052ADC(v & 0xFFFF, &D_800F4DB8, from);
      return;
    }
    D_800F4DB8.h26C6 = 0;
    func_80052AB8(d, &D_800F4DB8);
  }
}
