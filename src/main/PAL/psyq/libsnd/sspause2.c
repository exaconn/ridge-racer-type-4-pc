/* GCC_VERSION: 2.7.2 */
#include "psyq/libsnd/ssatr2.h"
#include "rr/channel_stream.h"
typedef struct { unsigned char *c0; int c4, c8, cC; char p10[4]; unsigned char b14; char p15[2]; unsigned char b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f, b20, b21; char p22[4]; signed char b26; unsigned char b27[0x10]; unsigned char b37[0x10]; char p47[1]; short h48[8]; short h58[4]; unsigned short h60[0x10]; short h80; char p82[6]; int w88; int w8c; int w90; char p94[4]; int w98; int w9c; char pa0[0x10]; } Ch;

typedef struct { unsigned char *c0; int c4, c8, cC; char p10[4]; unsigned char b14; char p15[2]; unsigned char b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f, b20, b21; char p22[4]; signed char b26; unsigned char b27[0x10]; unsigned char b37[0x10]; char p47[1]; short h48[8]; unsigned short h58[4]; unsigned short h60[0x10]; short h80; char p82[6]; int w88; int w8c; int w90; char p94[4]; int w98; int w9c; char pa0[0x10]; } Ch_1;

void func_800823A4(int a0, short a1);
void func_80082440(short a);
void func_80082468(short a, short b);
void func_80082494(int a0, short a1, int a2, signed char a3);

void func_800823A4(int a0, short a1) {
    int t = a0 << 16;
    Ch **b = D_8010F068;
    Ch **q = (Ch **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    Ch *p = (Ch *)((char *)*q + o);
    int f = p->w98;
    if (!(f & 0x204)) {
        if (!(f & 0x100)) {
            p->w98 = f & ~2;
            ((Ch *)(o + (int)*q))->w98 |= 8;
            ((Ch *)(o + (int)*q))->w98 |= 1;
        }
    }
}

void func_80082440(short a) { func_800823A4(a, 0); }

void func_80082468(short a, short b) { func_800823A4(a, b); }

/* legacy typing of shared globals in this part of the unit */
#define D_8010F068 (*(Ch_1 *(*)[])&D_8010F068)

void func_80082494(int a0, short a1, int a2, signed char a3) {
    int t = a0 << 16;
    Ch_1 **b = D_8010F068;
    Ch_1 **q = (Ch_1 **)((int)b + (t >> 14));
    int o = a1 * 0xB0;
    Ch_1 *p = (Ch_1 *)((char *)*q + o);
    a2 = (signed char)a2;
    p->c0 = (unsigned char *)p->c4;
    p->c8 = p->c4;
    p->cC = p->c4;
    ((Ch_1 *)(a1 * 0xB0 + (int)*q))->w98 &= ~0x200;
    ((Ch_1 *)(a1 * 0xB0 + (int)*q))->w98 &= ~4;
    p->b20 = a3;
    if (a2 == 1) {
        ((Ch_1 *)(a1 * 0xB0 + (int)*q))->w98 |= 1;
        p->b14 = a2;
        p->b21 = 0;
        func_800897A4((short)(a0 | (a1 << 8)), p->h58[0], p->h58[1], 1);
    } else if (a2 == 0) {
        ((Ch_1 *)(a1 * 0xB0 + (int)*q))->w98 |= 2;
    }
}

#undef D_8010F068
