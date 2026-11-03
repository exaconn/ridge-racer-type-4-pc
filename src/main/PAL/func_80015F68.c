#include "types.h"
#include "rr_asm.h"

extern void func_800153FC(void *, void *);
extern void func_800155D8(void *, void *);
extern void *func_8001577C(void *, s32, void *);
extern void func_80015918(void *, s32, void *);
extern void *func_80015D00(void *, s32, void *);
extern s32 func_80096464(void);

extern s32 D_800AC5D8;
extern void *D_800F3A08[];
extern s8 D_800F4C68;
extern s16 D_800F4D44;
extern u8 D_8010B2E0[];

typedef struct {
    s32 sxy;
    s32 sz;
} VertScreen;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SVector;

typedef struct {
    s16 unk0;
    u16 unk2;
    VertScreen *verts;
} TrailEntry4;

typedef union {
    s32 w[0x70];
    s16 h[0xE0];
    u16 uh[0xE0];
    void *p[0x70];
    char *c[0x70];
    s32 *pw[0x70];
    SVector *sv[0x70];
} Scr;
#define SP ((Scr *)s2)

typedef union {
    s32 w[0x32];
    s16 h[0x64];
    u16 uh[0x64];
} Ent;
#define S4 ((Ent *)s4)

typedef struct {
    s16 unk0;
    s16 unk2;
    SVector *verts;
} TrailEntry8;

#define GTE_STORE_SXY_SZ(p) \
    __asm__ volatile( \
        "swc2 $12, 0(%0); swc2 $17, 4(%0); swc2 $13, 8(%0); swc2 $18, 12(%0); swc2 $14, 16(%0); swc2 $19, 20(%0)" \
        : : "r"(p) : "memory")

#define GTE_SETROT_AND_SAVE(src, dst) \
    __asm__ volatile( \
        "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; sw $12, 0(%1); sw $13, 4(%1); " \
        "lw $12, 8(%0); lw $13, 12(%0); ctc2 $12, $2; ctc2 $13, $3; sw $12, 8(%1); " \
        "lw $12, 16(%0); sw $13, 12(%1); ctc2 $12, $4; sw $12, 16(%1)" \
        : : "r"(src), "r"(dst) : "$12", "$13", "memory")

#define GTE_CTC2_27(val) \
    __asm__ volatile("ctc2 %0, $27" : : "r"(val))

#define GTE_CTC2_28(val) \
    __asm__ volatile("ctc2 %0, $28" : : "r"(val))

/* ASM_BARRIER: empty asm that only reads its operands; keeps both values live to the end of the block
 * so gcc expands the signed divisions with a copy (bgez; move; addiu) as in the original. */
#define KEEP_LIVE2(a, b) __asm__ volatile("" : : "r"(a), "r"(b))
#define GTE_LWC2_IR0(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p))
#define GTE_LWC2_IR0_MEM(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p) : "memory")
#define GTE_DPCS_COL(col, dc) __asm__ volatile("mtc2 %1, $8\n\tmtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010\n\tmfc2 %0, $22" : "+r"(col) : "r"(dc))
#define GTE_MTC2_RGB_DPCS(c) __asm__ volatile("mtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010" : : "r"(c))

