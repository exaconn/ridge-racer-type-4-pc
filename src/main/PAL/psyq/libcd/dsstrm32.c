/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "rr_asm.h"
#include "psyq/libcd/dsmedia2.h"
#include "psyq/libcd/dsstrm4.h"
#include "psyq/libgte/geo_.h"
#include "psyq/libcd/dsstrm32.h"
#include "psyq/libcd/dsstrm2.h"
#include "game/data/globals.h"

extern s32 CdReady(s32, u8 *);

typedef struct {
    u32 x;
} __attribute__((packed)) Packed32;

typedef struct {
    u16 state;
    u16 type;
    u16 sector_offset;
    u16 sector_size;
    u16 frame_no;
    u16 pad0A;
    u16 width;
    u16 height;
    u8 pad10[12];
    Packed32 headm;
} StHeader;

extern volatile u8 *CD_REG2_STRM, *DS_cd_reg3;
extern volatile s32 *CDROM_DELAY_SIZE, *CdComDelayReg, *D_800A6890, *DS_dma_regs;
extern s32 StCdIntrState;
extern s32 D_801102DC;
extern s32 StRingHeaderIndex;
extern u32 StEndFrame;
extern void *StRingSectorDest;
extern StHeader *StRingHeaders;
extern s32 StRingSize;
extern void (*StRingFullFunc)(void);
extern StHeader *StCurHeader;

