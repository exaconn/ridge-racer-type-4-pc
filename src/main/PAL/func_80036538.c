extern short D_800F4D44, D_800F4C6A, D_800F3A26, D_800F4D8A;
struct w3g2s { char pad[0x17]; signed char a; char b; signed char c; short d; };
extern struct w3g2s D_800AD4D8;
extern unsigned char D_8009F72C[];

void func_80036538(void) {
    struct w3g2s *p;
    if (D_800F4D44 == 1) {
        p = &D_800AD4D8;
        if (p->c > D_8009F72C[p->a / 2]) p->d = 5;
        else p->d = 4;
    }
    D_800F4C6A = 1;
    D_800F3A26 = 0x17;
    D_800F4D8A = 0;
}
