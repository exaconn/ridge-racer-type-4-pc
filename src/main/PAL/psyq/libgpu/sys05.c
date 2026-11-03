/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/gpu.h"
#include "rr/library.h"
#include "m2c_macros.h"

typedef struct { int w; int s4; } A;

typedef struct { int a, b, c; } B;

extern char D_80011EA8[];
extern char D_80011EB4[];
extern char D_80011EC8[];
extern char D_80011ECC[];
extern char *D_800ABAB8;
extern char D_80011ED8[];
/* extern */
extern M2C_UNK D_80011EE4;
extern char D_80011EF0[];
extern char D_80011EFC[];
extern char D_80011F08[];
extern char D_800ABB6C[];
extern int D_800ABB80;
extern char D_80011F20[];

void func_80092DB8(const char *name,RrGpuRectangle *rectangle);
void func_80092ED4(int a, int b, int c, int d);
void func_80092F64(int a, int b, int c, int d);
void func_80092FFC(s32 arg0, M2C_UNK arg1);
void func_8009305C(int a, int b);
int func_800930BC();
int *func_80093174();
unsigned *func_8009323C(unsigned *a, int b);
void func_800932E8(char *a0);

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

void func_80092ED4(int a, int b, int c, int d)
{
int v; char nv; nv = b & 0xFF; func_80092DB8(D_80011ECC, a); v = (((d & 0xFF) << 16) | ((c & 0xFF) << 8)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}

void func_80092F64(int a, int b, int c, int d)
{
int v; char nv; int t, u, k; nv = b & 0xFF; func_80092DB8(D_80011ED8, a); u = (d & 0xFF) << 16; t = (c & 0xFF) << 8; k = 0x80000000; v = (u | (t | k)) | nv; (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0xC), a, 8, v);
}

void func_80092FFC(s32 arg0, M2C_UNK arg1) {
    func_80092DB8(&D_80011EE4, arg0);
    M2C_FIELD(D_800ABAB8, M2C_UNK (**)(s32, s32, M2C_UNK, M2C_UNK), 8)(M2C_FIELD(D_800ABAB8, s32 *, 0x20), arg0, 8, arg1);
}

void func_8009305C(int a, int b) {
    func_80092DB8(D_80011EF0, a);
    (*(int (**)())(D_800ABAB8 + 8))(*(int *)(D_800ABAB8 + 0x1C), a, 8, b);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABB60 (*(B (*))&D_800ABB60)
#define D_800ABAB8 (*(int *(*))&D_800ABAB8)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)

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

#undef D_800ABB60
#undef D_800ABAB8
#undef D_800ABABC
#undef D_800ABAC2

/* legacy typing of shared globals in this part of the unit */
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

void func_800932E8(char *a0)
{
    int t = a0[3];
    (*(int (**)())(D_800ABAB8 + 0x3C))(0);
    (*(int (**)())(D_800ABAB8 + 0x14))(a0 + 4, t);
}

#undef D_800ABABC
#undef D_800ABAC2
