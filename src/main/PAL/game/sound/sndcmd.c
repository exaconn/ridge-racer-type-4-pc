/* RODATA 0x80011108 0x10 */
#include "types.h"
#include "rr/audio_loop.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndctrl.h"
#include "game/sound/sndown.h"
#include "game/sound/sndrst2.h"
#include "game/sound/sndvab.h"
#include "psyq/libsnd/vmvinit.h"
#include "psyq/libspu/spukeyst.h"
#include "game/sound/sndcmd.h"
#include "rr/channels.h"
#include "rr/race_setup.h"
#include "rr/audio_bank_setup.h"
#include "game/sound/sndslot.h"

extern void InitSceneOverlayState(void);
extern void SetSceneOverlayColor();
extern void SetSceneOverlayEnabled(s32 arg0);
extern int MaskBitSetting;
extern struct { char p0[0x1C]; s32 x1C; char p1[0x28]; s32 x48; s32 x4C; char p2[0x38C]; s32 x3DC; } RaceContext;

void SoundQueueUpdate(void)
{
    int s0 = 0;
    unsigned short s1;
    int v1;
    /* ASM_BARRIER: pin v0 so both copies read the sra result */
    register int v0 asm("$2");
    int t0;
    int t1;
    unsigned short s2;
    if (GameModeType == 1) {
        if (SoundState.unk1A28 > 0) {
            SoundState.unk1A28 = SoundState.unk1A28 - 1;
        }
    }
    if (SoundState.sequence19D0.active != 0) {
        s2 = SoundState.sequence19D0.programs[SoundState.sequence19D0.readIndex];
        if (SoundState.sequence19D0.programs[SoundState.sequence19D0.readIndex] >= 0) {
            if (SoundState.sequence19D0.startImmediately == 0 || SoundState.sequence19D0.readIndex > 0) {
                if (SpuGetKeyStatus(0x10) == 1) {
                    s0 = -1;
                }
            }
            if (s0 == 0) {
                v1 = SoundState.sequence19D0.rightVolume * Options.audio.volume31;
                s1 = *(u16 *)&SoundState.sequence19D0.leftVolume;
                if (v1 < 0) {
                    v1 += 0xF;
                }
                t1 = v1 >> 4;
                v0 = ((short) s1) * Options.audio.volume31;
                s0 = t1;
                if (v0 < 0) {
                    v0 += 0xF;
                }
                t0 = v0 >> 4;
                s1 = t0;
                if (Options.audio.mono32 != 0) {
                    v0 = (((short) t0) + ((short) t1)) / 2;
                    s0 = v0;
                    s1 = v0;
                }
                SsUtKeyOnV(4, SoundState.bank24[4], (short) s2, 0, 0x3C, 0, (short) s1, (short) s0);
                SsUtKeyOnV(5, SoundState.bank24[4], (short) s2, t1 = 1, 0x3C, 0, (short) s1, (short) s0);
                SoundState.sequence19D0.readIndex = SoundState.sequence19D0.readIndex + t1;
            }
        } else {
            SoundState.sequence19D0.active = 0;
            SoundState.sequence19D0.readIndex = 0;
            SoundState.sequence19D0.unknown0C = 0;
        }
    }
}

void SoundCloseVabsAndReset(void) {
    int idx[4] = {1, 4, 5, 6};
    int i;
    SsUtAllKeyOff(0);
    for (i = 0; i < 4; i++) {
        if (SoundState.bank24[idx[i]] >= 0) {
            SsVabClose(SoundState.bank24[idx[i]]);
            SoundState.bank24[idx[i]] = -1;
        }
    }
    SsSetReservedVoice(0x18);
    SoundState.active0C = 0;
    SoundMasterVolumeReset();
    ResetPlayerVoices();
    InitLoopVoices(0);
    ResetSoundChannelVolLR();
    SoundQueueClear();
    SoundLoopReset();
    ReverbReset();
    SoundStopAll();
    SsUtAllKeyOff(0);
    SoundState.seqTrack2868 = -1;
    SoundState.muted04 = 0;
}

void SoundInitForRace(void) {
    SsUtAllKeyOff(0);
    SsSetReservedVoice(4);
    SoundState.active0C = 1;
    SoundState.muted04 = 0;
    SoundReset(SoundState.split10, 1);
    InitSceneOverlayState();
    MaskBitSetting = 1;
    SetSceneOverlayColor(0, 0x90, 0x90, 0x90);
    SetSceneOverlayEnabled(RaceContext.x3DC);
    SlotsStartForRace();
    SoundState.slots38[0].levels20.halves.level = 0x52;
    SoundState.slots38[1].levels20.halves.level = 0x52;
    SoundState.slots38[2].levels20.halves.level = 0x4E;
    SoundState.slots38[3].levels20.halves.level = 0x4E;
    RaceContext.x4C = -1;
    RaceContext.x1C = 1;
    RaceContext.x48 = 0;
}

void StopAllSoundAndReset(void) {
    SsUtAllKeyOff(0);
    SsSetReservedVoice(4);
    SoundState.active0C = 1;
    SoundState.muted04 = 0;
    SoundReset(SoundState.split10, 0);
    CameraCutFlag = 0;
}