void func_80015F68(void *arg0, s32 *arg1, void *arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6) {
    char *s2 = (char *)0x1F80007C;
    char *s4 = (char *)&D_8010B2E0[arg4 * 0x190];
    s32 s0;
    s32 a0;
    TrailEntry8 *a2;
    char *vdst;
    s32 *p;
    char *vsrc;
    s32 vcnt;

    SP->w[0x23] = arg5;
    if (*(s32 *)0x1F800064 != 0) {
        s4 += 0xC8;
        SP->w[0x2F] = 0x80000000;
    } else {
        SP->w[0x2F] = 0;
    }
    SP->p[0x24] = arg0;
    SP->w[0x1B] = *(s32 *)0x1F800004 + 0x200 + arg6 * 4;
    SP->p[0x25] = D_800F3A08[arg4 / 2];

    if (arg1 == 0 || (SP->w[0x23] & 1) == 0) {
        S4->w[0x3] = 0;
        S4->w[0xD] = 0;
        S4->w[0x1A] = 0;
        S4->w[0x17] = 0;
    }

    if (*(u8 *)(SP->w[0x25] + 0xA) == 3) {
        if (D_800F4C68 == 0) {
            s32 v;
            s0 = S4->w[0x18];
            if (SP->w[0x23] & 1) {
                a0 = (*(s16 *)(SP->w[0x24] + 0x1D8) - 0x358) / 4;
                if (s0 < a0) {
                    s0 = a0;
                } else if (a0 < s0) {
                    s0 -= 4;
                }
            } else if (s0 != 0) {
                s0 -= 4;
            }
            if (s0 < 0) goto L_clamp_low;
            if (s0 > 0xF0) goto L_clamp_high;
        L_clamp_low:
            v = s0;
            if (s0 < 0) v = 0;
            goto L_clamp_done;
        L_clamp_high:
            v = 0xF0;
        L_clamp_done:
            s0 = v;
            S4->w[0x18] = s0;
            if (SP->w[0x23] & 1) {
                s0 += func_80096464() & 0xF;
            }
            S4->w[0x2] = s0;
        }
        if (S4->w[0x2] <= 0) {
            return;
        }
    } else {
        s0 = 0x80;
        if ((SP->w[0x23] & 1) == 0) {
            s0 = 0;
            a0 = 0x80;
            S4->w[0x3] = 0;
            S4->w[0xD] = 0;
            S4->w[0x1A] = 0;
            S4->w[0x17] = 0;
        } else {
            a0 = 0xF0;
        }
        if (D_800F4C68 == 0) {
            s32 v1;
            s32 d = a0 - s0;
            if (SP->w[0x23] & 2) {
                v1 = S4->w[0x18] + d / 4;
                if (v1 < a0) {
                    S4->w[0x18] = v1;
                } else {
                    S4->w[0x18] = a0;
                }
                v1 = S4->w[0x19] + a0 / 4;
                if (v1 < a0) {
                    S4->w[0x19] = v1;
                } else {
                    S4->w[0x19] = a0;
                }
            } else {
                v1 = S4->w[0x18] - d / 8;
                if (v1 > s0) {
                    S4->w[0x18] = v1;
                } else {
                    S4->w[0x18] = s0;
                }
                v1 = S4->w[0x19] - a0 / 8;
                if (v1 > 0) {
                    S4->w[0x19] = v1;
                } else {
                    S4->w[0x19] = 0;
                }
            }
            /* ASM_BARRIER: keep a0/d live (see KEEP_LIVE2) */
            KEEP_LIVE2(a0, d);
        }
    }

    if (arg1 == 0) {
        return;
    }

    SP->w[0xE] = arg1[0] - 0x8000;
    SP->w[0xF] = arg1[1];
    SP->w[0x10] = arg1[2] - 0x8000;
    {
        s32 *g = (s32 *)0x1F800000;
        SP->h[0x64] = arg1[0] - g[2];
        SP->h[0x65] = arg1[1] - g[3];
        SP->h[0x66] = arg1[2] - g[4];
    }
    RR_GTE_SETROT((void *)0x1F800028);
    RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
    RR_GTE_SWC2_MAC123(s2 + 0x14);
    GTE_SETROT_AND_SAVE(arg2, s2);

    SP->h[0x65] = 0;
    SP->h[0x64] = 0;
    SP->h[0x66] = -0x1000;

    RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
    RR_GTE_SWC2_MAC123(s2 + 0x98);

    if (*(u8 *)(SP->w[0x25] + 0xA) == 3 && D_800F4D44 == 0) {
        func_800155D8(s4, s2);
        return;
    }

    if (S4->w[0x1C] != 0 && (SP->w[0x23] & 3) == 3) {
        if (S4->w[0x17] == 0) {
            S4->w[0x1D] = SP->w[0xE];
            S4->w[0x1E] = SP->w[0xF];
            S4->w[0x1F] = SP->w[0x10];
            S4->w[0x17] = 1;
        }
    } else if (S4->w[0x17] == 1) {
        S4->w[0x17] = 2;
    }

    if (*(u8 *)(SP->w[0x25] + 0xA) == 2 && (D_800F4D44 == 6 || D_800F4D44 == 0)) {
        if (D_800AC5D8 != 6) {
            func_800153FC(s4, s2);
        }
    }

    RR_GTE_SETFC(0, 0, 0);
    GTE_CTC2_28(0);

    if (SP->w[0x23] & 1) {
        s32 v1;
        RR_GTE_LWC2_RGB((char *)(SP->w[0x25] + 0xC));
        v1 = SP->w[0x28];
        if (v1 >= -0xEC8) {
            char *a0;
            char *a1;
            if (D_800F4C68 != 0) {
                SP->w[0x30] = S4->w[0x2];
            } else {
                s32 a0_val;
                s32 v0_val;
                SP->w[0x31] = v1;
                if (v1 < 0) {
                    SP->w[0x31] = 0;
                } else if (v1 >= 0x1000) {
                    SP->w[0x31] = 0xFFF;
                }
                v0_val = func_80096464();
                v1 = (SP->w[0x28] + 0xEC8) / 2;
                a0_val = v0_val & SP->w[0x31];
                SP->w[0x30] = a0_val;
                if (v1 > 0x200) {
                    v1 = 0x200;
                }
                v0_val = (a0_val >> 2) + v1;
                SP->w[0x30] = v0_val;
                if (v0_val > 0xE00) {
                    SP->w[0x30] = 0xE00;
                }
                v0_val = 0x1000 - SP->w[0x30];
                SP->w[0x30] = v0_val;
                S4->w[0x2] = v0_val;
            }
            RR_GTE_SETTR(s2 + 0x14);
            SP->w[0x1F] = *(s32 *)(S4->w[0] + 0x10);
            a1 = *(char * volatile *)(s2 + 0x7C);
            SP->w[0x22] = *(s32 *)(S4->w[0] + 0x14);
            v1 = *(volatile s32 *)(s2 + 0x88);
            SP->w[0x21] = *(s32 *)(S4->w[0] + 0x18);
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE_SWC2_IR0(s2 + 0xDC);
                RR_GTE14_LWC2_V012(a1);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                a1 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
            if (D_800F4C68 == 0) {
                s32 idx = S4->w[0x4];
                s32 n = S4->w[0x3];
                s32 off = (idx << 3) + 0x14;
                TrailEntry4 *a2 = (TrailEntry4 *)(s4 + off);
                s32 rem = 4 - idx;
                VertScreen *dst;
                VertScreen *src;
                if (n > 0) {
                    do {
                        a2->unk0 += a2->unk0 / 2;
                        if (a2->unk0 > 0x1000) {
                            a2->unk0 = 0x1000;
                        }
                        rem--;
                        a2++;
                        if (rem <= 0) {
                            a2 = (TrailEntry4 *)(s4 + 0x14);
                            rem = 4;
                        }
                        n--;
                    } while (n > 0);
                }
                if (S4->w[0x3] < 4) {
                    S4->w[0x3] += 1;
                }
                S4->w[0x4] = (S4->w[0x4] + 3) % 4;
                a2 = (TrailEntry4 *)(s4 + ((S4->w[0x4] << 3) + 0x14));
                v1 = 0x1000 - SP->w[0x31];
                if (v1 >= 0x200) {
                    a2->unk0 = v1;
                } else {
                    a2->unk0 = 0x200;
                }
                a2->unk2 = SP->uh[0x6E];
                n = SP->w[0x22];
                dst = a2->verts;
                src = (VertScreen *)(s2 + 0x1A0);
                while (n > 0) {
                    s32 x = src->sxy;
                    s32 y = src->sz;
                    src++;
                    n--;
                    dst->sxy = x;
                    dst->sz = y;
                    dst++;
                }
            }
            if (S4->w[0x3] >= 2) {
                s32 *out = (s32 *)(s2 + 0x160);
                s32 idx = S4->w[0x4];
                s32 s3 = S4->w[0x3] - 1;
                s32 off = (idx << 3) + 0x14;
                TrailEntry4 *s0 = (TrailEntry4 *)(s4 + off);
                s32 s1 = 4 - idx;
                while (s3 > 0) {
                    s32 unk0 = s0->unk0;
                    RR_GTE_MTC2_IR0(unk0);
                    RR_GTE14_NOP2();
                    RR_GTE_OP(0x4A780010);
                    RR_GTE_SWC2_RGB2(out);
                    out++;
                    s1--;
                    s0++;
                    if (s1 <= 0) {
                        s0 = (TrailEntry4 *)(s4 + 0x14);
                        s1 = 4;
                    }
                    s3--;
                }
                *out = 0;
                SP->p[0x35] = s2 + 0x1A0;
                {
                s32 i2 = S4->w[0x4];
                SP->p[0x36] = s2 + 0x168;
                s1 = 4 - i2;
                off = (i2 << 3) + 0x14;
                }
                s3 = S4->w[0x3] - 1;
                s0 = (TrailEntry4 *)(s4 + off);
                while (s3 > 0) {
                    p = SP->pw[0x21];
                    SP->p[0x34] = SP->p[0x35];
                    SP->p[0x35] = s0->verts;
                    func_80015918(p + 1, *(volatile s32 *)p >> 16, s2);
                    SP->w[0x58] = SP->w[0x59];
                    s1--;
                    SP->w[0x59] = *SP->pw[0x36]++;
                    s0++;
                    if (s1 <= 0) {
                        s0 = (TrailEntry4 *)(s4 + 0x14);
                        s1 = 4;
                    }
                    s3--;
                }
            }
            {
                char *v0 = *(char **)(S4->w[0] + 0x1C);
                v1 = *(s32 *)(S4->w[0] + 0x20);
                a0 = s2 + 0x188;
                while (v1 > 0) {
                    RR_GTE14_LWC2_V012(v0);
                    RR_GTE14_NOP2();
                    RR_GTE14_RTPT();
                    v0 += 0x18;
                    a0 += 0x18;
                    GTE_STORE_SXY_SZ(a0);
                    v1 -= 3;
                }
            }
            SP->w[0x58] = 0x2E808080;
            p = *(s32 **)(S4->w[0] + 0x24);
            func_8001577C(p + 1, *(volatile s32 *)p >> 16, s2);
            GTE_LWC2_IR0(s2 + 0xC0);
            RR_GTE14_NOP2();
            RR_GTE_OP(0x4A780010);
            RR_GTE_SWC2_RGB2(s2 + 0x160);
            SP->w[0x59] = 0;
            SP->w[0x58] = (SP->w[0x58] & 0xFFFFFF) | 0x32000000;
            p = *(s32 **)(S4->w[0] + 0xC);
            func_80015D00(p + 1, *(volatile s32 *)p >> 16, s2);
        } else {
            S4->w[0x3] = 0;
        }
    }

    if (SP->w[0x28] >= 0x7D0) {
        s32 v1 = S4->w[0x17];
        S4->w[0xD] = 0;
        if (v1 == 0) {
            return;
        }
        if (v1 == 1) {
            S4->w[0x17] = 2;
        }
    } else {
        s32 v1;
        char *a0;
        char *a1;
        RR_GTE_SETROT(s2);
        RR_GTE_SETTR(s2 + 0x14);
        GTE_CTC2_27(0x88);
        SP->w[0x1F] = *(s32 *)(S4->w[0] + 0x34);
        a1 = *(char * volatile *)(s2 + 0x7C);
        SP->w[0x22] = *(s32 *)(S4->w[0] + 0x38);
        v1 = *(volatile s32 *)(s2 + 0x88);
        SP->w[0x21] = *(s32 *)(S4->w[0] + 0x3C);
        a0 = s2 + 0x188;
        while (v1 > 0) {
            RR_GTE_SWC2_IR0(s2 + 0xDC);
            RR_GTE14_LWC2_V012(a1);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();
            a1 += 0x18;
            a0 += 0x18;
            GTE_STORE_SXY_SZ(a0);
            v1 -= 3;
        }
        if (D_800F4C68 == 0) {
            s32 idx;
            TrailEntry4 *entry;
            s32 n;
            VertScreen *dst;
            VertScreen *src;
            if (S4->w[0xD] < 4) {
                S4->w[0xD] += 1;
            }
            idx = (S4->w[0xE] + 3) % 4;
            S4->w[0xE] = idx;
            entry = (TrailEntry4 *)(s4 + ((idx << 3) + 0x3C));
            entry->unk0 = S4->h[0x30];
            n = SP->w[0x22];
            dst = entry->verts;
            src = (VertScreen *)(s2 + 0x1A0);
            while (n > 0) {
                s32 x = src->sxy;
                s32 y = src->sz;
                src++;
                n--;
                dst->sxy = x;
                dst->sz = y;
                dst++;
            }
        }
        if (S4->w[0xD] >= 2) {
            s32 cnt = S4->w[0xD];
            s32 col = S4->w[0x18];
            s32 s0;
            s32 *out;
            s32 s1;
            GTE_DPCS_COL(col, SP->w[0x37]);
            out = (s32 *)(s2 + 0x160);
            s0 = cnt - 1;
            while (s0 > 0) {
                if (col > 0) {
                    *out = col;
                } else {
                    *out = 0;
                }
                col -= col / 4;
                s0--;
                out++;
            }
            *out = 0;
            SP->p[0x35] = s2 + 0x1A0;
            SP->p[0x36] = s2 + 0x168;
            s0 = S4->w[0xD] - 1;
            s1 = S4->w[0xE];
            while (s0 > 0) {
                SP->p[0x34] = SP->p[0x35];
                s1 = (s1 + 1) % 4;
                p = SP->pw[0x21];
                SP->p[0x35] = ((TrailEntry4 *)(s4 + (s1 << 3) + 0x3C))->verts;
                func_80015918(p + 1, *(volatile s32 *)p >> 16, s2);
                if (SP->w[0x59] <= 0) {
                    break;
                }
                do {
                    SP->w[0x58] = SP->w[0x59];
                } while (0);
                {
                    s32 *q;
                    q = SP->pw[0x36];
                    s0--;
                    col = (s32) q;
                    q = (s32 *)*q;
                    col += 4;
                    SP->pw[0x36] = (s32 *)col;
                    SP->w[0x59] = (s32)q;
                }
            }
        }
        {
            char *v0 = *(char **)(S4->w[0] + 0x40);
            v1 = *(s32 *)(S4->w[0] + 0x44);
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE14_LWC2_V012(v0);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                v0 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
        }
        {
            s32 c = S4->w[0x18];
            SP->w[0x58] = 0x2E000000 | (c << 16) | (c << 8) | c;
            p = *(s32 **)(S4->w[0] + 0x48);
            func_8001577C(p + 1, *(volatile s32 *)p >> 16, s2);
        }
        {
            char *v0 = *(char **)(S4->w[0] + 0x28);
            v1 = *(s32 *)(S4->w[0] + 0x2C);
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE14_LWC2_V012(v0);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                v0 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
        }
        {
            s32 c = S4->w[0x19];
            SP->w[0x58] = 0x2E000000 | (c << 16) | (c << 8) | c;
            p = *(s32 **)(S4->w[0] + 0x30);
            func_8001577C(p + 1, *(volatile s32 *)p >> 16, s2);
        }
    }

    GTE_CTC2_27(0x88);
    if (S4->w[0x17] == 0) {
        return;
    }

    SP->w[0x1F] = *(s32 *)(S4->w[0] + 0x34);
    SP->w[0x22] = *(s32 *)(S4->w[0] + 0x38);
    SP->w[0x21] = *(s32 *)(S4->w[0] + 0x3C);
    SP->p[0x2A] = ((TrailEntry8 *)(s4 + (S4->w[0x1B] << 3) + 0x84))->verts;

    if (S4->w[0x17] == 1) {
        RR_GTE_SETROT(arg3);
        SP->w[0x2B] = SP->w[0xE] - S4->w[0x1D];
        SP->w[0x2C] = SP->w[0xF] - S4->w[0x1E];
        SP->w[0x2D] = SP->w[0x10] - S4->w[0x1F];
        RR_GTE_SETTR(s2 + 0xAC);
        vdst = (SP->c[0x2A] = s2 + 0xE0);
        vsrc = SP->c[0x1F];
        vcnt = SP->w[0x22];
        while (vcnt > 0) {
            RR_GTE_MVMVA_ROT_TR_V0(vsrc);
            RR_GTE_STORE_IR_S16(vdst);
            vsrc += 8;
            vcnt--;
            vdst += 8;
        }
        if (D_800F4C68 == 0) {
            S4->w[0x1A] += 1;
            if (S4->w[0x1A] == 8) {
                S4->w[0x17] = 2;
            }
            S4->w[0x1B] = (S4->w[0x1B] + 7) % 8;
            a2 = (TrailEntry8 *)(s4 + ((S4->w[0x1B] << 3) + 0x84));
            vdst = (char *)a2->verts;
            a2->unk0 = S4->h[0x30];
            vcnt = SP->w[0x22];
            vsrc = SP->c[0x2A];
            while (vcnt > 0) {
                *(SVector *)vdst = *(SVector *)vsrc;
                vsrc += 8;
                vcnt--;
                vdst += 8;
            }
        }
    }

    if (S4->w[0x1A] != 0) {
        /* ASM_BARRIER: pin the fade counter to $a1 (original register choice) */
        register s32 nleft __asm__("$5");
        s32 dec8;
        s32 i8;
        s32 s5;
        nleft = S4->w[0x1A];
        dec8 = (S4->w[0x17] == 2) ? 8 : 4;
        i8 = S4->w[0x1B];
        while (nleft > 0) {
            a2 = (TrailEntry8 *)(s4 + ((i8 << 3) + 0x84));
            i8 = (i8 + 1) % 8;
            s5 = a2->unk0;
            s5 -= dec8;
            if (s5 < 0) {
                s5 = 0;
            }
            nleft--;
            a2->unk0 = s5;
        }
        if (s5 == 0) {
            S4->w[0x1A] -= 1;
        }
        if (S4->w[0x1A] == 0) {
            S4->w[0x17] = 0;
            return;
        }
        if (S4->w[0x1A] >= 2) {
            /* ASM_BARRIER: pin the RTPT vertex counter to $v1 (original register choice) */
            register s32 nvert __asm__("$3");
            s32 s0;
            s32 s1;
            RR_GTE_SETROT((void *)0x1F800028);
            {
                char *t = s2 + 0x1A0;
                s32 *g;
                s32 m;
                do {
                    m = -0x8000;
                } while (0);
                g = (s32 *)0x1F800000;
                SP->p[0x34] = t;
                SP->h[0x64] = S4->w[0x1D] + m - g[2];
                SP->h[0x65] = S4->w[0x1E] - g[3];
                SP->h[0x66] = S4->w[0x1F] + m - g[4];
            }
            RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
            RR_GTE_SWC2_MAC123(s2 + 0x38);
            RR_GTE_SETTR_AT20(s2 + 0x24);
            vsrc = SP->c[0x2A];
            vdst = SP->c[0x34] - 0x18;
            nvert = SP->w[0x22];
            while (nvert > 0) {
                RR_GTE14_LWC2_V012(vsrc);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                vsrc += 0x18;
                vdst += 0x18;
                GTE_STORE_SXY_SZ(vdst);
                nvert -= 3;
            }
            SP->c[0x35] = vdst + 0x18;
            s0 = S4->w[0x1B];
            SP->w[0x58] = 0;
            s1 = S4->w[0x1A] - 1;
            while (s1 > 0) {
                s32 off;
                void *d0;
                void *d4;
                s0 = (s0 + 1) % 8;
                off = (s0 << 3) + 0x84;
                a2 = (TrailEntry8 *)(s4 + off);
                vsrc = (char *)a2->verts;
                vdst = SP->c[0x35] - 0x18;
                nvert = SP->w[0x22];
                while (nvert > 0) {
                    RR_GTE_SWC2_IR0(s2 + 0xDC);
                    RR_GTE14_LWC2_V012(vsrc);
                    RR_GTE14_NOP2();
                    RR_GTE14_RTPT();
                    vsrc += 0x18;
                    vdst += 0x18;
                    GTE_STORE_SXY_SZ(vdst);
                    nvert -= 3;
                }
                if (s1 == 1) {
                    SP->w[0x59] = 0;
                } else {
                    u16 c;
                    GTE_LWC2_IR0_MEM(s2 + 0xDC);
                    c = a2->unk0;
                    GTE_MTC2_RGB_DPCS(c);
                    RR_GTE_SWC2_RGB2(s2 + 0x164);
                }
                p = SP->pw[0x21];
                func_80015918(p + 1, *(volatile s32 *)p >> 16, s2);
                do {
                    d4 = SP->p[0x35];
                    d0 = SP->p[0x34];
                } while (0);
                nvert = SP->w[0x59];
                SP->p[0x34] = d4;
                SP->p[0x35] = d0;
                if (nvert <= 0) {
                    break;
                }
                s1--;
                SP->w[0x58] = nvert;
            }
        }
    }
}
