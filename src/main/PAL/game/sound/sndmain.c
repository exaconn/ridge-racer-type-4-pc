/* CC1_FLAGS: -fno-schedule-insns */
typedef struct { int a, b, c, d; } V4;
extern int func_80051BD4();
extern int func_800523C8();
extern int func_80052460();
extern char D_800F4DB8[];
extern char D_800ABEF8[];
#define W(o) (*(int *)(b + (o)))
#define A(o) (*(int *)(c + (o)))
#define S16(o) (*(short *)(b + (o)))
#define U16(o) (*(unsigned short *)(b + (o)))

void func_80052548(void)
{
    int v0, v1, a1, a2, a3, t0, t1;
    int q3;
    register int q5 asm("$5");
    int *g;
    char *b = D_800F4DB8;
    char *c;
    char *e;
    g = (int *)0x1F800000;
    if (W(0x18B8) != 0) {
        switch (U16(0x18C0)) {
        case 0:
            if (U16(0x18C2) == 0) {
                c = D_800ABEF8;
                v1 = A(0x184);
                q5 = A(0x188);
                W(0x18E4) = v1 + q5;
                *(V4 *)(b + 0x18C4) = *(V4 *)(c + 0x20);
                *(V4 *)(b + 0x18D4) = *(V4 *)(c + 0x50);
                func_80051BD4(0);
            } else {
                goto neg;
            }
            break;
        case 1:
            if (U16(0x18C2) == 0) {
                v0 = g[2];
                if (v0 < 0) v0 += 3;
                q3 = g[3];
                W(0x18C4) = v0 >> 2;
                if (q3 < 0) q3 += 3;
                q5 = g[4];
                v0 = q3 >> 2;
                W(0x18C8) = v0;
                if (q5 < 0) q5 += 3;
                v0 = q5 >> 2;
                W(0x18CC) = v0;
                *(V4 *)(b + 0x18D4) = *(V4 *)0x1F800018;
                func_80051BD4(1);
                func_800523C8(0);
            } else {
                v0 = g[2];
                if (v0 < 0) v0 += 3;
                q3 = g[3];
                W(0x18C4) = v0 >> 2;
                if (q3 < 0) q3 += 3;
                q5 = g[4];
                v0 = q3 >> 2;
                W(0x18C8) = v0;
                if (q5 < 0) q5 += 3;
                v0 = q5 >> 2;
                W(0x18CC) = v0;
                *(V4 *)(b + 0x18D4) = *(V4 *)0x1F800018;
                func_800523C8(0);
                func_800523C8(1);
neg:
                S16(0x18F0) = -1;
                S16(0x190C) = -1;
            }
            break;
        }
        func_80052460();
        e = D_800F4DB8;
        if (*(int *)(e + 0x18E8) > 0) *(int *)(e + 0x18E8) = *(int *)(e + 0x18E8) - 1;
    }
}
