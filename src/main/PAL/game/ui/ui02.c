/* RODATA 0x800108b4 0x24 */
typedef struct {
    short s0, s2, x, y, w, h, st, cnt;
} E;
extern int func_80022694();
extern unsigned char D_8009F9C0[];
extern unsigned char D_8009F9CC[];
extern int D_800AC540;
extern int D_800ACC00;
#define EP ((E *)((a0 * 16) + D_800ACC00))
extern int D_800ACC14;

int func_8003DCEC(int a0) {
    int lx[4];
    int ly[4];
    int lv[4];
    int r;
    int hh;
    int v;
    int k;
    int a1, a3;
    E *e0, *e1, *e2, *e3, *e4, *e5;
    unsigned char *p;
    int *q;
    int *q2;
    int *xs, *ys;
    unsigned char *tb;

    v = a0 < D_800AC540;
    if (v != 0) {
        r = 0;
        e0 = EP;
        a1 = e0->st;
        a3 = e0->cnt;
        k = 3;
        q = &lv[3];
        do {
            *q = 0;
            k--;
            q--;
        } while (k >= 0);
        e1 = EP;
        lx[0] = e1->x;
        ly[0] = e1->y;
        lx[1] = e1->x + e1->w - 1;
        ly[1] = e1->y;
        lx[2] = e1->x;
        ly[2] = e1->y + e1->h - 1;
        lx[3] = e1->x + e1->w - 1;
        ly[3] = e1->y + e1->h - 1;
        switch (a1) {
        case 0:
            e2 = EP;
            e2->st = 1;
            e2->cnt = 0;
            break;
        case 5:
            r = 1;
            hh = 0x100;
            k = 3;
            q2 = &lv[3];
            do {
                *q2 = hh;
                k--;
                q2--;
            } while (k >= 0);
            break;
        case 4:
            lv[2] = 0x100;
        case 3:
            lv[1] = 0x100;
        case 2:
            lv[0] = 0x100;
        case 1:
            lv[a1 - 1] = (a3 << 8) / 6;
            if (a3 < 6) {
                e3 = EP;
                e3->cnt = e3->cnt + 1;
            } else {
                e4 = EP;
                e4->cnt = 0;
                e4->st = e4->st + 1;
            }
            break;
        case 8:
            e5 = EP;
            if (e5->s2 != 0) {
                e5->st = 1;
            }
            break;
        default:
            break;
        }
        k = 0;
        xs = lx;
        ys = ly;
        tb = D_8009F9CC;
        p = D_8009F9C0;
    loop:
        func_80022694(D_800ACC14 + 0x70, *(int *)((char *)xs + (p[0] << 2)), *(int *)((char *)ys + (p[0] << 2)), *(int *)((char *)xs + (p[1] << 2)), *(int *)((char *)ys + (p[1] << 2)), lv[*(unsigned char *)(k + (int)tb)], 1, 8, 8, 8, -1);
        p += 2;
        k++;
        if (k < 6) goto loop;
        v = r;
    }
    return v;
}
