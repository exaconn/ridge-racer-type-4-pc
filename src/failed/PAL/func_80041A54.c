typedef struct { int x, y; } __attribute__((packed)) U8;
typedef struct {
    char pad0[4];
    short f4, f6, f8, fA;
    char pad1[0x2C - 0x0C];
    short f2C;
    unsigned char f2E, f2F;
    unsigned char f30[2];
    unsigned char f32, f33;
    char pad2[0x38 - 0x34];
} E;

extern E *D_800F3A28;
extern U8 D_8009FDA8[];

void func_80041A54(a0, a1, a2, a3, a4)
int a0;
int a1;
int a2;
short a3;
short a4;
{
    int t1 = 6;
    int t2 = t1 / a2;
    E *e;
    int t0 = a0 * 0x38;

    e = (E *)(t0 + (int)D_800F3A28);
    e->f2E = t2 * 3;
    ((E *)(t0 + (int)D_800F3A28))->f2F = t1;
    e = (E *)(t0 + (int)D_800F3A28);
    e->fA = a4 / e->f2F;
    e->f2C = a2;
    e->f4 = 0x40;
    e->f6 = 8;
    e->f8 = a3 / (a2 * e->f2E);
    *(U8 *)e->f30 = D_8009FDA8[a1];
    e = (E *)(t0 + (int)D_800F3A28);
    e->f32 /= e->f2E;
    e = (E *)(t0 + (int)D_800F3A28);
    e->f33 /= e->f2F;
}
