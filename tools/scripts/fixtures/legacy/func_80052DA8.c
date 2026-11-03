extern int func_8008C140();
extern int func_8008F274();
extern struct { char pad[0x266C]; int a; int b; int pp; int c; int d; int e; char pad2[0x14]; int g; } D_800F4DB8;

void func_80052DA8(void) {
    char sp[8];
    switch (D_800F4DB8.c) {
    case 0:
        sp[0] = 1;
        sp[1] = D_800F4DB8.a;
        if (func_8008C140(0xD, sp, 0) != 0) {
            D_800F4DB8.c++;
            return;
        }
        if (func_8008F274(1) == 0x10) {
            D_800F4DB8.d = 1;
            D_800F4DB8.e = -3;
            D_800F4DB8.b = -1;
            D_800F4DB8.c = 0;
            return;
        }
        return;
    case 1:
        D_800F4DB8.b = -1;
        D_800F4DB8.c = 0;
        D_800F4DB8.e = 0;
        D_800F4DB8.g = D_800F4DB8.a;
        break;
    }
}
