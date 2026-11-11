#include "types.h"
#include "rr/cd.h"
#include "rr/cd_poll.h"
#include "rr/library.h"
#include "psyq/libcd/s_3.h"

/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
extern int CdTestParamNum;
extern u8 CD_syncResult[];
extern u8 CdReadyResult[];

s32 CD_getsector(u8 *a0, u32 a1);
s32 CD_getsector2(u8 *a, u32 b);
void CD_set_test_parmnum(int a);

static inline int check_alarm(void) {
    if (VSync(-1) > CD_timeout_deadline || CD_timeout_count++ > 0x3C0000) {
        PsyqPuts(CdTimeoutPrefixStr);
        DebugPrintStub(
            CdTimeoutStatusFmt,
            CdWaitCallerName[0],
            CD_comstr[CD_com],
            CD_intstr[CD_intr_status[0]],
            CD_intstr[CD_intr_status[1]]
        );
        CD_flush();
        return -1;
    }
    return 0;
}

s32 CD_datasync(s32 mode) {
    s32 t = VSync(-1);
    CD_timeout_deadline = t + 0x3C0;
    CD_timeout_count = 0;
    CdWaitCallerName[0] = CdDatasyncNameStr;
    do {
        if (check_alarm() != 0) {
            return -1;
        }
        if ((*DMA3_CHCR & 0x1000000) == 0) {
            return 0;
        }
    } while (mode == 0);
    return 1;
}

s32 CD_getsector(u8 *a0, u32 a1) {
    volatile unsigned char *p;
    *CD_reg0 = 0;
    *CD_reg3 = 0x80;
    *CD_DELAY = 0x20943;
    /* Ordinary write views retain the original setup and teardown scheduling. */
    *(u32 *)COM_DELAY = 0x1323;
    *CD_dpcr |= 0x8000;
    *DMA3_MADR = a0;
    *DMA3_BCR = a1 | 0x10000;
    p = CD_reg0;
    do {
    } while (!(*p & 0x40));
    *DMA3_CHCR = 0x11000000;
    if (*DMA3_CHCR & 0x01000000) {
        do {
        } while (*DMA3_CHCR & 0x01000000);
    }
    *(u32 *)COM_DELAY = 0x1325;
    return 0;
}

s32 CD_getsector2(u8 *a, u32 b) {
    volatile int x;
    *CD_reg0 = 0;
    *CD_reg3 = 0x80;
    *CD_DELAY = 0x21020843;
    *COM_DELAY = 0x1325;
    *CD_dpcr |= 0x8000;
    *DMA3_MADR = a;
    *DMA3_BCR = b | 0x10000;
    while (!(*CD_reg0 & 0x40)) {
    }
    *DMA3_CHCR = 0x11400100;
    x = *DMA3_CHCR;
    return 0;
}

void CD_set_test_parmnum(int a) { CdTestParamNum = a; }

void CdCommandCallback(void) {
    u8 *p=&CD_intr_status[1];
    u8 *q=p-1;
    s32 saved=*CD_reg0 & 3;
    s32 result;
    while((result=getintr())) {
        if((result & 4) && CD_cbready) {
            CD_cbready(*p,CdReadyResult);
        }
        if((result & 2) && CD_cbsync) {
            CD_cbsync(*q,CD_syncResult);
        }
    }
    *CD_reg0=saved;
}

