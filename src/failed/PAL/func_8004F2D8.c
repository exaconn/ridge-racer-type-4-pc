struct P { short *a, *b; };
struct S { char pad[0x19D0]; int f19D0, f19D4, f19D8, f19DC, f19E0; short f19E4, f19E6; short list[1]; };
extern struct P D_800A0374[];
extern struct S D_800F4DB8;
extern int D_800F6784;
short *func_8004F2D8(a0, a1) short *a0; int a1; {
    int n;
    int idx;
    int c;
    short *q;
    struct S *s;
    n = 0;
    idx = 0;
    if (D_800F6784 == 0) a0 = D_800A0374[(int)a0].a;
    else a0 = D_800A0374[(int)a0].b;
    if (a0 == (short *)-1) return (short *)-1;
    s = &D_800F4DB8;
    if (s->f19D0 == 0) {
        s->f19D0 = 1;
        goto reset;
    }
    if (a1 != 0) {
    reset:
        s->f19D8 = 0;
        s->f19DC = 0;
        s->f19E0 = a1;
        s->f19E4 = 0x50;
        s->f19E6 = 0x50;
        goto copy;
    }
    idx = s->f19D4;
    c = idx;
    if (*a0 > 0) {
        q = a0;
        do { q++; n++; } while (*q > 0);
        c = idx + n;
    }
    if (c >= 8) return (short *)-1;
copy:
    a1 = 0;
    if (*a0 > 0) {
        do {
            s->list[idx] = *a0;
            a0++; a1++; idx++;
        } while (*a0 > 0);
    }
    s->list[idx] = -1;
    s->f19D4 = idx;
    if (a1 == 0) s->f19D0 = 0;
    return (short *)((char *)s + idx * 2);
}
