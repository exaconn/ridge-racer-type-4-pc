/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "rr/layout.h"

#define VQ (*(struct S2 * volatile *)&a0->q)

struct S2 { char p0[2]; unsigned char b2, b3, b4, b5, b6, b7; };

struct S {
    char p0[0x3C];
    struct S2 *q;
    char p1[6];
    unsigned char st, cnt;
    char p2[1];
    unsigned char f49;
    char p3[0x99];
    unsigned char c3, c4;
    unsigned short s6;
    char p4[1];
    unsigned char c9, ca, cb;
    unsigned short sc, se;
};

struct S_1 { char pad[0xE3]; unsigned char a; char pad1[5]; unsigned char b; char pad2[2]; unsigned short c; };

typedef struct { int f0; int f4; int f8; char p0[8]; int f14; int f18; char p1[0x2A]; char f46; char f47; char p2[1]; char f49; char p3[0x99]; unsigned char fE3; char p4[5]; unsigned char fE9; } O;

struct S_2 { char pad[0x46]; unsigned char c46, c47, c48; };

struct E4 {
    unsigned char b0, b1, b2, b3, b4;
};

struct E8 {
    unsigned char b0;
    char pad[3];
    unsigned char *p4;
};

struct S_3 {
    unsigned short *f0;
    struct E4 *f4;
    struct E8 *f8;
    char p0[0x30];
    struct S2 *q;
    char p1[6];
    unsigned char st, cnt, c48, f49;
    char p3[0x99];
    unsigned char c3, c4;
    unsigned short s6;
    char p4[1];
    unsigned char c9, ca, cb;
    unsigned short sc, se;
};

extern void (*D_800ABC6C)(struct S *);
extern int (*D_800ABC84)();
extern int func_8009964C(), func_8009966C(), func_8009968C(), func_800996AC();
extern unsigned char *D_80114438;
/* Partial layout: six input slot IDs, per-group limits, six output IDs. */
typedef struct {
    char pad0[4];
    u8 *group_data;
    char pad8[0x20 - 8];
    u8 *slot_groups;
    char pad24[0x46 - 0x24];
    u8 status;
    char pad47[0x5D - 0x47];
    u8 assignments[6];
    char pad63[0xE9 - 0x63];
    u8 group_count;
} RrSlotAssignmentState;
RR_ASSERT_SIZE(RrSlotAssignmentState, 0xEC);
RR_ASSERT_OFFSET(RrSlotAssignmentState, group_data, 4);

int func_80098B64();
int func_80098DD4();
int func_80098E0C();
void func_80098EE0();
int func_80098F88(struct S_3 *s);
int func_80099380();
void func_800993E8(void *arg0);
s32 func_80099404(RrSlotAssignmentState *state);

int func_80098B64(a0) struct S *a0; {
    struct S2 *q;
    int t, n;
    /* ASM_BARRIER: pin b4 to $v1 so the load/add chain takes the original register allocation */
    register int b __asm__("$3");
    switch (a0->st) {
    case 2:
        q = a0->q;
        if (q->b7 != 0) goto r0;
        if (a0->c3 == q->b3 && a0->c4 == q->b4 && a0->c9 == q->b5 && a0->ca == q->b6) {
            a0->se = 0;
        } else {
            a0->se = 0xFFFF;
        }
        a0->c3 = VQ->b3;
        a0->c4 = VQ->b4;
        a0->s6 = 0;
        a0->c9 = VQ->b5;
        a0->ca = VQ->b6;
        a0->sc = 0;
        if (a0->se != 0) goto r0;
        a0->cb = 0;
        goto r1;
    case 3:
        if (a0->q->b2 != 0) goto r0;
        if (a0->q->b3 != 0) goto r0;
        t = a0->q->b5 + (a0->q->b4 << 8);
        a0->s6 = t;
        if (a0->se == (t & 0xFFFF)) goto eq;
        a0->se = t;
    r0:
        return 0;
    eq:
        a0->se = 0xFFFF;
        a0->cb = 0;
        a0->cnt = 0;
        goto r1;
    case 4:
        if (a0->q->b2 != 0) goto r0;
        if (a0->q->b3 != 0) goto r0;
        t = a0->sc;
        b = a0->q->b4;
        t += 8;
        t += (b + 3) & 0x1FC;
        n = a0->cnt + 1;
        a0->cnt = n;
        a0->sc = t;
        if ((unsigned char)n < a0->ca) goto r0;
        if (func_80098DD4(a0) >= 0x81) {
            D_800ABC6C(a0);
            a0->st = 0xFE;
            a0->f49 = 2;
            goto r0;
        }
        if (a0->se != a0->sc) {
            a0->se = a0->sc;
            a0->cnt = 0;
            a0->sc = 0;
            return 0;
        }
        a0->se = 0;
        a0->cb = 0;
        a0->st = 0xFF;
        func_80098E0C(a0, (char *)a0 + 0x63);
        a0->st = 2;
        goto r0;
    default:
        return 1;
    }
r1:
    return 1;
}

int func_80098DD4(s) struct S_1 *s; {
    int a = s->a, b = s->b, c = s->c;
    int x = ((a + 1) >> 1) * 4;
    int y = ((b * 5 + 3) & 0xFFC) + 4;
    return x + y + c;
}

