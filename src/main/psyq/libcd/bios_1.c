#include "types.h"
#include "rr/cd.h"
#include "rr/cd_poll.h"
#include "rr/library.h"
#include "rr/spu.h"
#include "game/data/globals.h"
#include "psyq/libcd/bios_1.h"
#include "psyq/libcd/s_3.h"

extern s32 CheckCallback(void);
extern const char D_80011AB0[];
extern const char StrCdNoParamFmt[];
extern const char CdCwLabel[];
extern s32 CdCommandParamTable[];
extern s32 D_800A66CC[];
extern volatile u8 *CD_REG1;
extern u8 CD_syncResult[];
extern u8 CdReadyResult[];
extern SpuRegisterBank *CdSpuRegBase;

s32 CD_cw(u8 arg0, const u8 *arg1, u8 *arg2, s32 arg3);

static inline int check_alarm(const char **ac, const char ** sc, volatile u8 *st) {
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

s32 CD_cw(u8 arg0, const u8 *arg1, u8 *arg2, s32 arg3) {
    s32 i;
    u8 *src;
    u8 *dst;
    const u8 *p;

    if (CD_debug >= 2) {
        DebugPrintStub(D_80011AB0, CD_comstr[arg0]);
    }
    if (D_800A66CC[arg0] != 0 && arg1 == 0) {
        if (CD_debug > 0) {
            DebugPrintStub(StrCdNoParamFmt, CD_comstr[arg0]);
        }
        return -2;
    }
    p = arg1;
    {
        s32 a0_zero = 0;
        CD_sync(a0_zero, 0);
    }
    if (arg0 == 2) {
        for (i = 0; i < 4; i++) {
            CD_pos[i] = p[i];
        }
    }
    if (arg0 == 0xE) {
        CD_mode = *p;
    }
    *(u8 *)&CD_intr_status[0] = 0;
    if (((s32 (*)[64])CdCommandParamTable)[0][arg0] != 0) {
        *(u8 *)&CD_intr_status[1] = 0;
    }
    *CD_reg0 = 0;
    for (i = 0; i < ((s32 (*)[64])CdCommandParamTable)[1][arg0]; i++) {
        *CD_REG2 = p[i];
    }
    CD_com = arg0;
    *CD_REG1 = arg0;
    if (arg3 != 0) {
        return 0;
    }
    CD_timeout_deadline = VSync(-1) + 0x3C0;
    CD_timeout_count = 0;
    CdWaitCallerName[0] = CdCwLabel;
    if (*(u8 *)&CD_intr_status[0] == 0) {
        const char **ac = CD_comstr;
        const char **sc = CD_intstr;
        u8 *st = CD_intr_status;
        u8 *st1 = st + 1;
        do {
            if (check_alarm(ac, sc, st)) {
                return -1;
            }
            if (CheckCallback() != 0) {
                cd_poll_cb(st, st1);
            }
        } while (*st == 0);
    }
    dst = arg2;
    src = CD_syncResult;
    if (dst != 0) {
        s32 n;
        for (n = 7; n != -1; n--) {
            *dst++ = *src++;
        }
    }
    return -(CD_intr_status[0] == 5);
}

int CD_vol(unsigned char *a) {
    *CD_reg0 = 2;
    *CD_REG2 = a[0];
    *(u8 *)CD_reg3 = a[1];
    *CD_reg0 = 3;
    *CD_REG1 = a[2];
    *CD_REG2 = a[3];
    *(u8 *)CD_reg3 = 0x20;
    return 0;
}

void CD_flush(void) {
    volatile unsigned char *p;
    *CD_reg0 = 1;
    if (*(u8 *)CD_reg3 & 7) {
        do {
            *CD_reg0 = 1;
            *(u8 *)CD_reg3 = 7;
            *CD_REG2 = 7;
        } while (*(u8 *)CD_reg3 & 7);
    }
    p = CD_intr_status;
    p[2] = 0;
    p[1] = p[2];
    p[0] = 2;
    *CD_reg0 = 0;
    *(u8 *)CD_reg3 = 0;
    *(s32 *)COM_DELAY = 0x1325;
}

int CD_initvol(void) {
    unsigned char b[4];
    if (*(u16 *)&CdSpuRegBase->curr_vol_left == 0 && *(u16 *)&CdSpuRegBase->curr_vol_right == 0) {
        *(u16 *)&CdSpuRegBase->main_vol_left = 0x3FFF;
        *(u16 *)&CdSpuRegBase->main_vol_right = 0x3FFF;
    }
    *(u16 *)&CdSpuRegBase->cd_vol_left = 0x3FFF;
    *(u16 *)&CdSpuRegBase->cd_vol_right = 0x3FFF;
    *(u16 *)&CdSpuRegBase->spu_control = 0xC001;
    b[2] = 0x80;
    b[0] = 0x80;
    b[3] = 0;
    b[1] = 0;
    *CD_reg0 = 2;
    *CD_REG2 = b[0];
    *(u8 *)CD_reg3 = b[1];
    *CD_reg0 = 3;
    *CD_REG1 = b[2];
    *CD_REG2 = b[3];
    *(u8 *)CD_reg3 = 0x20;
    return 0;
}

void CD_initintr(void) {
    CD_cbready = 0;
    CD_cbsync = 0;
    CD_status1 = 0;
    {
        s32 *base;
        /* PAL mapping: base[0x6494 / 4] is CD_status. */
        base = (s32 *)0x800A0000U;
        base[0x6494 / 4] = 0;
    }
    ResetCallback();
    InterruptCallback(2, CdCommandCallback);
}
