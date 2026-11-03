/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/gpu_reset.h"
#include "m2c_macros.h"
#include "rr/gpu.h"
#include "rr/library.h"
#include "rr/draw_environment.h"
#include "psyq/libgpu/addprim.h"
#include "psyq/libgpu/gpubase2.h"

typedef struct { short a,b,c,d,e,f,g,h; char i,j,k,l; } W3L2_A;

typedef struct { int w; int s4; } A;

typedef struct { int a, b, c; } B;

typedef struct { char pad[0x1C]; unsigned w; } W3L2_P;

extern int func_8008BDF8();
extern unsigned char D_800ABAC1[];
extern int D_80011E50[];
extern char D_80011E64[];
extern char D_80011E80[];
extern M2C_UNK D_80011E94;
extern char D_80011EA8[];
extern char D_80011EB4[];
extern char D_80011EC8[];
extern char D_80011ECC[];
extern char D_80011ED8[];
/* extern */
extern M2C_UNK D_80011EE4;
extern char D_80011EF0[];
extern char D_80011EFC[];
extern char D_80011F08[];
extern char D_800ABB6C[];
extern int D_800ABB80;
extern char D_80011F20[];
extern const char D_80011F38[];
extern int func_80096444();
extern char D_80011F4C[];
extern char D_80011F64[];
extern char D_800ABAD0[];

int func_80092734(int *pix, int tp, int abr, int x, int y, int w, int h);
int func_8009281C(int a, int b, int c);
int func_80092880(int a, int b, int c);
char *func_800928E4(char *p, int a, int b, int c, int d);
W3L2_A *func_80092998(W3L2_A *p, int a, int b, int c, int d);
s32 func_800929D4(s32 mode);
s32 func_80092B48(s32 enabled);
int func_80092BA4();
u8 func_80092C48(void);
int func_80092C58();
void func_80092CB8();
void func_80092D50(s32 arg0);
void func_80092DB8(const char *name,RrGpuRectangle *rectangle);
void func_80092ED4(int a, int b, int c, int d);
void func_80092F64(int a, int b, int c, int d);
void func_80092FFC(s32 arg0, M2C_UNK arg1);
void func_8009305C(int a, int b);
int func_800930BC();
int *func_80093174();
unsigned *func_8009323C(unsigned *a, int b);
void func_800932E8(char *a0);
void func_80093344(s32 ordering_table);
int func_800933B4(int *a0);
void func_80093474(int a, W3L2_P *b);
int func_8009354C(int a);

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

/* legacy typing of shared globals in this part of the unit */
#define func_80095A24 ((int (*)())func_80095A24)

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

#undef func_80095A24

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

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*)[])&D_800ABAC2)
#define D_800ABABC (*(void (*(*))(const char *, s32, s32, s32))&D_800ABABC)

s32 func_80092B48(s32 enabled) {
    u8 *state = D_800ABAC2;
    s32 previous;
    previous = state[0];
    state[0] = enabled;
    if (enabled & 0xFF) {
        const char *format;
        void (*callback)(const char *, s32, s32, s32) = D_800ABABC;
        s32 first = state[0], second = state[-2], third = state[1];
        /* ASM_BARRIER: read the callback and state bytes before materializing the format page. */
        __asm__ volatile("" : : "r"(callback), "r"(first), "r"(second), "r"(third) : "$4");
        /* PAL read-only mapping: format + 0x1E24 is D_80011E24. */
        format = (const char *)0x80010000U;
        /* ASM_BARRIER: expose the page-relative addition in the indirect-call delay slot. */
        __asm__("" : "=r"(format) : "0"(format));
        callback(format + 0x1E24, first, second, third);
        return previous;
    }
    return previous;
}

#undef D_800ABAC2
#undef D_800ABABC

/* legacy typing of shared globals in this part of the unit */
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAB8 ((int *)D_800ABAB8)
#define func_8008B3C4 ((int (*)())func_8008B3C4)

int func_80092BA4(a0) int a0; {
    register unsigned char *p __asm__("$17"); /* ASM_BARRIER: original keeps the pointer in s1 and a0 in s0 */
    int old;
    p = D_800ABAC1;
    old = p[0];
    if (p[1] >= 2) {
        D_800ABABC(D_80011E50, a0);
    }
    if (a0 != p[0]) {
        ((int (*)())D_800ABAB8[0x34 / 4])(1);
        p[0] = a0;
        func_8008B3C4(2, 0);
    }
    return old;
}

