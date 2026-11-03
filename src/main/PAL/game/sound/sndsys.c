#include "types.h"

struct B { char b[0x5BC]; };

struct E { struct B x; char pad[1568 - 0x5BC]; };

extern struct { char pad[0x614]; volatile u16 m[1][5]; char r[2]; } D_800F4DB8[];
extern struct E D_800F4E2C[];

void func_80050844(int a0, int a1, int a2, int a3);
void func_80050894(struct B *a0, int a1);

void func_80050844(int a0, int a1, int a2, int a3) {
    u16 old = D_800F4DB8[a3].m[a1][a0];
    if (a2 >= 0) D_800F4DB8[a3].m[a1][a0] = a2;
}

void func_80050894(struct B *a0, int a1) {
    D_800F4E2C[a1].x = *a0;
}
