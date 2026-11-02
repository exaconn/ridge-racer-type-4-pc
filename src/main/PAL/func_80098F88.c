/* GCC_VERSION: 2.7.2 */
struct S2 {
    char p0[2];
    unsigned char b2, b3, b4, b5, b6, b7;
};

struct E4 {
    unsigned char b0, b1, b2, b3, b4;
};

struct E8 {
    unsigned char b0;
    char pad[3];
    unsigned char *p4;
};

struct S {
    unsigned short *f0;
    struct E4 *f4;
    struct E8 *f8;
    char p0[0x30];
    struct S2 *q;
    char p1[6];
    unsigned char st, cnt, c48, f49;
    char p3[0x99];
    unsigned char c3, c4;
    unsigned short s6;
    char p4[1];
    unsigned char c9, ca, cb;
    unsigned short sc, se;
};

extern unsigned char *D_80114438;

int func_80098F88(struct S *s) {
    struct S2 *q;
    struct E4 *p5;
    struct E8 *e8;
    unsigned char *src;
    int i;
    int t;

    switch (s->st) {
    case 2:
        if (s->q->b2 != 0) return 0;
        if (s->q->b3 != 0) return 0;
        s->f0[s->cnt] = s->q->b5 + (s->q->b4 << 8);
        if (s->se != s->f0[s->cnt]) {
            s->se = s->f0[s->cnt];
            return 0;
        }
        s->se = 0;
        s->cb = 0;
        if (++s->cnt < s->c3) return 0;
        s->cnt = 0;
        break;

    case 3:
        q = s->q;
        if (q->b2 != 0) return 0;
        if (q->b3 != 0) return 0;
        p5 = &s->f4[s->cnt];
        if (p5->b0 == q->b4 && p5->b1 == (*(volatile unsigned char *)&q->b5 & 0x7F) && p5->b2 == q->b6 && p5->b3 == q->b7 && p5->b4 == ((t = q->b5) >> 7)) {
            s->se = 0;
        } else {
            s->se = 0xFFFF;
        }
        p5->b0 = s->q->b4;
        p5->b1 = s->q->b5 & 0x7F;
        p5->b2 = s->q->b6;
        p5->b3 = s->q->b7;
        t = s->q->b5;
        p5->b4 = t >> 7;
        if (s->se != 0) return 0;
        s->cb = 0;
        if (++s->cnt < s->c9) return 0;
        s->cnt = 0;
        s->c48 = 0;
        break;

    case 4:
        q = s->q;
        if (q->b2 != 0) {
            s->c48 = 0;
            return 0;
        }
        e8 = &s->f8[s->cnt];
        if (s->c48 == 0) {
            i = 3;
            e8->b0 = s->c48 = q->b4;
            src = &s->q->b5;
            if (s->cnt == 0) {
                D_80114438 = e8->p4 = (unsigned char *)(s->f8 + s->ca);
            } else {
                D_80114438 = e8->p4 = e8[-1].p4 + ((e8[-1].b0 + 3) & 0x1FC);
            }
        } else {
            i = 5;
            src = &q->b3;
        }
        while (i-- != 0) {
            if (s->c48 == 0) break;
            if (D_80114438 >= &s->c3) {
                s->cnt = 0;
                s->c48 = 0;
                return 0;
            }
            if (*D_80114438 != *src) {
                s->se = 0xFFFF;
            }
            {
                unsigned char **pp = &D_80114438;
                unsigned char *p = *pp;
                *p = *src++;
                *pp = p + 1;
            }
            s->c48--;
        }
        if (s->c48 == 0) {
            if (s->se != 0) {
                s->se = 0;
                s->c48 = 0;
                return 0;
            }
            if (++s->cnt >= s->ca) {
                s->f49 = 6;
                s->st = 0xFE;
                s->cb = 0;
                return 0;
            }
            s->c48 = 0;
            s->cb = 0;
            return 0;
        }
        return 0;

    default:
        return 1;
    }
    return 1;
}