#undef D_800ABABC
#undef D_800ABAB8
#undef func_8008B3C4

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

u8 func_80092C48(void) {
    return D_800ABAC2;
}

#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)
#define D_800ABABC (*(void (*(*))())&D_800ABABC)

int func_80092C58(a0) int a0; {
 int o;
 if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011E64, a0);
 o = *(int *)(D_800ABAC2 + 0xA);
 *(int *)(D_800ABAC2 + 0xA) = a0;
 return o; }

#undef D_800ABAC2
#undef D_800ABABC

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)
#define D_800ABABC (*(void (*(*))())&D_800ABABC)
#define D_800ABAB8 (*(struct { char pad[0x10]; int (*f)(); } *(*))&D_800ABAB8)
#define func_800959B8 ((int (*)())func_800959B8)

void func_80092CB8(a0) int a0; {
 unsigned char *p = D_800ABAC2;
 if (*p >= 2) D_800ABABC(D_80011E80, a0);
 if (a0 == 0) func_800959B8(p + 0x6A, -1, 0x14);
 D_800ABAB8->f(a0 ? 0x3000000 : 0x3000001); }

#undef D_800ABAC2
#undef D_800ABABC
#undef D_800ABAB8
#undef func_800959B8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)
#define D_800ABABC (*(M2C_UNK (*(*))(M2C_UNK *, s32))&D_800ABABC)
#define D_800ABAC2 (*(u8 (*))&D_800ABAC2)

void func_80092D50(s32 arg0) {
    if ((u8) D_800ABAC2 >= 2U) {
        D_800ABABC(&D_80011E94, arg0);
    }
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32), 0x3C)(arg0);
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

