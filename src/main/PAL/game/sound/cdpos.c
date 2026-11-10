#include "types.h"
#include "rr/audio_cd_controller.h"
#include "game/sound/cdmain.h"
#include "psyq/libcd/s_1.h"
#include "game/sound/cdpos.h"
#include "rr/overlay668_stream.h"
#include "rr/menu_overlay_runtime.h"

typedef struct {
    unsigned char pos[4];
    unsigned int size;
    char name[16];
} CdlFILE;

extern CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
extern char R4StrPath[];
extern char CdAudioTrackTable[];

void CdAudioFindStreamFile(void) {
    struct { u8 unused[8]; u8 b[8]; CdlFILE c; } L;
    SoundState.streamBanks[0].table = AUDIO_STREAM_TABLE_UNAVAILABLE;
    SoundState.streamBanks[1].table = AUDIO_STREAM_TABLE_UNAVAILABLE;
    if (CdControlB(1, 0, L.b) != 0) {
        u32 v = L.b[0];
        if ((v & 0x10) == 0) {
            if ((v & 1) == 0) {
                CdSearchFile(&L.c, R4StrPath);
                SoundState.streamBanks[0].sector = CdPosToInt(L.c.pos) & 0xFFFFFF;
                SoundState.streamBanks[0].table = (u8 *)CdAudioTrackTable;
            }
        }
    }
}

void CdAudioInit(void) {
    SoundState.cdMode2658 = 0xC8;
    CdControlB(0xE, &SoundState.cdMode2658, 0);
    SsSetSerialVol(0, 0x7F, 0x7F);
    CdReadyCallback(CdAudioReadyCallback);
    SoundState.sector2664 = -1;
    SoundState.action2670 = -1;
    SoundState.phase2674 = 0;
    SoundState.step2678 = 0;
    SoundState.state269C = -1;
    SoundState.timer26A8 = 0;
    SoundState.pausedState26B8 = 0;
    SoundState.playActive26BC = 0;
    SoundState.currentVol26C4 = 0;
    SoundState.fadeLength26C6 = 0;
    SoundState.fadeTick26C8 = 0;
    SoundState.fadeTarget26CA = 0;
    CdAudioSetVolume(0x7F);
    SoundState.seqTrack2868 = -1;
    CdPlaybackTrackIndex = 0;
}
