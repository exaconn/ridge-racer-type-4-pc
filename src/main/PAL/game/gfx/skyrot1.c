#include "types.h"
#include "rr_asm.h"
#include "m2c_macros.h"
#include "game/gfx/sky.h"
#include "psyq/libgte/gtemat2.h"
#include "game/gfx/skyrot1.h"

struct V8 { unsigned char c[8]; };

struct M { int a[8]; };

extern int func_800701C4();
extern int func_80091844();
extern int func_8006EE3C();
extern int func_8005F0D0();
extern char D_8001013C[];
extern int D_800F2FD8;
extern struct V8 D_800101E4;
extern struct { char pad[0xA8]; unsigned short v; } *D_8010BF78;
extern int func_80091FF4();
extern int func_80092194();
extern int func_80092334();
extern char D_8009E88C[];
extern int func_80092CB8();
extern s16 D_800AC5D0;
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

int func_8001BD40(int a0, int a1);
void func_8001BEF8(void);
void func_8001BFA8(int a0, int a1, int a2);
void func_8001C0F8(void);

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF78 ((char *)D_8010BF78)

int func_8001BD40(int a0, int a1)
{
    int t[8];
    short *s3 = (short *)0x1F80007C;
    int s2;
    func_800701C4(4);
    s2 = *(short *)(D_8010BF78 + (a0 << 1) + 0xAA);
    if (a1 || (s2 && (s2 & 0x3FF) > 0x20)) s2 += 0x20;
    else if (s2) s2 = 0;
    s2 &= 0xFFF;
    *(short *)(D_8010BF78 + (a0 << 1) + 0xAA) = s2;
    s3[0] = 0;
    s3[2] = 0;
    s3[1] = -s2;
    func_80091844(s3, (char *)s3 + 8);
    RR_GTE_SETROT(D_8010BF78);
    func_80090A24((char *)s3 + 8, (char *)s3 + 0x28);
    RR_GTE_SETLLM((char *)s3 + 0x28);
    RR_GTE_SETROT(0x1F800028);
    func_80090A24((char *)s3 + 8, t);
    func_8006EE3C(0x1F80007C, D_8001013C + a0 * 16, t);
    *(int *)0x1F800078 = 0;
    func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
    return s2;
}

#undef D_8010BF78

void func_8001BEF8(void)
{
    struct V8 sp10;
    struct M sp18;
    int t;
    sp10 = D_800101E4;
    D_8010BF78->v += 0x20;
    sp18.a[4] = 0x1000;
    sp18.a[2] = 0x1000;
    sp18.a[3] = 0;
    sp18.a[1] = 0;
    t = (short)D_8010BF78->v;
    sp18.a[0] = 0x1000;
    func_80092194(t, &sp18);
    func_80091FF4(0x100, &sp18);
    func_80092334(0xAA, &sp18);
    func_800197C8(D_8009E88C, &sp18, &sp10, 0xC8);
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF78 ((char *)D_8010BF78)

void func_8001BFA8(int a0, int a1, int a2)
{
    int t[8];
    func_800701C4(4);
    func_80091844(a1, 0x1F800084);
    RR_GTE_SETROT(D_8010BF78 + 0x20);
    func_80090A24(0x1F800084, 0x1F8000A4);
    RR_GTE_SETLLM(0x1F8000A4);
    RR_GTE_SETROT(0x1F800028);
    func_80090A24(0x1F800084, t);
    func_8006EE3C(0x1F80007C, a2, t);
    *(int *)0x1F800078 = 0;
    func_8005F0D0(0x1F800000, a0 < D_800F2FD8 ? a0 : 1, 0, 0);
}

#undef D_8010BF78

void func_8001C0F8(void) {
    D_800AC5DC = 0x80;
    D_800AC5D0 = 0;
    D_8010BF7C = 0;
    func_80092CB8(1);
    D_800F4C6A = 0;
    D_800F3A26 = 2;
    D_800F4D8A = 0;
}
