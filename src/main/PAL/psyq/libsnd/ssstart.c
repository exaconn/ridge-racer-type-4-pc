/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/sequencer_dispatch.h"
#include "psyq/libsnd/vmtick2.h"
#include "psyq/libsnd/ssramp.h"
#include "psyq/libsnd/ssresum2.h"
#include "psyq/libsnd/ssresume.h"
#include "psyq/libsnd/sstick3.h"
extern void _SsSndTempo(s32, s32);
void SsSeqCalledTbyT(void) {
    s32 bank, row;
    if (_snd_ev_flag != 1) {
        _snd_ev_flag = 1;
        _SsVmFlush();
        for (bank = 0; bank < _snd_seq_s_max; bank++) {
            if (_snd_openflag & (1U << bank)) {
                for (row = 0; row < _snd_seq_t_max; row++) {
                    if (_ss_score[bank][row].w98 & 1) {
                        _SsSndPlay((s16)bank, (s16)row);
                        if (_ss_score[bank][row].w98 & 0x10) _SsSndCrescendo((s16)bank, (s16)row);
                        if (_ss_score[bank][row].w98 & 0x20) _SsSndCrescendo((s16)bank, (s16)row);
                        if (_ss_score[bank][row].w98 & 0x40) _SsSndTempo((s16)bank, (s16)row);
                        if (_ss_score[bank][row].w98 & 0x80) _SsSndTempo((s16)bank, (s16)row);
                    }
                    if (_ss_score[bank][row].w98 & 2) _SsSndPause((s16)bank, (s16)row);
                    if (_ss_score[bank][row].w98 & 8) _SsSndReplay((s16)bank, (s16)row);
                    if (_ss_score[bank][row].w98 & 4) {
                        _SsSndStop((s16)bank, (s16)row);
                        _ss_score[bank][row].w98 = 0;
                    }
                }
            }
        }
        _snd_ev_flag = 0;
    }
}
