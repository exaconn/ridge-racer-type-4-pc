/* RODATA 0x80011160 0x14 */
extern struct {
    char pad0[0x2670];
    int f2670;
    char pad1[0x2678 - 0x2674];
    int f2678;
    char pad2[0x2688 - 0x267C];
    int f2688;
    char pad3[0x2694 - 0x268C];
    int f2694;
    char pad4[0x269C - 0x2698];
    int f269C;
    char pad5[0x26B8 - 0x26A0];
    int f26B8;
} SoundState;
extern unsigned int D_800F7430;
extern int CdAudioSetVolume();
extern int CdSync();
extern int CdControl();

void CdAudioPauseStep()
{
    int r;
    switch (D_800F7430) {
    case 0:
        SoundState.f269C = 2;
        CdAudioSetVolume(0);
        SoundState.f2678 = SoundState.f2678 + 1;
    case 1:
        if (CdSync(1, 0) == 0) break;
        SoundState.f2678 = SoundState.f2678 + 1;
    case 2:
        if (CdControl(9, 0, 0) == 0) break;
        goto inc;
    case 3:
        r = CdSync(1, 0);
        if (r == 2) {
    inc:
            SoundState.f2678 = SoundState.f2678 + 1;
        } else if (r == 5) {
            D_800F7430 = 1;
        }
        break;
    case 4:
        SoundState.f2670 = -1;
        SoundState.f2678 = 0;
        if (SoundState.f26B8 == 1) SoundState.f2694 = SoundState.f2688;
        break;
    }
}
