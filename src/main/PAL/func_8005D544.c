/* RODATA 0x8001129c 0x30 */
struct E {
    int state; int mode; char pad8[0x44 - 8]; int x44; unsigned short h48; short h4A; unsigned short h4C;
};
struct Z { char pad[0x4C]; unsigned short h4C; char pad2[0xE4 - 0x4E]; struct E e; };
struct A { short h0; char pad2[0xE]; unsigned int i10; };
extern struct Z *D_800ACA28;

void func_8005D544(struct A *a0) {
    int tbl[2][3] = {{1, 2, 4}, {5, 6, 4}};
    struct Z *p = D_800ACA28;
    struct E *s = &p->e;
    int mode = s->mode;
    unsigned int t, x;
    int val, ns, m;
    if (mode < 0) return;
    switch (mode) {
    case 0:
    case 1:
    {
        switch (p->e.state) {
        case 0:
            if (a0->h0 == 0) return;
            x = a0->i10 / 50 * 2;
            x = x / 3;
            s->h4A = x - (D_800ACA28->h4C + 1);
            val = tbl[s->mode][0];
            s->h48 = 0;
            s->state = s->state + 1;
            s->x44 = val;
            break;
        case 1:
            s->h48 = s->h48 + 1;
            if ((short)s->h48 < s->h4A) return;
            m = s->mode;
            val = tbl[m][1];
            s->h48 = 0;
            s->state = s->state + 1;
            s->x44 = val;
            break;
        case 2:
            s->h48 = s->h48 + 1;
            if ((short)s->h48 < s->h4A) return;
            m = s->mode;
            val = tbl[m][2];
            s->h48 = 0;
            s->state = 3;
            s->x44 = val;
            break;
        case 4:
            s->h48 = s->h48 + 1;
            if ((short)s->h48 < s->h4A) return;
            s->h4A = (a0->i10 / 100) - (D_800ACA28->h4C + 1);
            s->x44 = 5;
            s->h48 = 0;
            s->state = s->state + 1;
            break;
        case 5:
            s->h48 = s->h48 + 1;
            if ((short)s->h48 < s->h4A) return;
            s->x44 = 6;
            s->h48 = 0;
            s->state = s->state + 1;
            break;
        }
    }
    break;
    case 2:
    {
        switch (p->e.state) {
        case 0:
            if (a0->h0 == 0) return;
            s->x44 = 3;
            s->h48 = 0;
            p->e.state = p->e.state + 1;
            break;
        case 1:
            if (a0->h0 == 0) return;
            s->x44 = 0;
            s->h48 = 0;
            p->e.state = 0;
            break;
        }
    }
    }
}
