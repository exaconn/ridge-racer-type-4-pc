/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/gpu_reset.h"

typedef struct { short a,b,c,d,e,f,g,h; char i,j,k,l; } W3L2_A;

extern int func_80092FFC();
extern unsigned short func_800959E4();
extern unsigned short func_80095A24();
extern int func_8008BDF8();

int func_80092734(int *pix, int tp, int abr, int x, int y, int w, int h);
int func_8009281C(int a, int b, int c);
int func_80092880(int a, int b, int c);
char *func_800928E4(char *p, int a, int b, int c, int d);
W3L2_A *func_80092998(W3L2_A *p, int a, int b, int c, int d);
s32 func_800929D4(s32 mode);

int func_80092734(int *pix, int tp, int abr, int x, int y, int w, int h) {
    short sp[4];
    int r;
    sp[0] = x;
    sp[1] = y;
    sp[3] = h;
    switch (tp) {
    case 0:
        sp[2] = w / 4;
        break;
    case 1:
        sp[2] = w / 2;
        break;
    case 2:
        sp[2] = w;
        break;
    }
    func_80092FFC(sp, pix);
    /* ASM_BARRIER: boost x and abr reference counts so global-alloc assigns $s1 to x, $s2 to abr, and $s3 to y */
    __asm__ volatile("" : : "r"(x), "r"(abr));
    r = func_800959E4(tp, abr, x, y);
    /* ASM_BARRIER: keep andi right after the call, before the epilogue loads */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}

int func_8009281C(int a, int b, int c) {
    short sp[4];
    int r;
    sp[2] = 0x100;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    func_80092FFC(sp, a);
    r = func_80095A24(b, c);
    /* ASM_BARRIER: keep andi right after the call, before the epilogue loads */
    __asm__ volatile("" : "=r"(r) : "0"(r));
    return r;
}

int func_80092880(int a, int b, int c) {
    short sp[4];
    int r;
    sp[2] = 0x10;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    func_80092FFC(sp, a);
    /* ASM_BARRIER: keeps the u16 mask (andi) scheduled right after the call, before the epilogue loads */
    r = func_80095A24(b, c) & 0xFFFF; __asm__ volatile("" : "=r"(r) : "0"(r)); return r;
}

char *func_800928E4(char *p, int a, int b, int c, int d) {
    int t = func_8008BDF8();
    *(short *)p = a;
    *(short *)(p + 2) = b;
    *(short *)(p + 4) = c;
    *(short *)(p + 0xC) = 0;
    *(short *)(p + 0xE) = 0;
    *(short *)(p + 0x10) = 0;
    *(short *)(p + 0x12) = 0;
    p[0x19] = 0;
    p[0x1A] = 0;
    p[0x1B] = 0;
    p[0x16] = 1;
    *(short *)(p + 6) = d;
    if (t) p[0x17] = d < 0x121; else p[0x17] = d < 0x101;
    *(short *)(p + 8) = a;
    *(short *)(p + 0xA) = b;
    *(short *)(p + 0x14) = 0xA;
    p[0x18] = 0;
    return p;
}

W3L2_A *func_80092998(W3L2_A *p, int a, int b, int c, int d) {
    p->a = a; p->b = b; p->c = c;
    p->e = 0; p->f = 0; p->g = 0; p->h = 0;
    p->j = 0; p->i = 0; p->l = 0; p->k = 0;
    p->d = d;
    return p;
}

s32 func_800929D4(s32 mode)
{
    RrGpuResetState80 *state;
    switch(mode & 7) {
    case 0:
    case 3:
        func_8001E84C(D_80011DF0,&D_800ABA78,&D_800ABAC0);
    case 5:
        state=&D_800ABAC0;
        func_800959B8((s8 *)state,0,128);
        func_8008B364();
        func_80096E24((unsigned long)D_800ABAB8 & 0xFFFFFF);
        state->kind00=func_800950D4(mode);
        state->enabled01=1;
        /* Preserve the explicit byte reload after the reset service. */
        state->debug02.width=D_800ABB40[*(volatile u8 *)&state->kind00].value;
        state->debug02.height=D_800ABB4C.dimensions[state->kind00].value;
        func_800959B8((s8 *)state->queue10,-1,0x5C);
        func_800959B8((s8 *)state->tail6C,-1,0x14);
        return state->kind00;
    default:
        if(D_800ABAC2.level>=2) D_800ABABC(D_80011E10,mode);
        return D_800ABAB8->reset34(1);
    }
}
