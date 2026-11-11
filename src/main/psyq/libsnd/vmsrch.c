#include "rr/channels.h"
int _SsVmKeyOff(s32 a, s32 b, s32 c, u32 group) {
    s16 first = a;
    s16 second = b;
    s16 third = c;
    u16 selector = group;
    u8 i;
    s32 count = 0;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if ((_snd_vmask & (1 << i)) == 0) {
            if (_svm_voice[i].group0E == selector &&
                _svm_voice[i].match14 == third &&
                _svm_voice[i].match10 == first &&
                _svm_voice[i].match18 == second) {
                if (_svm_voice[i].value00 == 255) {
                    count++;
                    vmNoiseOff(i);
                } else {
                    _svm_cur.f18 = i;
                    _SsVmKeyOffNow(0);
                    count++;
                }
            }
        }
    }
    return count;
}