void StCdInterrupt(void) {
    u32 i;
    struct {
        s16 sp20;
        volatile u16 sp22;
        volatile s16 sp24;
        s16 sp26;
        u8 sp28[8];
        u8 sp30[8];
    } s;
    s32 var_t0;
    s32 *page_8011;
    s32 *page_800a;
    s32 one;
    s32 st;
    StHeader *hdr_v1;

    st = StStreamStarted;
    one = 1;
    if (st == one) {
        return;
    }
    if (D_801102D8 != 0) {
        if (*D_800A6890 & 0x01000000) {
            s32 f300 = StRingBase;
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x2DC / 4] = one;
            if (f300 != 0) {
                StRingHeaderIndex += 1;
            }
            page_800a = (s32 *)0x800A0000U;
            page_800a[0x68B8 / 4] = one;
            return;
        }
    }
    if (CdReady(one, s.sp30) == 5) {
        return;
    }
    s.sp22 = s.sp30[0];
    s.sp24 = s.sp30[1];
    if (s.sp22 & 4) {
        st = 3;
        page_800a = (s32 *)0x800A0000U;
        page_800a[0x68B8 / 4] = st;
        return;
    }
    {
        s32 f2f4 = StRingWriteIndex;
        hdr_v1 = StRingHeaders;
        hdr_v1 = (StHeader *)((u8 *)hdr_v1 + (f2f4 << 5));
    }
    StCurHeader = hdr_v1;
    /* ASM_BARRIER: Keep hdr_v1 in v1 after storing to StCurHeader. */
    __asm__ volatile("" : "=r"(hdr_v1) : "0"(hdr_v1) : "memory");
    if (hdr_v1->state != 0) {
        if (StRingBase != 0) {
            StRingHeaderIndex += 1;
        }
        st = 4;
        page_800a = (s32 *)0x800A0000U;
        page_800a[0x68B8 / 4] = st;
        return;
    }
    *CD_REG0_STRM = 0;
    *DS_cd_reg3 = 0;
    *CD_REG0_STRM = 0;
    *DS_cd_reg3 = 0x80;
    *CDROM_DELAY_SIZE = 0x20943;
    *CdComDelayReg = 0x1323;
    i = 0;
    if (CdReadModeFlag == 0) {
        u8 *buf_base = s.sp28;
        do {
            u8 *p78 = CD_REG2_STRM;
            u8 *dst = buf_base + i;
            u8 val = *p78;
            i += 1;
            *dst = val;
        } while (i < 4U);
        i = 0;
        {
            volatile u8 *p78 = CD_REG2_STRM;
            RR_NOP_DEP_ON(p78);
            do {
                *p78;
                i += 1;
            } while (i < 8U);
        }
    }
    var_t0 = 0x11000000;
    if (StRingBase != 0) {
        mem2mem(StCurHeader, StRingBase + (StRingHeaderIndex << 11), 8, 0);
    } else {
        dma_execute(3, StCurHeader, 0, 8, var_t0, 0, 0);
    }
    while (*DS_dma_regs & 0x01000000) {
    }
    {
        s32 c20843 = 0x20843;
        StHeader *h;
        s32 *p80;
        h = StCurHeader;
        p80 = CDROM_DELAY_SIZE;
        h->headm = *(Packed32 *)s.sp28;
        *p80 = c20843;
    }
    *CdComDelayReg = 0x1325;
    if (StMaskEnabled == 1) {
        s32 e4 = StStartFrame;
        if (e4 != 0) {
            hdr_v1 = StCurHeader;
            if (e4 != hdr_v1->frame_no) {
                hdr_v1->state = 0;
                /* ASM_BARRIER: Store state before checking StRingBase. */
                __asm__ volatile("" : : : "memory");
                if (StRingBase != 0) {
                    s32 f2f0 = StRingHeaderIndex + 1;
                    page_8011 = (s32 *)0x80110000U;
                    page_8011[0x2F0 / 4] = f2f0;
                }
                return;
            }
            StMaskEnabled = 0;
        }
    }
    if (StCurHeader->state != 0x160 || ((StCurHeader->type >> 10) & 0x1F) != D_801102E8) {
        if (StRingBase != 0) {
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x2F0 / 4] = 0;
        } else {
            *(volatile u16 *)&StCurHeader->state;
        }
        StCdIntrState = 5;
        StCurHeader->state = 0;
        return;
    }
    if (StCurSectorOffset != StCurHeader->sector_offset || (StCurFrameNo != 0 && StCurFrameNo != StCurHeader->frame_no)) {
        StCurFrameNo = 0;
        StCurSectorOffset = 0;
        init_ring_status(StRingFrameStartIndex, StRingWriteIndex - StRingFrameStartIndex);
        StRingWriteIndex = StRingFrameStartIndex;
        StCurHeader->state = 0;
        /* ASM_BARRIER: Store state before checking StRingBase. */
        __asm__ volatile("" : : : "memory");
        if (StRingBase != 0) {
            StRingHeaderIndex += 1;
        }
        st = 6;
        page_800a = (s32 *)0x800A0000U;
        page_800a[0x68B8 / 4] = st;
        return;
    }
    if (StCurHeader->sector_offset == 0) {
        s32 fn = StCurHeader->frame_no;
        u32 f304 = StEndFrame;
        StCurSectorOffset = 0;
        StCurFrameNo = fn & 0xFFFF;
        if (f304 != 0 && StCurFrameNo >= f304) {
            void (*cb)(void);
            StCurFrameNo = 0;
            StCurSectorOffset = 0;
            init_ring_status(StRingFrameStartIndex, StRingWriteIndex - StRingFrameStartIndex);
            StRingWriteIndex = StRingFrameStartIndex;
            StCurHeader->state = 0;
            /* ASM_BARRIER: Store state before loading callback. */
            __asm__ volatile("" : : : "memory");
            cb = StRingFullFunc;
            st = 1;
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x308 / 4] = st;
            if (cb != 0) {
                cb();
            }
            if (StRingBase != 0) {
                StRingHeaderIndex += 1;
            }
            st = 7;
            page_800a = (s32 *)0x800A0000U;
            page_800a[0x68B8 / 4] = st;
            return;
        }
        if ((u32)(StRingSize - StRingWriteIndex - 1) < StCurHeader->sector_size) {
            if (StEndFrame == 0) {
                void (*cb)(void);
                StCurHeader->state = 1;
                /* ASM_BARRIER: Store state before loading callback. */
                __asm__ volatile("" : : : "memory");
                cb = StRingFullFunc;
                st = 1;
                page_8011 = (s32 *)0x80110000U;
                page_8011[0x308 / 4] = st;
                if (cb != 0) {
                    cb();
                }
                if (StRingBase != 0) {
                    StRingHeaderIndex += 1;
                }
                st = 8;
                page_800a = (s32 *)0x800A0000U;
                page_800a[0x68B8 / 4] = st;
                return;
            }
            if ((s16)StRingHeaders->state != 0) {
                StCurHeader->state = 0;
                /* ASM_BARRIER: Store state before checking StRingBase. */
                __asm__ volatile("" : : : "memory");
                if (StRingBase != 0) {
                    StRingHeaderIndex += 1;
                }
                st = 9;
                page_800a = (s32 *)0x800A0000U;
                page_800a[0x68B8 / 4] = st;
                return;
            }
            StCurHeader->state = 1;
            /* ASM_BARRIER: Store state before reloading StRingHeaders. */
            __asm__ volatile("" : : : "memory");
            {
                s32 *dst = (s32 *)StRingHeaders;
                s32 *src = (s32 *)StCurHeader;
                i = 0;
                StRingWriteIndex = 0;
                do {
                    s32 val = *src;
                    src += 1;
                    i += 1;
                    *dst = val;
                    dst += 1;
                } while (i < 8U);
            }
            StCurHeader = StRingHeaders;
        }
        StRingFrameStartIndex = StRingWriteIndex;
    }
    StCdIntrState = 0xA;
    StCurSectorOffset = (u16)StCurSectorOffset + 1;
    StRingSectorDest = (u8 *)(StRingHeaders + StRingSize) + StRingWriteIndex * 0x7E0;
    var_t0 = 0x11000000;
    if (D_801102D8 != 0) {
        {
            s32 c20943 = 0x20943;
            s32 *p80 = CDROM_DELAY_SIZE;
            s32 *p84;
            *p80 = c20943;
            p84 = CdComDelayReg;
            *p84 = 0x1323;
        }
    } else {
        s32 c2102 = 0x21020843;
        s32 *p80;
        /* ASM_BARRIER: Materialize c2102 and upper half of var_t0 before loading p80. */
        __asm__("" : "=r"(c2102), "=r"(var_t0) : "0"(c2102), "1"(0x11400000));
        p80 = CDROM_DELAY_SIZE;
        /* ASM_BARRIER: Complete var_t0 after loading p80. */
        __asm__("" : "=r"(p80), "=r"(var_t0) : "0"(p80), "1"(var_t0), "r"(c2102));
        var_t0 |= 0x100;
        *(s32 *)p80 = c2102;
    }
    if (StCurHeader->sector_size - 1 == StCurHeader->sector_offset) {
        s32 one_v1 = 1;
        s32 f300 = StRingBase;
        StStreamStarted = one_v1;
        if (f300 != 0) {
            s32 f2f0;
            mem2mem(StRingSectorDest, f300 + (StRingHeaderIndex << 11) + 0x20, 0x1F8, 1);
            f2f0 = StRingHeaderIndex + 1;
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x2F0 / 4] = f2f0;
        } else {
            dma_execute(3, StRingSectorDest, 0, 0x1F8, var_t0, one_v1, 0);
        }
        {
            s32 e0 = D_801102E0;
            StCurSectorOffset = 0;
            StCurFrameNo = 0;
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x2E8 / 4] = e0;
        }
    } else {
        s32 f300 = StRingBase;
        if (f300 != 0) {
            s32 f2f0;
            mem2mem(StRingSectorDest, f300 + (StRingHeaderIndex << 11) + 0x20, 0x1F8, 0);
            f2f0 = StRingHeaderIndex + 1;
            page_8011 = (s32 *)0x80110000U;
            page_8011[0x2F0 / 4] = f2f0;
        } else {
            dma_execute(3, StRingSectorDest, 0, 0x1F8, var_t0, 0, 0);
        }
    }
    *CdComDelayReg = 0x1325;
    StCurHeader->state = 3;
    /* ASM_BARRIER: Store state before incrementing StRingWriteIndex. */
    __asm__ volatile("" : : : "memory");
    {
        s32 f2f4 = StRingWriteIndex;
        s32 f300 = StRingBase;
        f2f4 += 1;
        page_8011 = (s32 *)0x80110000U;
        page_8011[0x2F4 / 4] = f2f4;
        if (f300 != 0 && StStreamStarted != 0) {
            data_ready_callback();
        }
    }
}
