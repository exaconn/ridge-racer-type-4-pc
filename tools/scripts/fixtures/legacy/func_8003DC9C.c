typedef struct { short f0, f2, f4, f6, f8, fA, fC, fE; } E;
extern int Rect16RowCount;
extern E *Rect16Rows;

void InitRect16Row(int i, short a, short b, short c, short d, short e, short f) {
    if (i < Rect16RowCount) {
        E *p = (E *)((i << 4) + (int)Rect16Rows);
        p->f0 = f;
        p->f2 = 0;
        p->f4 = a;
        p->f6 = b;
        p->f8 = c;
        p->fA = d;
        p->fC = e;
    }
}
