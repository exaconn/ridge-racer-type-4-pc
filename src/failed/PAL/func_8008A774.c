struct E { unsigned f; int s; };
extern int D_800A48BC;
extern struct E *D_800A48C0;
void func_8008A774() {
    int t1, a2, t;
    struct E *a3, *v1, *a1, *a0, *t2;
    unsigned x, y, z;
    t1 = 0;
    if (D_800A48BC >= 0) {
        a3 = D_800A48C0;
        do {
            a2 = t1 + 1;
            if (a3->f & 0x80000000) {
                v1 = D_800A48C0 + a2;
            lp:
                if (v1->f == 0x2FFFFFFF) { v1++; a2++; goto lp; }
                a1 = D_800A48C0 + a2;
                x = a1->f;
                if ((x & 0x80000000) && (x & 0x0FFFFFFF) == (a3->f & 0x0FFFFFFF) + a3->s) {
                    a1->f = 0x2FFFFFFF;
                    a3->s = a3->s + a1->s;
                    goto next;
                }
            }
            a3++;
            t1++;
        next: ;
        } while (D_800A48BC >= t1);
    }
    t1 = 0;
    if (D_800A48BC >= 0) {
        v1 = D_800A48C0;
        do {
            if (v1->s == 0) v1->f = 0x2FFFFFFF;
            t1++;
            v1++;
        } while (D_800A48BC >= t1);
    }
    t1 = 0;
    if (D_800A48BC >= 0) {
        t2 = D_800A48C0;
        for (;;) {
            if (!(t2->f & 0x40000000)) {
                a2 = t1 + 1;
                if (D_800A48BC >= a2) {
                    a0 = D_800A48C0 + a2;
                    for (;;) {
                        y = a0->f;
                        if (y & 0x40000000) break;
                        z = t2->f;
                        if ((y & 0x0FFFFFFF) < (z & 0x0FFFFFFF)) {
                            t2->f = y;
                            t = t2->s;
                            t2->s = a0->s;
                            a0->f = z;
                            a0->s = t;
                        }
                        a2++;
                        a0++;
                        if (!(D_800A48BC >= a2)) break;
                    }
                }
                t1++;
                t2++;
                if (D_800A48BC >= t1) continue;
            }
            break;
        }
    }
    t1 = 0;
    if (D_800A48BC >= 0) {
        a0 = D_800A48C0;
        for (;;) {
            x = a0->f;
            if (x & 0x40000000) break;
            if (x == 0x2FFFFFFF) {
                a1 = D_800A48C0 + D_800A48BC;
                a0->f = a1->f;
                D_800A48BC = t1;
                a0->s = a1->s;
                break;
            }
            t1++;
            a0++;
            if (!(D_800A48BC >= t1)) break;
        }
    }
    t1 = D_800A48BC - 1;
    if (t1 >= 0) {
        a0 = D_800A48C0 + t1;
        for (;;) {
            x = a0->f;
            if (!(x & 0x80000000)) break;
            t = D_800A48BC;
            a0->f = (x & 0x0FFFFFFF) | 0x40000000;
            D_800A48BC = t1;
            t1--;
            a0->s = a0->s + D_800A48C0[t].s;
            a0--;
            if (t1 < 0) break;
        }
    }
}
