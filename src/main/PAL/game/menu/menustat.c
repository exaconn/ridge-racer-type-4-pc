/* RODATA 0x80010538 0x18 */
#include "game/data/trkld1.h"
#include "game/menu/menuicon.h"
#include "game/ui/barsel0.h"
typedef struct { char pad[0x14]; unsigned char b14; char p15, p16; signed char b17; char p18[2]; short h1A; } S4D8;

extern int func_80052800();
extern int func_800528A8();
extern int func_800207D4();
extern int func_8001FC28();
extern int func_8003A1B4();
extern int func_80052A30();
extern int func_8001FDD4();
extern unsigned int D_800F2DE4;
extern unsigned short D_800F4D8A;
extern S4D8 D_800AD4D8;
extern char D_800ABEF8[];
extern int D_800ACC18;
extern int D_800F4D78;
extern unsigned char D_8009F070[];
extern int D_800F3A60;
extern unsigned short D_800AC5D0;
extern unsigned char D_800F4D98[];
extern char *D_800ACC14;
extern short D_800F4C6A;
extern short D_800F3A26;
extern int func_80020074();
extern unsigned char D_8009F074[];
extern signed char D_8009F108[];

void func_800302C0();
int func_8003069C();

void func_800302C0()
{
    int sp[3];
    unsigned int x;
    int r;
    S4D8 *p;
    D_800F2DE4++;
    if (D_800F2DE4 > 0x7FFFF) D_800F2DE4 = 0x1000;
    switch (D_800F4D8A) {
    case 0:
        p = &D_800AD4D8;
        if (p->h1A == 6) {
            if (func_8003AE4C(0, 1) == 1) {
                func_8002E4F4(D_800ABEF8);
                D_800F4D78 = D_800ACC18;
                r = (p->b17 - 1) / 2;
                x = D_8009F070[p->b14];
                if (r >= 3) { r = 2; x = x * 3; }
                func_80055700(r, x);
                func_8004D7B4(D_800ACC18);
                D_800F4D8A = 3;
                goto tail;
            }
        } else if (D_800F2DE4 >= 0x96) {
            if (D_800F3A60 == 0) {
                if (D_800F2DE4 < 0x177) goto tail;
            }
            D_800F4D8A = 1;
        } else {
            goto tail;
        }
        break;
    case 1:
        D_800AC5D0 = 0x20;
        D_800F4D8A = 5;
        break;
    case 3:
        if (func_8004D7D8(D_800F4D98[0], D_800F4D98[1])) D_800F4D8A = 4;
        break;
    case 4:
        if (D_800F3A60 == 0 && D_800F2DE4 < 0x177) goto tail;
        func_80052800();
        func_800528A8(0, 1);
        D_800F4D8A = 1;
        break;
    case 5:
        D_800AC5D0--;
        func_800207D4(D_800ACC14 + 0x70, (0x20 - (short)D_800AC5D0) << 3, 0x45);
        *(int *)0x1F800000 = func_8001FC28(D_800ACC14 + 0x70, *(int *)0x1F800000, 0, 0, 0x140, 0x200);
        if ((short)D_800AC5D0 <= 0) {
            if (D_800AD4F2 == 6) {
                func_8003A1B4();
                D_800F4C6A = 1;
                D_800F3A26 = 0x21;
                D_800F4D8A = 0;
                D_800ACC18 = D_800F4D78;
                func_80052A30(0);
            } else {
                D_800F4C6A = 1;
                D_800F3A26 = 0x11;
                D_800F4D8A = 0;
            }
            return;
        }
        break;
    default:
        break;
    }
tail:
    func_800307B4(D_800F2DE4);
    r = x = D_800F2DE4;
    if (x < 0x20) {
        sp[0] = (int)(x * 255 + (0x20 - x) * 8) / 32;
        sp[1] = (int)(x * 189 + (0x20 - x) * 8) / 32;
        r = 0x20 - x;
        sp[2] = r / 4;
    } else {
        sp[0] = 0xFF;
        sp[1] = 0xBD;
        sp[2] = 0;
    }
    *(int *)0x1F800000 = func_8001FDD4(D_800ACC14 + 0xB6C, *(int *)0x1F800000, 0, 0, 0x140, 0x200, sp[0], sp[1], sp[2]);
}

int func_8003069C(a, p, x, y, w, z) int a; int p; int x; int y; int w; int z; {
    int t0 = x;
    int s1 = y;
    int i = 0;
    do {
        signed char *q = &D_8009F108[i + a * 8];
        int v = *q;
        unsigned char *e;
        int s0, d, c0, c1, c3;
        i++;
        if (v == -1) break;
        e = D_8009F074 + v * 4;
        d = q[4];
        c0 = e[0]; c1 = e[1]; s0 = e[2]; c3 = e[3];
        s1 += d;
        t0 = func_80020074(p, t0, s1, w, s0, c3, c0, c1, 0x4A, 0x2E28, 1, z);
        s1 += s0;
    } while (i < 4);
    return t0;
}
