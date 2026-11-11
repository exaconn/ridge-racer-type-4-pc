#include "rr/channel_reset.h"

s16 SsUtKeyOff(s16 voice, s16 bank, s16 program, s16 instrument, s16 group) {
    if (_snd_ev_flag != 1) {
        _snd_ev_flag = 1;
        if ((u16)voice < 24) {
            if (_svm_voice[voice].match18 == bank &&
                _svm_voice[voice].match14 == program &&
                _svm_voice[voice].unk16 == instrument &&
                _svm_voice[voice].group0E == group &&
                _svm_voice[voice].value00 == 255) {
                vmNoiseOff((u8)voice);
            } else {
                _svm_cur.f18 = voice;
                _SsVmKeyOffNow(0);
            }
            _svm_voice[voice].field2A = 0;
            _svm_voice[voice].field1E = 0;
            _snd_ev_flag = 0;
            return 0;
        }
        _snd_ev_flag = 0;
    }
    return -1;
}
