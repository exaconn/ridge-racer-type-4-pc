extern int CdControl();
extern int CdDiskReady();
extern struct { char pad[0x266C]; int a; int b; int pp; int c; int d; int e; char pad2[0x14]; int g; } SoundState;

void CdAudioFilterStep(void) {
    char sp[8];
    switch (SoundState.c) {
    case 0:
        sp[0] = 1;
        sp[1] = SoundState.a;
        if (CdControl(0xD, sp, 0) != 0) {
            SoundState.c++;
            return;
        }
        if (CdDiskReady(1) == 0x10) {
            SoundState.d = 1;
            SoundState.e = -3;
            SoundState.b = -1;
            SoundState.c = 0;
            return;
        }
        return;
    case 1:
        SoundState.b = -1;
        SoundState.c = 0;
        SoundState.e = 0;
        SoundState.g = SoundState.a;
        break;
    }
}
