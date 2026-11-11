/* RODATA 0x80011a88 0x14 */
#include "types.h"
#include "rr/cd.h"
#include "rr/cd_poll.h"
#include "rr/library.h"
#include "game/data/globals.h"
#include "psyq/libcd/s_3.h"

#define COPY8(dst_arr, src_arr) \
    { \
        u8 *d = (dst_arr); \
        u8 *s = (src_arr); \
        s32 n; \
        if (d != 0) { \
            for (n = 7; n != -1; n--) { \
                *d++ = *s++; \
            } \
        } \
    }

extern const char DiskErrorPrefixStr[];
extern const char StrCdComCodeFmt[];
extern const char CdUnknownIntrMsg[];
extern const char CD_fmt_intr_code[];
extern s32 CD_comHasCompleteInt[];
extern s32 D_800A664C[];
extern volatile u8 *CD_REG1;
extern u8 CD_syncResult[];
extern u8 CdReadyResult[];
extern u8 D_80110328[];
extern s32 CheckCallback(void);
extern const char StrCdSync[];
extern const char CdReadyNameStr[];

void CdDataCallback(a0);
unsigned char *CdIntToPos(s32 i, u8 *p);
s32 CdPosToInt(void *arg0);
s32 CD_sync(s32 mode, u8 *result);
s32 CD_ready(s32 mode, u8 *result);

void CdDataCallback(a0) { DMACallback(3, a0); }

void CdDataSync() { ((void (*)(void))CD_datasync)(); }

unsigned char *CdIntToPos(i, p)
int i;
unsigned char *p;
{
  int a;
  int b;
  int c;
  int new_var;
  i += 150;
  a = i / 75;
  b = i % 75;
  new_var = a / 60;
  c = a % 60;
  p[2] = ((b / 10) * 16) + (b % 10);
  p[1] = ((c / 10) * 16) + (c % 10);
  p[0] = ((new_var / 10) * 16) + (new_var % 10);
  return p;
}

s32 CdPosToInt(void *arg0) {
    u8 temp_a1;
    u8 temp_a2;
    u8 temp_v1;

    temp_v1 = ((u8 *)arg0)[0];
    temp_a2 = ((u8 *)arg0)[1];
    temp_a1 = ((u8 *)arg0)[2];
    return (((((((temp_v1 >> 4) * 0xA) + (temp_v1 & 0xF)) * 0x3C) + (((temp_a2 >> 4) * 0xA) + (temp_a2 & 0xF))) * 0x4B) + (((temp_a1 >> 4) * 0xA) + (temp_a1 & 0xF))) - 0x96;
}

s32 getintr(void) {
    volatile u8 intr;
    u8 res[8];
    s32 i;
    s32 j;
    s32 err;

    *CD_reg0 = 1;
    err = 0;
    if ((intr = *CD_reg3 & 7) == 0) {
        return 0;
    }
    while (intr != (*CD_reg3 & 7)) {
        intr = *CD_reg3 & 7;
    }
    for (i = 0; i < 8; i++) {
        if ((*CD_reg0 & 0x20) == 0) {
            break;
        }
        res[i] = *CD_REG1;
    }
    for (j = i; j < 8; j++) {
        res[j] = 0;
    }
    *CD_reg0 = 1;
    *CD_reg3 = 7;
    *CD_REG2 = 7;
    {
        s32 t = intr;
        if (t != 3 || D_800A664C[CD_com] != 0) {
            if ((CD_status.word & 0x10) == 0 && (res[0] & 0x10) != 0) {
                CD_nopen++;
            }
            CD_status.word = *(volatile u8 *)res;
            err = CD_status.word & 0x1D;
            CD_status1 = res[1];
        }
    }
    {
        s32 t = intr;
        if (t == 5) {
            if (CD_debug > 0) {
                DebugPrintStub(DiskErrorPrefixStr);
            }
            if (CD_debug > 0) {
                DebugPrintStub(StrCdComCodeFmt, CD_comstr[CD_com], CD_status.word, CD_status1);
            }
        }
    }
    switch (intr) {
    case 3:
        if (err) {
            CD_intr_status[0] = 5;
            COPY8(CD_syncResult, res);
            return 2;
        }
        if (CD_comHasCompleteInt[CD_com]) {
            CD_intr_status[0] = 3;
            COPY8(CD_syncResult, res);
            return 1;
        }
        CD_intr_status[0] = 2;
        COPY8(CD_syncResult, res);
        return 2;
    case 2:
        CD_intr_status[0] = err ? 5 : 2;
        COPY8(CD_syncResult, res);
        return 2;
    case 1:
        if (err && i == 1) {
            err = 0;
        }
        {
            u8 v = err ? 5 : 1;
            volatile u8 *p = CD_intr_status;
            p[1] = v;
        }
        COPY8(CdReadyResult, res);
        *CD_reg0 = 0;
        *CD_reg3 = 0;
        return 4;
    case 4:
        {
            volatile u8 *p = CD_intr_status;
            p[1] = p[2] = 4;
        }
        COPY8(D_80110328, res);
        COPY8(CdReadyResult, res);
        return 4;
    case 5:
        CD_intr_status[0] = CD_intr_status[1] = 5;
        COPY8(CD_syncResult, res);
        COPY8(CdReadyResult, res);
        return 6;
    default:
        PsyqPuts(CdUnknownIntrMsg);
        DebugPrintStub(CD_fmt_intr_code, intr);
        return 0;
    }
}