int func_80098E0C(o, a1) O *o; int a1; {
    int r;
    if (a1 != 0) {
        if (o->f4 != 0) return 0;
        if (D_800ABC84() != 0) return 0;
    } else return 0;
    /* ASM_BARRIER: return value 1 must be a separate li (not CSE'd with the constant 1 used by the stores/shift) */
    asm volatile("" : "=r"(r) : "0"(1));
    o->f49 = 4;
    o->f0 = (a1 + 3) >> 2 << 2;
    o->f4 = o->f0 + ((o->fE3 + 1) >> 1 << 2); o->f8 = o->f4 + ((o->fE9 * 5 + 3) & 0xFFC);
    o->f46 = 1;
    o->f14 = (int)func_80098EE0;
    o->f47 = 0;
    o->f18 = (int)func_80098F88;
    return r;
}

void func_80098EE0(s) struct S_2 *s; {
    switch (s->c46) {
    case 2: func_8009964C(s, s->c47); break;
    case 3: func_8009966C(s, s->c47); break;
    case 4:
        if (s->c48 == 0) func_8009968C(s, s->c47);
        else func_800996AC(s);
        break;
    }
}

int func_80098F88(struct S_3 *s) {
    struct S2 *q;
    struct E4 *p5;
    struct E8 *e8;
    unsigned char *src;
    int i;
    int t;

    switch (s->st) {
    case 2:
        if (s->q->b2 != 0) return 0;
        if (s->q->b3 != 0) return 0;
        s->f0[s->cnt] = s->q->b5 + (s->q->b4 << 8);
        if (s->se != s->f0[s->cnt]) {
            s->se = s->f0[s->cnt];
            return 0;
        }
        s->se = 0;
        s->cb = 0;
        if (++s->cnt < s->c3) return 0;
        s->cnt = 0;
        break;

    case 3:
        q = s->q;
        if (q->b2 != 0) return 0;
        if (q->b3 != 0) return 0;
        p5 = &s->f4[s->cnt];
        if (p5->b0 == q->b4 && p5->b1 == (*(volatile unsigned char *)&q->b5 & 0x7F) && p5->b2 == q->b6 && p5->b3 == q->b7 && p5->b4 == ((t = q->b5) >> 7)) {
            s->se = 0;
        } else {
            s->se = 0xFFFF;
        }
        p5->b0 = s->q->b4;
        p5->b1 = s->q->b5 & 0x7F;
        p5->b2 = s->q->b6;
        p5->b3 = s->q->b7;
        t = s->q->b5;
        p5->b4 = t >> 7;
        if (s->se != 0) return 0;
        s->cb = 0;
        if (++s->cnt < s->c9) return 0;
        s->cnt = 0;
        s->c48 = 0;
        break;

    case 4:
        q = s->q;
        if (q->b2 != 0) {
            s->c48 = 0;
            return 0;
        }
        e8 = &s->f8[s->cnt];
        if (s->c48 == 0) {
            i = 3;
            e8->b0 = s->c48 = q->b4;
            src = &s->q->b5;
            if (s->cnt == 0) {
                D_80114438 = e8->p4 = (unsigned char *)(s->f8 + s->ca);
            } else {
                D_80114438 = e8->p4 = e8[-1].p4 + ((e8[-1].b0 + 3) & 0x1FC);
            }
        } else {
            i = 5;
            src = &q->b3;
        }
        while (i-- != 0) {
            if (s->c48 == 0) break;
            if (D_80114438 >= &s->c3) {
                s->cnt = 0;
                s->c48 = 0;
                return 0;
            }
            if (*D_80114438 != *src) {
                s->se = 0xFFFF;
            }
            {
                unsigned char **pp = &D_80114438;
                unsigned char *p = *pp;
                *p = *src++;
                *pp = p + 1;
            }
            s->c48--;
        }
        if (s->c48 == 0) {
            if (s->se != 0) {
                s->se = 0;
                s->c48 = 0;
                return 0;
            }
            if (++s->cnt >= s->ca) {
                s->f49 = 6;
                s->st = 0xFE;
                s->cb = 0;
                return 0;
            }
            s->c48 = 0;
            s->cb = 0;
            return 0;
        }
        return 0;

    default:
        return 1;
    }
    return 1;
}

int func_80099380(a0, a1)
char *a0; int a1;
{
    int r;
    if (D_800ABC84(a0, a1) == 0) {
        /* ASM_BARRIER: keep separate li 1 for return value (original has two li 1) */
        __asm__ volatile("" : "=r"(r) : "0"(1));
        a0[0x46] = 1;
        *(int *)(a0 + 0x14) = (int)func_800993E8;
        *(int *)(a0 + 0x20) = a1;
        *(int *)(a0 + 0x18) = (int)func_80099404;
        return r;
    }
    return 0;
}

void func_800993E8(void *arg0) {
    M2C_FIELD(arg0, s8 *, 0x37) = 0x4D;
    M2C_FIELD(arg0, s8 *, 0x36) = 6;
    M2C_FIELD(arg0, s32 *, 0x2C) = (s32) M2C_FIELD(arg0, s32 *, 0x20);
}

s32 func_80099404(RrSlotAssignmentState *state)
{
    s32 group, minimum, remaining, i;
    u8 *slots;

    /* Groups with fewer slots than their minimum are marked unassigned. */
    group = 0;
    if (state->group_count != 0) {
        do {
            slots = state->slot_groups;
            remaining = 0;
            for (i = 5; i >= 0; i--) {
                if (*slots++ == group) remaining++;
            }
            minimum = state->group_data[group * 5 + 2];
            slots = state->slot_groups;
            if (minimum == 0) minimum = 1;
            i = 0;
            do {
                if (*slots++ == group) {
                    if (remaining < minimum) {
                        state->assignments[i] = 0xFF;
                        remaining--;
                    } else {
                        state->assignments[i] = group;
                    }
                }
                i++;
            } while (i < 6);
            group++;
        } while (group < state->group_count);
    }
    state->status = 0xFE;
    return 0;
}
