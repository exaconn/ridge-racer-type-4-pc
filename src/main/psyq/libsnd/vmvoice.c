#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libspu/s_snc.h"
#include "psyq/libsnd/vmrow.h"
#include "game/data/globals.h"

extern u16 D_8010F90C[];
extern s16 D_8010FE3A[];
extern s8 D_80110105;
extern s8 D_8011010B;
extern u8 D_80110112;

void vmNoiseOn(s32 arg0) {
    s32 s3 = arg0;
    u32 s0;
    u32 left;
    u32 right;
    s8 *p2;
    u16 low_mask, high_mask;
    s16 i;
    u16 f8, fa;

    {
        u8 *f4;
        s32 v0;
        s32 v1;
        s8 *p;
        u32 a3;
        u32 vol;
        s32 ch;
        u32 offset;
        s32 group_off;
        ChannelValueRecordB0 **table;
        ChannelValueRecordB0 *rec;
        s32 pan;

        f4 = _svm_vh;
        p = (s8 *)&_svm_cur_vab_id;
        ch = f4[0x18];
        v1 = p[-0x10];
        v0 = ch * 0x3FFF;
        v1 *= v0;
        a3 = v1 / 16129;
        v1 = a3 * p[-0xA];
        v0 = p[-0x7];
        vol = (v1 * v0) / 16129U;
        ch = *(u16 *)p;
        offset = (ch & 0xFF) * sizeof(ChannelValueRecordB0 *);
        ch = (s16)ch;
        group_off = ((ch & 0xFF00) >> 8) * 11;
        table = (ChannelValueRecordB0 **)0x80110000U;
        table += (s32)(offset / sizeof(*table));
        rec = table[-0xF98 / 4];
        rec = (ChannelValueRecordB0 *)((u8 *)rec + (group_off << 4));
        a3 = vol;
        if (ch != 0x21) {
            a3 = (vol * rec->h58[0]) / 127U;
            vol = (vol * rec->h58[1]) / 127U;
        }
        pan = p[-0x6];
        if (pan < 0x40U) {
            left = a3;
            right = (vol * pan) / 63U;
        } else {
            right = vol;
            left = (a3 * (0x7F - pan)) / 63U;
        }
        pan = D_8011010B;
        if (pan < 0x40U) {
            right = (right * pan) / 63U;
        } else {
            left = (left * (0x7F - pan)) / 63U;
        }
        pan = D_80110105;
        if (pan < 0x40U) {
            right = (pan * right) / 63U;
        } else {
            left = (left * (0x7F - pan)) / 63U;
        }
    }
    if (_svm_stereo_mono == 1) {
        if (left < right) {
            left = right;
        } else {
            right = left;
        }
    }
    p2 = (s8 *)&_svm_cur_vab_id;
    if (*(s16 *)p2 != 0x21) {
        left = (left * left) / 16383U;
        right = (right * right) / 16383U;
    }
    SpuSetNoiseClock((p2[-0x12] - p2[-0x4]) & 0x3F);
    s0 = s3 & 0xFF;
    D_8010FE3A[s0 * 8] = right;
    {
        u8 *flags_page;
        u8 flags;
        flags_page = (u8 *)0x80110000U;
        flags = *(flags_page + s0 - 0x1E8);
        _svm_sreg_buf[s0].left = left;
        ((u8 *)&_svm_sreg_dirty)[s0] = flags | 3;
    }
    if (s0 < 0x10U) {
        low_mask = 1 << s0;
        high_mask = 0;
    } else {
        low_mask = 0;
        high_mask = 1 << (s0 - 0x10);
    }
    D_8010F90C[(s3 & 0xFF) * 27] = 0xA;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (!(_snd_vmask & (1 << i))) {
            u32 row_off = i * 0x36;
            u8 *row_page;
            row_page = (u8 *)0x80110000U;
            row_page += row_off;
            D_8010F925[row_off] = row_page[-0x6DB] & 1;
        }
    }
    D_8010F925[(s3 & 0xFF) * 0x36] = 2;
    f8 = _svm_okon1 | low_mask;
    fa = _svm_okon2 | high_mask;
    _svm_okon1 = f8;
    _svm_okon2 = fa;
    _svm_okof1 &= ~f8;
    _svm_okof2 &= ~fa;
    if (D_80110112 & 4) {
        u16 *base;
        u16 fe;
        _svm_orev1 |= low_mask;
        fe = _svm_orev2 | high_mask;
base = (u16 *)0x80110000U;
base[-0x702 / 2] = fe;
    } else {
        _svm_orev1 &= ~low_mask;
        _svm_orev2 &= ~high_mask;
    }
    _svm_onos1 = low_mask;
    _svm_onos2 = high_mask;
}
