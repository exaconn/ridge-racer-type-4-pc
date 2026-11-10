/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libspu/libspu.h"
#include "psyq/libsnd/vmtick2.h"
#include "psyq/libsnd/vmreset.h"
#include "psyq/libsnd/vmvinit.h"
#include "psyq/libspu/sminit.h"
#include "game/data/globals.h"
#include "rr/spu_voice_cache.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
} Unk80110008;

extern Unk80110008 _svm_rattr;
extern u8 D_801101C8[];

void _SsVmInit(s32 arg0) {
    SpuVoiceAttr attr;
    u16 i;
    u16 *p;
    s32 v;
    s32 c18, cff, cm1, c40, c1;

    _spu_setInTransfer(0);
    _svm_damper = 0;
    SpuInitMalloc(0x20, D_801101C8);

    i = 0;
    p = (u16 *)_svm_sreg_buf;
    do {
        p[i] = 0;
        i++;
    } while (i < 0xC0);

    for (i = 0; i < 0x18; i++) {
        _svm_sreg_dirty.flags[i] = 0;
    }

    _svm_vab_count = 0;

    for (i = 0; i < 0x10; i++) {
        _svm_vab_used[i] = 0;
    }

    v = (s8)arg0;
    if (v >= 0x18U) {
        spuVmMaxVoice = 0x18;
    } else {
        spuVmMaxVoice = v;
    }

    attr.mask = 0x60093;
    i = 0;
    attr.pitch = 0x1000;
    attr.addr = 0x1000;
    attr.adsr1 = 0x80FF;
    attr.volume.left = 0;
    attr.volume.right = 0;
    attr.adsr2 = 0x4000;

    if (spuVmMaxVoice > 0) {
        c18 = 0x18;
        cff = 0xFF;
        cm1 = -1;
        c40 = 0x40;
        c1 = 1;
        do {
            _svm_voice[i].age02 = c18;
            _svm_voice[i].value00 = cff;
            _svm_voice[i].flags1D = 0;
            _svm_voice[i].value04 = 0;
            _svm_voice[i].value06 = 0;
            _svm_voice[i].match10 = cm1;
            _svm_voice[i].unk12 = 0;
            _svm_voice[i].match14 = 0;
            _svm_voice[i].unk16 = cff;
            _svm_voice[i].unk08 = 0;
            _svm_voice[i].unk0C = 0;
            _svm_voice[i].unk0A = c40;
            _svm_voice[i].field1E = 0;
            _svm_voice[i].field20 = 0;
            _svm_voice[i].field22 = 0;
            _svm_voice[i].field24 = 0;
            _svm_voice[i].field2A = 0;
            _svm_voice[i].field2C = 0;
            _svm_voice[i].field2E = 0;
            _svm_voice[i].field30 = 0;
            _svm_voice[i].field32 = 0;
            _svm_voice[i].field26 = 0;
            attr.voice = c1 << i;
            SpuSetVoiceAttr(&attr);
            _svm_voice_cursor = i;
            _SsVmKeyOffNow(1);
            i++;
        } while (i < spuVmMaxVoice);
    }

    _svm_rattr.unk0 = 0;
    _svm_rattr.unk8 = 0x3FFF;
    _svm_rattr.unkA = 0x3FFF;
    _svm_rattr.unk4 = 0;
    _svm_okon1 = 0;
    _svm_okon2 = 0;
    _svm_okof1 = 0;
    _svm_orev1 = 0;
    _svm_orev2 = 0;
    _svm_onos1 = 0;
    _svm_onos2 = 0;
    _svm_auto_kof_mode = 0;
    _svm_stereo_mono = 0;
    kMaxPrograms = 0x80;
    _SsVmFlush();
}