void func_80092DB8(const char *name,RrGpuRectangle *rectangle) {
    const char *label=name;
    RrGpuRectangle *rect=rectangle;
    RrGpuDebugSettings *settings=&D_800ABAC2;
    const char *format;
    switch(settings->level) {
    case 1: {
        s32 w=rect->rect.w;
        s32 maxw=settings->width;
        s32 x;
        s32 y;
        /* ASM_BARRIER: Preserve the original height temporary through the final validity tests. */
        register s32 h __asm__("$6");
        s32 maxh;
        s32 comparison;
        if(maxw<w) goto invalid;
        x=rect->rect.x;
        comparison=maxw<w+x;
        if(comparison) goto invalid;
        y=rect->rect.y;
        maxh=settings->height;
        if(maxh<y) goto invalid;
        h=rect->rect.h;
        comparison=maxh<y+h;
        if(comparison) goto invalid;
        if(w<=0) goto invalid;
        if(x<0) goto invalid;
        if(y<0) goto invalid;
        if(h>0) return;
    }
invalid:
        format=(const char *)0x80010000U;
        /* ASM_BARRIER: Expose the mapped invalid-message address for the jump delay slot. */
        __asm__("" : "+r"(format));
        format+=0x1EA8;
        goto report;
    case 2:
        format=D_80011EC8;
report:
        D_800ABABC(format,label);
        {
            s32 x=rect->rect.x;
            s32 y=rect->rect.y;
            s32 w=rect->rect.w;
            s32 h=rect->rect.h;
            /* ASM_BARRIER: Reload all rectangle fields before fetching the second diagnostic callback. */
            __asm__("" : : "r"(x),"r"(y),"r"(w),"r"(h) : "$2");
            D_800ABABC(D_80011EB4,x,y,w,h);
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

void func_80092ED4(int a, int b, int c, int d)
{
int v; char nv; nv = b & 0xFF; func_80092DB8(D_80011ECC, a); v = (((d & 0xFF) << 16) | ((c & 0xFF) << 8)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

void func_80092F64(int a, int b, int c, int d)
{
int v; char nv; int t, u, k; nv = b & 0xFF; func_80092DB8(D_80011ED8, a); u = (d & 0xFF) << 16; t = (c & 0xFF) << 8; k = 0x80000000; v = (u | (t | k)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

void func_80092FFC(s32 arg0, M2C_UNK arg1) {
    func_80092DB8(&D_80011EE4, arg0);
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32, s32, M2C_UNK, M2C_UNK), 8)(M2C_FIELD(D_800ABAB8, s32 *, 0x20), arg0, 8, arg1);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

void func_8009305C(int a, int b) {
    func_80092DB8(D_80011EF0, a);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x1C), a, 8, b);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABB60 (*(B (*))&D_800ABB60)
#define D_800ABAB8 ((int *)D_800ABAB8)

int func_800930BC(a0, a1, a2)
A *a0; int a1; int a2;
{
    B *p;
    int *o;
    int w;
    func_80092DB8(D_80011EFC, a0);
    if (*(short *)&a0->s4 == 0) return -1;
    if (*((short *)&a0->s4 + 1) != 0) {
    p = &D_800ABB60;
    w = a0->w;
    o = D_800ABAB8;
    p->b = (a2 << 16) | (a1 & 0xFFFF);
    p->a = w;
    p->c = a0->s4;
    return ((int (*)())o[2])(o[6], (char *)p - 8, 0x14, 0);
    }
    return -1;
}

#undef D_800ABB60
#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

int *func_80093174(a0, a1) int *a0; int a1; {
    /* ASM_BARRIER: pins p->$16, n->$17 to match original s-reg assignment */
    register int *p asm("$16") = a0;
    int n = a1;
    int *q; int *t; int m; int (*f)();
    if (D_800ABAC2 >= 2) { f = D_800ABABC; q = (int *)D_80011F08; f(q, p, n); }
    n--;
    if (n != 0) {
        do {
            n--;
            q = p + 1;
            ((char *)p)[3] = 0;
            m = (int)q & 0xFFFFFF; *p = (*p & 0xFF000000) | m;
            p = q;
        } while (n != 0);
    }
    t = &D_800ABB80;
    *t = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *p = (int)t & 0xFFFFFF;
    return p;
}

#undef D_800ABABC
#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

unsigned *func_8009323C(unsigned *a, int b) {
    int *q;
    if (D_800ABAC2 >= 2) D_800ABABC(D_80011F20, a, b);
    (*(int (**)())(D_800ABAB8 + 0x2C))(a, b);
    q = &D_800ABB80;
    *q = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *a = (int)q & 0xFFFFFF;
    return a;
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)

void func_800932E8(char *a0)
{
    int t = a0[3];
    (*(int (**)())(D_800ABAB8 + 0x3C))(0);
    (*(int (**)())(D_800ABAB8 + 0x14))(a0 + 4, t);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((RrLibGraphicsDriverPrefix *)D_800ABAB8)

void func_80093344(s32 ordering_table) {
    /* ASM_BARRIER: Keep the third dispatch argument distinct from the final constant zero. */
    register s32 second;
    register s32 third;
    RrLibGraphicsDriverPrefix *driver;
    if (D_800ABAC2.level >= 2) D_800ABABC(D_80011F38, ordering_table);
    second = ordering_table;
    driver = D_800ABAB8;
    /* ASM_BARRIER: Load the current driver before preparing the dispatch zeros. */
    __asm__("" : "+r"(driver) : : "$6");
    third = 0;
    /* ASM_BARRIER: Prevent reuse of the third argument when preparing the fourth zero. */
    __asm__("" : "+r"(third));
    driver->dispatch(driver->draw_ordering_table, second, third, 0);
}

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

int func_800933B4(int *a0) {
    unsigned char *p = &D_800ABAC2;
    if (*p >= 2) {
        D_800ABABC(D_80011F4C, a0);
    }
    func_80093E70((u32 *)(a0 + 7), (RrFrameEnvironment *)a0);
    a0[7] |= 0xFFFFFF;
    ((int (*)())D_800ABAB8[2])(D_800ABAB8[6], a0 + 7, 0x40, 0);
    func_80096444(p + 0xE, a0, 0x5C);
    return (int)a0;
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((char *)D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*)[])&D_800ABAC2)

void func_80093474(int a, W3L2_P *b) {
    char *t;
    if (D_800ABAC2[0] >= 2) D_800ABABC(D_80011F64, a, b);
    t = (char *)b + 0x1C;
    func_80093E70((u32 *)t, (RrFrameEnvironment *)b);
    b->w = (b->w & 0xFF000000) | (a & 0xFFFFFF);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x18), t, 0x40, 0);
    func_80096444(D_800ABAC2 + 0xE, b, 0x5C);
}

#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

int func_8009354C(int a) { func_80096444(a, D_800ABAD0, 0x5C); return a; }
