typedef struct { char f0, f1, f2, f3; unsigned short s[6]; unsigned char b0, b1; char pad[2]; } E;
typedef struct { unsigned short s[6]; unsigned char b0, b1, pad[2]; } Q;
extern Q D_8009F9FC[];
extern int D_800ACC24;
extern E *D_800F2AB0;

void func_8003FAE4(int i, char a, char b, int j) {
    if (i < D_800ACC24) {
        Q *q;
        E *p;
        D_800F2AB0[i].f1 = a;
        D_800F2AB0[i].f3 = b;
        p = (E *)(i * 20 + (int)D_800F2AB0);
        q = &D_8009F9FC[j];
        p->s[0] = q->s[0];
        p->s[1] = q->s[1];
        p->s[2] = q->s[2];
        p->s[3] = q->s[3];
        p->s[4] = q->s[4];
        p->s[5] = q->s[5];
        p->b0 = q->b0;
        D_800F2AB0[i].b1 = q->b1;
    }
}
