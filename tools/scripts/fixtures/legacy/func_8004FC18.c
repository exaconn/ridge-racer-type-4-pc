extern void SoundStopAll();
extern void CdAudioFade(int, short);
extern struct { char pad[0x18]; unsigned short a; short b; short c; unsigned short d; unsigned short e; } SoundState;

void SoundSetMasterVolume(short arg0, short arg1) {
    if (arg1 >= 2) {
        SoundState.b = arg1;
        SoundState.c = 0;
        SoundState.d = arg0;
        SoundState.e = SoundState.a;
    } else {
        SoundState.a = arg0;
        SoundState.b = 0;
        SoundState.c = 0;
        SoundState.d = 0;
        SoundState.e = 0;
        if (arg0 == 0) {
            SoundStopAll();
        }
    }
    CdAudioFade(arg0 & 0xFFFF, arg1);
}
