typedef struct {
    char pad0[0x2664];
    int f2664;
    char pad1[0x2670 - 0x2668];
    int f2670;
    char pad2[0x2678 - 0x2674];
    int f2678;
    char pad3[0x2694 - 0x267C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26A4 - 0x26A0];
    int f26A4;
    char pad6[0x26B0 - 0x26A8];
    int f26B0;
    int f26B4;
    int f26B8;
    char pad7[0x26C4 - 0x26BC];
    unsigned short h26C4;
    char pad8[0x26CE - 0x26C6];
    unsigned short h26CE;
} S;
extern S SoundState;

void CdAudioRequestPause()
{
    switch (SoundState.f269C) {
    case 0:
    case 1:
        if (SoundState.f2670 != 2) {
            SoundState.f2670 = 2;
            SoundState.f2678 = 0;
            SoundState.h26CE = SoundState.h26C4;
        }
        if (SoundState.f269C == 1) {
            SoundState.f26B8 = SoundState.f269C;
        } else {
            SoundState.f26B8 = 0;
            SoundState.f2694 = SoundState.f2664;
        }
        break;
    case 2:
        if (SoundState.f2670 != 3) {
            SoundState.f2670 = 3;
            SoundState.f26A4 = 0;
            SoundState.f2678 = 0;
        }
        break;
    }
    if (SoundState.f2670 == 1) {
        if (SoundState.f26B0 < 0x20) {
            SoundState.f26B4 = SoundState.f2670;
            return;
        }
        SoundState.f2670 = 2;
        SoundState.f2678 = 0;
        SoundState.h26CE = SoundState.h26C4;
    }
}
