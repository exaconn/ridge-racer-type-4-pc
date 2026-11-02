extern short func_80088608();
struct A { short a; short b; short c; short d; short e; short f; char pad[0x36 - 12]; };
extern char D_8010F916[], D_8010FE18[], D_8010FE3C[], D_8010F918[], D_8010F91C[], D_8010F91E[], D_8010F920[];
extern int D_801100F8;
extern char D_80110107;
extern char D_8011010C[];
int func_80089344(a0, a1, a2, a3, a4) short a0; short a1; short a2; short a3; int a4; {
    short t0;
    int v0;
    int a;
    unsigned short s;
    int e;
    int x;
    int idx;
    t0 = a4 - 0x40;
    a = a0 * 0x36;
    v0 = 0;
    if (*(short *)(D_8010F918 + a) == a1) {
        v0 = 0;
        if (*(short *)(D_8010F920 + a) == a2) {
            v0 = 0;
            if (*(short *)(D_8010F91C + a) == a3) {
                s = *(unsigned short *)(D_8010F916 + a);
                idx = *(short *)(D_8010F91E + a) + D_80110107 * 16;
                if (t0 > 0) {
                    x = *(unsigned char *)(((idx & 0xFFFF) << 5) + D_801100F8 + 0xD);
                    s += (t0 * x) / 63;
                    e = ((t0 * x) % 63) * 2;
                } else if (t0 < 0) {
                    x = t0 * *(unsigned char *)(((idx & 0xFFFF) << 5) + D_801100F8 + 0xC);
                    s = s + (x / 64) - 1;
                    e = ((x - ((x / 64) << 6)) * 2) + 0x7F;
                } else e = 0;
                *(short *)(D_8011010C + 0xC) = a0;
                *(unsigned char *)D_8011010C = *(unsigned char *)(D_8010F91E + a0 * 0x36);
                *(short *)(D_8010FE3C + a0 * 16) = func_80088608(s & 0xFFFF, e & 0xFFFF, s);
                *(unsigned char *)(D_8010FE18 + a0) |= 4;
                v0 = 1;
            }
        }
    }
    return v0;
}
