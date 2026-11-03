/* RODATA 0x80010514 0x24 */
#include "rr/menu_transition.h"
#include "game/menu/menutran.h"
#include "game/data/ldscn1.h"

typedef struct { int a; short b; char c; char d; } S8;

typedef struct { int x; } __attribute__((packed)) U4;

extern s16 D_800FFC18;
extern int func_80096434();
extern int func_80032E9C();
extern unsigned char D_8009F06C[];
extern short D_800ABEC8;
extern short D_800AC648;
extern short D_800AD520;
extern short D_800F3826;
extern short D_800F3914;
extern signed char D_800AD4D8[];
extern signed char D_800AD4EF;
extern unsigned char D_800AD4F0;
extern unsigned char D_800F3981;
extern S8 D_800F2A68[];
extern unsigned char D_800F4D98[];
extern int D_800F3920[];

void func_8002F5BC(void);
void func_8002F688();
int func_8002F8E4();

void func_8002F5BC(void){
 switch(D_800F4D2C){
 case 0:D_800AD50A=0x105;break;
 case 1:default:D_800AD50A=0x131;break;
 case 2:D_800AD50A=0x137;break;
 case 3:D_800AD50A=0x127;break;
 case 6:D_800AD50A=0x201;break;
 case 4:D_800AD50A=0x149;break;
 case 5:D_800FFC18=0;D_800AD50A=0x147;break;
 case 7:D_800AD50A=0x14E;break;
 case 8:D_800AD50A=0x13B;break;
 }
}

void func_8002F688() {
    int x, c0, i0, j;
    int a0;
    register int v1;
    int a2;
    char *s0, *p;
    a2 = (unsigned char)D_800AD4D8[0x17];
    i0 = (signed char)a2;
    if (i0 != -1) {
        if (D_800AD4D8[0x19] <= D_8009F06C[i0 / 2]) {
            if (D_800AD4D8[i0] != -1) {
                D_800AD4D8[0x17] = a2 + 1;
            }
        }
    }
    a0 = D_800AD4EF;
    if (a0 == -1) {
        func_80096434(D_800F2A68, 0x40);
        j = 0;
        do {
            D_800F2A68[j].c = j;
            D_800F2A68[j].b = j;
            j++;
        } while (j < 8);
        v1 = 0;
        func_80096434(D_800F4D98, 0x20);
        func_80096434(D_800AD4D8, 0x2C);
        a0 = 0;
        v1 = 0;
        a2 = -1;
        *(U4 *)&D_800AD4D8[0x1C] = *(U4 *)&D_800F4D98[0];
        D_800ABEC8 = 0;
        do {
            p = (char *)D_800AD4D8 + v1;
            v1++;
            p[0] = a2;
            p[8] = 3;
        } while (v1 < 8);
        D_800AD4F0 = D_800F3981;
    }
    x = (unsigned char)D_800AD4D8[0x17];
    x = (x << 24) >> 25;
    D_800AD4D8[0x15] = x;
    if ((unsigned)(x & 0xFF) >= 3) {
        D_800AD4D8[0x15] = 2;
    }
    *(U4 *)&D_800F4D98[0] = *(U4 *)&D_800AD4D8[0x1C];
    D_800AC648 = a0;
    c0 = D_800F4D98[0];
    D_800AD520 = c0;
    D_800F3826 = D_800F4D98[1];
    D_800F4C28 = D_800F4D98[2];
    D_800F3914 = func_80032E9C(a0, c0, a2);
    D_800ABEC8 = 0;
    D_800F3920[3] |= 1 << D_800AC648;
    if (D_800F4D98[2] >= 5) {
        D_800ABEC8 = (D_800F4D98[2] - 5) % 7;
        return;
    }
    if (D_800F4D98[2] != 0) {
        D_800ABEC8 = D_800F4D98[2] - 1;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3920 (*(char (*)[])&D_800F3920)

int func_8002F8E4() {
    int b, off, i;
    /* ASM_BARRIER: pin reproduces original register assignment */
    register int n __asm__("$7");
    int a;
    unsigned v;
    n = 0;
    a = 0;
    do {
        char *p = D_800F3920;
        b = 0;
        off = a << 4;
      L1:
        i = 0;
        v = *(unsigned *)(off + (int)p + 0x10);
        do {
            if ((v >> i) & 1) n++;
            i++;
        } while (i < 20);
        b++;
        off += 4;
        if (b < 4) goto L1;
        a++;
    } while (a < 4);
    return n;
}

#undef D_800F3920
