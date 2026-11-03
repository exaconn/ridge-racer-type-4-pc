
extern int D_800F4C1C;
extern char *D_800F4D8C;
extern char D_8009F9D4[];
void func_8003E040(a0, a1, a2, a3)
int a0;
int a1;
int a2;
int a3;
{
  char *s;
  char *new_var;
  char *q;
  if (a0 < D_800F4C1C)
  {
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 2)) = a1;
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 7)) = a2;
    s = D_8009F9D4 + (a3 * 0x14);
    new_var = (char *) ((a0 * 0x1C) + ((int) D_800F4D8C));
    q = new_var;
    *((short *) (q + 8)) = *((short *) (s + 0));
    *((short *) (q + 0xA)) = *((short *) (s + 2));
    *((short *) (q + 0xC)) = *((short *) (s + 4));
    *((short *) (q + 0xE)) = *((short *) (s + 6));
    q[1] = s[0xE];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 5)) = s[0xF];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 6)) = s[0x10];
    q = (char *) ((a0 * 0x1C) + ((int) D_800F4D8C));
    *((short *) (q + 0x10)) = *((short *) (s + 8));
    *((short *) (q + 0x12)) = *((short *) (s + 0xA));
    q[0x14] = s[0xC];
    *((char *) (((a0 * 0x1C) + ((int) D_800F4D8C)) + 0x15)) = s[0xD];
  }
}
