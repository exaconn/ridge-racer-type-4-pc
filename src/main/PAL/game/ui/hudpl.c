struct S { char pad[0x2AA]; short f; char pad2[0x320 - 0x2AC]; };

typedef struct { int x; } __attribute__((packed)) U4;

extern struct S D_800ABEF8[];
extern char *D_800ACC14;
extern int D_800F3A30;
extern int func_8001FF84(), func_8001FCC8();
extern U4 D_8009EE4C[];

void func_80021C00(int a0);
void func_80021C94();

void func_80021C00(int a0) {
    int a1, a2;
    char *p;
    a2 = a0 - 1;
    if (a0 == 0) {
        a1 = 7;
        a2 = 0;
    } else {
        a1 = a2 * 6 + 1;
    }
    p = D_800ACC14 + (a1 * 0x14 + 0x22688);
    if (D_800ABEF8[a2].f < D_800F3A30) {
        *(short *)(p + 0x10) = 0;
        return;
    }
    *(short *)(p + 0x10) = 0x14;
}

void func_80021C94(a0, a1)
int a0; int a1;
{
    U4 buf;
    int s4, s5, a, i;
    if (a0 == 0) {
        s5 = 0xC;
        s4 = D_800ACC14 + 0x70;
    } else {
        s4 = D_800ACC14 + 0xB70;
        s5 = ((a0 - 1) << 7) + 0xC;
    }
    a = *(int *)0x1F800000;
    i = 0;
    do {
        buf = D_8009EE4C[i];
        a = func_8001FF84(s4, a, 0xEC + i * 0x38, s5, &buf, 0x7EF8, 0);
        i++;
    } while (i < 2);
    buf = D_8009EE4C[2];
    i = 0;
    if (a1 <= 0) {
        a1 = 1;
    }
    do {
        if (i == 0) {
            *(char *)&buf = a1 * 0xC;
        }
        if (i == 1) {
            *(char *)&buf = D_800F3A30 * 0xC;
        }
        a = func_8001FF84(s4, a, 0x118 + i * 0x14, s5, &buf, 0x7EF8, 0);
        i++;
    } while (i < 2);
    *(int *)0x1F800000 = func_8001FCC8(s4, a, 0x25);
}
