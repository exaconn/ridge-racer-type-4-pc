#include "rr/sequencer_init.h"

void SsSetTableSize(LibStateB0 *base, s32 raw_banks, s32 raw_rows) {
    s16 banks = raw_banks;
    s16 rows = raw_rows;
    s32 i, j;
    _snd_seq_s_max = banks;
    _snd_seq_t_max = rows;
    for (i = 0; i < banks; i++) {
        _ss_score[i] = &base[i * rows];
    }
    for (i = banks; i < 32; i++) {
        _snd_openflag |= 1U << i;
    }
    for (i = 0; i < _snd_seq_s_max; i++) {
        for (j = 0; j < _snd_seq_t_max; j++) {
            _ss_score[i][j].w98 = 0;
            _ss_score[i][j].b22[0] = -1;
            _ss_score[i][j].b22[1] = 0;
            _ss_score[i][j].h48[0] = 0;
            _ss_score[i][j].h48[1] = 0;
            _ss_score[i][j].w9c = 0;
            _ss_score[i][j].wa0 = 0;
            _ss_score[i][j].h48[2] = 0;
            _ss_score[i][j].wac = 0;
            _ss_score[i][j].wa8 = 0;
            _ss_score[i][j].wa4 = 0;
            _ss_score[i][j].h48[3] = 0;
            _ss_score[i][j].h58[0] = 127;
            _ss_score[i][j].h58[1] = 127;
            _ss_score[i][j].h58[2] = 127;
            _ss_score[i][j].h58[3] = 127;
        }
    }
}
