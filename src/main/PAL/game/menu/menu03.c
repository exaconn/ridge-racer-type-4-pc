typedef struct { int x; } __attribute__((packed)) W7C0_U;
typedef struct { char pad[0x24]; } W7C0_E;
typedef struct { char p0; signed char b1; unsigned char b2; } W7C0_S;
typedef struct { char pad[2]; short s2; char pad2[0xC]; short s10; } W7C0_F;
extern char D_800ABEF8[];
extern int D_800ACC18;
extern char D_800AD4D8[];
extern W7C0_S *D_800F2AB0;
extern int D_800F2DE4;
extern short D_800F3A26;
extern int D_800F3A60;
extern W7C0_F *D_800F3AFC;
extern short D_800F4C6A;
extern int D_800F4D70;
extern W7C0_E *D_800F4D80;
extern unsigned short D_800F4D8A;
extern unsigned char D_800F4D98[];
extern void func_8002A344();
extern void func_8002AF8C();
extern void func_8002B330();
extern void func_8002CC88();
extern void func_8002E5A0();
extern void func_80033A74();
extern void func_80037198();
extern void func_8003A198();
extern int func_8003AE4C();
extern void func_8003D6F0();
extern void func_8003EC88();
extern void func_8003FBA8();
extern void func_8004F24C();
extern void func_8005AF5C();
extern void func_8006EF8C();
#define P8(base, off) (*(unsigned char *)((char *)(base) + (off)))
#define IDX30(base, i) (*(unsigned char *)((char *)(base) + (i) * 0x24 + 0x30))

void func_80039C0C(void)
{
    int s0, s2, c2, v1, a0, a1;
    char *t0;
    unsigned char *q;
    unsigned char sp18[4];
    int s1;

    D_800F2DE4 = D_800F2DE4 + 1;
    func_8002A344(0x10022);
    s2 = func_8003AE4C(1, 3);
    if (s2 == 1) {
        v1 = D_800ACC18;
        s0 = D_800F4D8A;
        D_800F4D70 = v1;
        if (s0 == s2)
            goto b1;
        if (s0 < 2) {
            if (s0 == 0)
                goto b0;
            goto end;
        }
        if (s0 == 2)
            goto b2;
        goto end;
    b0:
        s0 = P8(D_800F4D80, 0x14);
        if (s0 == 2) {
            func_8002AF8C(1, 1, 0);
            P8(D_800F4D80, 0xC) = s0;
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                P8(D_800F4D80, 0xC) = s2;
                P8(D_800F4D80, 0x38) = 0;
                P8(D_800F4D80, 0x5C) = 0;
                D_800F4D8A = s2;
            }
        }
        goto end;
    b1:
        t0 = (char *)D_800F4D80;
        v1 = P8(t0, 0x38);
        c2 = 2;
        if (v1 == c2) {
            q = D_800F4D98;
            IDX30(t0, q[3]) = 0;
            func_8002B330(0, 1, q + 3, c2, 0);
            IDX30(D_800F4D80, q[3]) = c2;
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                a1 = 0;
                a0 = 0;
                IDX30(D_800F4D80, q[3]) = s2;
                for (a1 = 0; a1 < 3; a1++)
                    P8(D_800F4D80, a1 * 0x24 + 0x14) = 3;
            }
        } else if (v1 == 5) {
            D_800F4D8A = c2;
        }
        goto end;
    b2:
        D_800F2DE4 = 0;
        D_800F2AB0->b1 = 3;
        D_800F3AFC->s10 = 4;
        func_8003A198();
        {
        char *b = D_800AD4D8;
        *(W7C0_U *)(b + 0x1C) = *(W7C0_U *)D_800F4D98;
        if ((signed char)b[0x17] <= 0)
            *(short *)(b + 0x1A) = s0;
        }
        func_8005AF5C();
        D_800F4C6A = s2;
        D_800F3A26 = 0xB;
        D_800F4D8A = 0;
    }
end:
    func_80033A74(1);
    func_8006EF8C(6);
    if (D_800F2AB0->b1 > 0) {
        char *g = D_800ABEF8;
        sp18[0] = 0x80;
        sp18[1] = 0x80;
        sp18[2] = 0x80;
        *(int *)(g + 0x54) = (*(int *)(g + 0x54) + 8) & 0xFFF;
        if (D_800F2AB0->b1 == 2)
            s1 = 0x100;
        else
            s1 = (D_800F2AB0->b2 << 8) / 30;
        D_800F3AFC->s2 = 1;
        func_8003EC88(0);
        func_8003EC88(1);
        func_8003EC88(2);
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        func_8002CC88(D_800ABEF8, *(int *)sp18, s1, 1);
    }
    func_8003D6F0(0);
    func_8003FBA8(0);
}
