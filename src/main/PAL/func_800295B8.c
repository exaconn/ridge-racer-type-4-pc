typedef struct { int w[200]; } B800;
extern int D_800AD504;
extern int D_800ADAC8;
extern int D_800FF6B0;
extern short D_800AC59C;
extern B800 *D_800FFC20[];
extern B800 *D_800AC570[];
/* Callers explicitly supply this legacy argument; the body ignores it. */
void func_800295B8(int unused)
{
  int i;
  if (D_800AD504 == 0)
  {
    D_800ADAC8 = D_800FF6B0;
    for (i = 0; i < D_800AC59C; i++)
      *D_800AC570[i] = *D_800FFC20[i];
    D_800AD504 = 1;
  }
}