static inline int check_alarm(const char **ac, const char **sc, volatile u8 *st) {
    if (VSync(-1) > CD_timeout_deadline || CD_timeout_count++ > 0x3C0000) {
        PsyqPuts(CdTimeoutPrefixStr);
        {
            u8 s0_ = st[0];
            u8 s1_ = st[1];
            const char *x5 = sc[s1_];
            DebugPrintStub(
                CdTimeoutStatusFmt,
                CdWaitCallerName[0],
                ac[CD_com],
                sc[s0_],
                x5
            );
        }
        CD_flush();
        return -1;
    }
    return 0;
}

static inline void cd_poll_cb(volatile u8 *st, u8 *st1) {
    u8 saved = *CD_reg0 & 3;
    s32 res;
    while ((res = getintr()) != 0) {
        if ((res & 4) && CD_cbready != 0) {
            CD_cbready(*st1, CdReadyResult);
        }
        if ((res & 2) && CD_cbsync != 0) {
            CD_cbsync(*st, CD_syncResult);
        }
    }
    *CD_reg0 = saved;
}

s32 CD_sync(s32 mode, u8 *result) {
    s32 ret;
    u8 *src;
    u8 *dst;
    s32 i;
    const char **ac;
    const char **sc;
    volatile u8 *st;
    u8 *st1;

    s32 t = VSync(-1);
    CD_timeout_deadline = t + 0x3C0;
    CD_timeout_count = 0;
    CdWaitCallerName[0] = StrCdSync;
    ac = CD_comstr;
    sc = CD_intstr;
    st = CD_intr_status;
    st1 = st + 1;
    do {
        if (check_alarm(ac, sc, st) != 0) {
            return -1;
        }
        if (CheckCallback() != 0) {
            cd_poll_cb(st, st1);
        }
        ret = *st;
        if (ret == 2 || ret == 5) {
            *st = 2;
            dst = result;
            src = CD_syncResult;
            if (dst != 0) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ret;
        }
    } while (mode == 0);
    return 0;
}

s32 CD_ready(s32 mode, u8 *result) {
    s32 ret;
    u8 *src;
    u8 *dst;
    s32 i;
    const char **ac;
    const char **sc;
    volatile u8 *st, *st2;
    u8 *st1;

    CD_timeout_deadline = VSync(-1) + 0x3C0;
    CD_timeout_count = 0;
    CdWaitCallerName[0] = CdReadyNameStr;
    ac = CD_comstr;
    sc = CD_intstr;
    st = CD_intr_status;
    st1 = st + 1;
    st2 = st + 2;
    do {
        if (check_alarm(ac, sc, st) != 0) {
            return -1;
        }
        if (CheckCallback() != 0) {
            cd_poll_cb(st, st1);
        }
        ret = *st2;
        if (ret != 0) {
            st[2] = 0;
            src = D_80110328;
            if (result != 0) {
                dst = result;
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ret;
        }
        ret = st2[-1];
        if (ret != 0) {
            st[1] = 0;
            dst = result;
            src = CdReadyResult;
            if (dst != 0) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ret;
        }
    } while (mode == 0);
    return 0;
}
