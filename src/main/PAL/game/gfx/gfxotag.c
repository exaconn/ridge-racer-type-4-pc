typedef struct { unsigned addr : 24; unsigned len : 8; } P_TAG;

struct DB { char pad[0x70]; unsigned int ot[1]; };

extern struct {
    char pad0[0x50];
    int f50[2];
    int f58[2];
    char pad60[0x55C - 0x60];
    char *f55C;
} D_800AC650;
extern short D_800AD520;
extern int D_800F4BF0;
extern int D_800F2FD8;
extern struct DB *D_800ACC14;
extern short D_8009FE60[];
extern short D_800F3914;
extern void func_80044920();
extern void func_8006EE3C();
extern int func_80044B24();
extern void func_8009323C();
extern void func_800701C4();
extern void func_8005F0D0();
extern void func_80044BA0();
extern void func_8006E1AC();
extern void func_80090EC4();
extern void func_800912D4();
extern void func_80091D64();

void func_80044EC4(void);
void func_800451E4(int *a);

void func_80044EC4(void) {
    int sp10[8];
    int *g = (int *)0x1F800000;
    int s5;
    char *s2;
    int s1;

    s5 = g[0x18];
    s2 = D_800AC650.f55C + (D_800AD520 << 7);
    func_80044920(s2, sp10);
    func_8006EE3C(0x1F80007C, s2, sp10);
    s1 = func_80044B24(sp10, s2);
    if (s1 > 0) {
        func_8009323C(D_800AC650.f50[D_800F4BF0], 0x15F);
        g[1] = D_800AC650.f50[D_800F4BF0];
        g[0x18] = s1 >> 7;
        if (s1 >> 7 >= 6) {
            g[0x18] = 5;
        }
        func_800701C4(8);
        func_8006EE3C(0x1F80007C, s2, sp10);
        g[0x1E] = 0;
        func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
        s1 >>= s5;
        s1 += 0x6E;
        if (s1 > 0) {
            func_80044BA0(s1 + 1);
            ((P_TAG *)g[1])->addr = ((P_TAG *)&D_800ACC14->ot[s1])->addr;
            ((P_TAG *)&D_800ACC14->ot[s1])->addr = g[1] + 0x578;
        }
    }
    if (D_8009FE60[D_800F3914] == 7) {
        s2 = D_800AC650.f55C + (D_800AD520 << 7) + 0x40;
        func_80044920(s2, sp10);
        func_8006EE3C(0x1F80007C, s2, sp10);
        s1 = func_80044B24(sp10, s2);
        if (s1 > 0) {
            func_8009323C(D_800AC650.f58[D_800F4BF0], 0x15F);
            g[1] = D_800AC650.f58[D_800F4BF0];
            g[0x18] = s1 >> 7;
            if (s1 >> 7 >= 6) {
                g[0x18] = 5;
            }
            func_800701C4(9);
            func_8006EE3C(0x1F80007C, s2, sp10);
            *(int *)0x1F800078 = 0;
            func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
            s1 >>= s5;
            s1 += 0x6E;
            if (s1 > 0) {
                func_80044BA0(s1 + 1);
                ((P_TAG *)g[1])->addr = ((P_TAG *)&D_800ACC14->ot[s1])->addr;
                ((P_TAG *)&D_800ACC14->ot[s1])->addr = g[1] + 0x578;
            }
        }
    }
    g[0x18] = s5;
    g[1] = (int)D_800ACC14 + 0x70;
}

void func_800451E4(int *a) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int t[3];
    int *s = (int *)0x1F800000;
    s[6] = -*(short *)((char *)a + 0x10);
    s[7] = *(short *)((char *)a + 0x12);
    s[8] = *(short *)((char *)a + 0x14);
    v[0] = *(short *)((char *)a + 0x10);
    v[1] = *(short *)((char *)a + 0x12);
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -*(unsigned short *)((char *)a + 0x1C);
    func_80091D64(v, m);
    func_80090EC4(m, r, t);
    s[2] = a[0] + t[0];
    s[3] = a[1] + t[1];
    s[4] = a[2] + t[2];
    func_800912D4(a[8]);
    func_8006E1AC();
}
