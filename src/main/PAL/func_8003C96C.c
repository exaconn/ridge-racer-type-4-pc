typedef struct { int key; short idx; } W10_Q;
typedef struct { char p[0x184]; int a; int b; char q[0x62]; short n; } W10_O;
extern short D_800AC59C;
extern W10_Q *D_800F2DE8[];
extern int D_800FF6B0;
extern W10_O *D_800FFC20[];
void func_8003C96C(void)
{
  int i;
  int j;
  if (D_800FF6B0 < 4)
  {
    for (i = 0; i < D_800AC59C; i++)
    {
      W10_Q *q = D_800F2DE8[i];
      W10_O **o = &D_800FFC20[q->idx];
      q->key = (*o)->a + (*o)->b;
      (*o)->n = i;
    }
    for (i = 0; i < D_800AC59C - 1; i++)
    {
      for (j = D_800AC59C - 1; i < j; j--)
      {
        W10_Q *a2 = D_800F2DE8[j - 1];
        W10_Q *a1 = D_800F2DE8[j];
        int a0 = a2->key;
        int v1 = a1->key;
        if (a0 < v1 || (a0 == v1 && a1->idx == 0))
        {
          D_800F2DE8[j] = a2;
          D_800F2DE8[j - 1] = a1;
        }
      }
    }
  }
}
