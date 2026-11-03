/* RODATA 0x800110a0 0x1c */
/* CC1_FLAGS: -fno-strength-reduce */
extern int func_8004E3B0();
extern struct { char pad[0x56C]; int a, b, c, d; } D_800AC650;
extern struct { char pad[0x10]; int f10; char pad2[0x19CC - 0x14]; int f19cc; } D_800F4DB8;
extern short D_800F4D44;
extern short D_800F3826;
extern short D_800AD508;
extern int D_800F6784;
extern int D_800ACBC8;
extern int D_800F4DC8;
extern int func_8004E194();
extern int func_8004E260();
extern int D_800A0218;
extern int D_800A021C;
extern int D_800A0220;
extern int D_800A0224;
extern unsigned short D_800A02C0;
extern int D_800ACC18;
extern short D_800F3914;
extern int D_8010C1F0;
extern int func_8001223C(), func_8001292C(), func_80012A58(), func_80012A68(), func_800436E8();
extern char *func_8004D4BC();
extern int func_8004ED38(), func_8004F600(), func_80070108();
extern char D_800A0234[];
extern char *D_800ACBB4;
extern char D_8010C210[];

void func_8004E414(void);
int func_8004E5E8(void);
int func_8004E70C(void);

void func_8004E414(void) {
    switch (D_800F4D44) {
    case 6:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800F4DB8.f10 = 0;
        D_800AC650.d = D_800F3826 + 1;
        break;
    case 0:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800AC650.d = -1;
        D_800F4DB8.f10 = 0;
        break;
    default:
    case 1:
    case 5:
        D_800F4DB8.f19cc = 0;
        D_800AC650.a = 4;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = -1;
        D_800AC650.d = 0;
        D_800F4DB8.f10 = 0;
        break;
    case 2:
    case 3:
        D_800F4DB8.f19cc = 1;
        D_800AC650.a = 5;
        D_800AC650.b = func_8004E3B0(0);
        D_800AC650.c = func_8004E3B0(1);
        D_800AC650.d = -1;
        D_800F4DB8.f10 = 1;
        break;
    case 4:
        D_800F6784 = 1;
        D_800AC650.a = 5;
        if (D_800AD508 != 0) {
            D_800AC650.b = func_8004E3B0(0);
            D_800AC650.c = func_8004E3B0(1);
        } else {
            D_800AC650.b = func_8004E3B0(2);
            D_800AC650.c = func_8004E3B0(3);
        }
        D_800ACBC8 = -1;
        D_800F4DC8 = 1;
        break;
    }
}

int func_8004E5E8(void) {
    switch (D_800A0218) {
    case 0:
        func_8004E414();
        D_800A0218 += 1;
    case 1:
        if (func_8004E194() != 0) {
            D_800A021C = 0;
            D_800A0220 = 0;
            D_800A0224 = 0;
            D_8010C1F0 = D_800ACC18;
            *(int *)&D_800A02C0 = D_800F3914;
            if (D_800F3914 >= 8) {
                D_800F3914 = 7;
            }
            D_800A0218 += 1;
        }
        return 0;
    case 2:
        if (func_8004E260() != 0) {
            D_800A0218 += 1;
            D_800F3914 = D_800A02C0;
        }
        return 0;
    case 3:
        return 1;
    default:
        return 0;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC18 (*(char *(*))&D_800ACC18)
#define D_800F4DB8 (*(char (*)[])&D_800F4DB8)
#define D_8010C1F0 (*(char *(*))&D_8010C1F0)

int func_8004E70C(void)
{
    int i, i4, r, ret;
    char *s1, *s3;
    char *t;
    switch (D_800A021C) {
    case 0:
        ret = r = func_8001292C(func_8001223C(2, 1), D_8010C1F0);
        if (r == 0) break;
        D_800A021C = D_800A021C + 1;
    case 1:
        if (func_80012A58() == 0) { ret = 0; break; }
        t = D_8010C1F0 + *(int *)(D_8010C1F0 + 4);
        func_80070108(t, 7);
        i = 0;
        func_80012A68(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
        s3 = D_800F4DB8;
        s1 = D_800A0234;
        D_800ACBB4 = D_8010C1F0 + *(int *)(D_8010C1F0 + 8);
        do {
            *(char **)D_8010C210 = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x14);
            *(char **)(D_8010C210 + 4) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x1C);
            func_8004ED38(D_8010C210, *(int *)(s1 + 4), *(int *)(s1 + 8));
            s1 += 0xC;
            *(char **)(s3 + 0x2858) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0xC);
            s3 += 4;
            i++;
        } while (i < 2);
        func_800436E8(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x24));
        t = func_8004D4BC(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x1C));
        D_800ACC18 = t;
        D_8010C1F0 = t;
        D_800A021C = 0;
        func_8004F600();
        ret = 1;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

#undef D_800ACC18
#undef D_800F4DB8
#undef D_8010C1F0
