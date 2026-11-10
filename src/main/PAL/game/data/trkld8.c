/* RODATA 0x800110c0 0x18 */
#include "game/data/cdfile2.h"
#include "rr/load_cursor.h"
#include "game/gfx/motion1.h"
#include "game/sound/cdpos.h"
#include "psyq/libcd/s_1.h"
#include "psyq/libspu/s_i.h"
#include "game/data/trkld1.h"
#include "game/data/trkld8.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "rr/course_choices.h"
#include "rr/sequencer_init.h"
#include "rr/spu_tick.h"
#include "rr/bios_devices.h"
#include "rr/audio_state_types.h"
#include "game/course/trkintp2.h"
extern short SelectedTeamIndex;
extern int D_800AD564;
extern short CourseIndex;
extern int MenuArenaBase;
extern char D_800F74B0[];
extern char RaceContext[];
extern void SsSetSerialAttr(signed char a0, signed char a1, signed char a2);
extern int SsSetSerialVol(signed char a0, int a1, int a2);
extern void InitSceneOverlayState(void);
extern void CdAudioInit(void);

int LoadIntroSceneStep(void) {
    int v;
    int t;
    int u;
    int w;
    int g;
    int h;
    switch (RaceLoadStepIndex) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(1, 10), RaceLoadCursor);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 1:
            t = CdFileBytesLoaded();
            if (t != 0) {
                u = AlignUp4(t);
                w = RaceLoadCursor + u;
                CourseIndex = 8;
                LoadCursor = w;
                RaceLoadCursor = w;
                MenuArenaBase = w;
                RaceLoadStepIndex += 1;
            }
    default:
            v = 0;
        }
        return v;
    case 2:
        v = RequestSceneLoad(CourseIndex);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 3:
            if (SceneLoadStep() != 0) {
                RaceLoadStepIndex += 1;
            }
            v = 0;
        }
        return v;
    case 4:
        g = D_800AD564 << 2;
        h = SelectedTeamIndex + 0x97;
        v = CdFileReadStart(GetR4FileIndex(2, g + h), LoadCursor);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 5:
            if (CdFileBytesLoaded() != 0) {
                LoadTimList(LoadCursor);
                RaceLoadStepIndex = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    }
}

void SoundSystemInit(void) {
    char *s1;
    /* ASM_BARRIER: keep SPU table cursor in $s0 across init calls */
    register char *s0 asm("$16");
    int i; int n; short m;
    CdReset(1);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0x7F, 0x7F);
    s0 = D_800F74B0;
    SsSetTableSize(s0, 2, 1);
    SsSetTickMode(1);
    s1 = s0 - 0x26F8;
    SsStart();
    SsSetMVol(0x7F, 0x7F);
    PsyqBzero(s1, 0xA8D0);
    PsyqBzero(RaceContext, 0x57C);
    s0 -= 0x1498;
    InitSceneOverlayState();
    ResetSlotOrder();
    ((AudioState *)s1)->master08 = 0x80;
    n = -1;
    for (i = 3; i >= 0; i--) {
        *(AudioBankStorage32 *)&((AudioState *)s0)->slots38[0].bank04 = n;
        s0 -= 0x620;
    }
    m = -1;
    for (i = 7; i >= 0; i--)
        SoundState.bank24[i] = m;
    SoundState.seqInit2860 = -1;
    SoundState.seqInit2862 = -1;
    CdAudioInit();
    CdAudioFindStreamFile();
}
