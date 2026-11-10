#include "types.h"
#include "game/gfx/fadesel.h"
#include "game/math/shuffle.h"
#include "game/sound/sndctrl.h"
#include "game/gfx/fadein.h"
#include "game/gfx/fadespr.h"
#include "game/ui/hudmap2.h"
#include "game/demo/attract.h"
#include "game/menu/title.h"
#include "rr/audio_loop.h"
#include "rr/bios_devices.h"
#include "rr/color_conversion.h"
#include "rr/race_setup.h"
#include "rr/driver_control.h"
#include "game/data/selection.h"
#include "psyq/libc/rand.h"
#include "game/menu/mnrows.h"

extern int SlotOrder[];
extern char D_800ACB0C[];
extern int D_800A2124[];
extern short CourseIndex;
extern unsigned short D_800A2134;
extern unsigned short D_800A2138[];
extern short MenuTransitionOverlayMode;
extern short MenuScriptId;
extern short ModeGroup;
extern short ModeIndex;
extern short SubmodeState;
extern int MaskBitSetting;
extern char RaceContext[];
extern short ActiveSceneId;
extern int InitSceneOverlayState();
extern int SetSceneOverlayColor();
extern int SetSceneOverlayEnabled();

void StartAttractDemo(int a0);
void AttractDemoRaceEnter(void);

void StartAttractDemo(int a0) {
    int sp[4];
    int i;
    int y;
    int u;
    int *tb;
    int *q;
    int s0;
    int s2;
    int v;
    /* ASM_BARRIER: pin rand()%2 result to $v0 like the original shuffle loop */
    register int r __asm__("$2");
    int *w;
    struct { unsigned char a, b, c, d; } *b;
    unsigned short m;
    unsigned short *e;
    int n;
    int *d;
    unsigned short h;
    char *c;
    int x6;
    int *pa, *pb;
    int vq;
    char *c2;
    int x5;

    if (SlotOrder[0] != 0) {
        ShuffleSlotOrder();
        SlotOrder[0] = 0;
    }
    PsyqBzero(D_800ACB0C, 0x80);
    ((int *)D_800ACB0C)[3] = a0;
    i = 3;
    q = sp + 3;
    do {
        *q = i;
        i--;
        q--;
    } while (i >= 0);
    i = 0;
    do {
        s0 = PsyqRand() % 4;
        r = PsyqRand() % 2;
        v = s0 + r;
        vq = v % 4;
        s2 = sp[s0];
        y = sp[vq];
        sp[s0] = y;
        sp[vq] = s2;
        i++;
    } while (i < 0x10);
    i = 0;
    x6 = PsyqRand();
    s2 = x6 % 18;
    tb = D_800A2124;
    w = AttractCarTeams;
    b = (void *)SelectionRows;
    do {
        b[i].a = sp[i];
        b[i].b = tb[sp[i]];
        *w = tb[sp[i]];
        u = s2 + PsyqRand() % 4;
        v = u - 2;
        if (v < 0) v = 0;
        if (v > 0x12) v = 0x12;
        b[i].c = v;
        w++;
        i++;
    } while (i < 4);
    CourseIndex = -1;
    if (a0 == 1) {
        m = D_800F3A3A & D_800A2134;
        if (m != 0) {
            i = 0;
            e = D_800A2138;
            do {
                if (m == e[0])
                    CourseIndex = e[1];
                i++;
                e += 2;
            } while (i < 8);
        }
    }
    if (CourseIndex < 0) {
        d = SlotOrder;
        n = d[9];
        h = ((unsigned short *)d)[n * 2 + 2];
        n = n + 1;
        d[9] = n;
        CourseIndex = h;
        if (n >= 8)
            ShuffleSlotOrder();
    }
    InitRaceOpeningOverlay(0, 0);
    GameModeType = 5;
    MenuTransitionOverlayMode = 0;
    MenuScriptId = 0xA;
    ModeGroup = 1;
    ModeIndex = 0x23;
    SubmodeState = 0;
    if (a0 == 0) {
        StartDemoFadeIn(-1);
        RaceBgmListIndex = CourseBgmChoiceTable2[CourseIndex] + 1;
    } else {
        InitDemoFadeContext(0);
        SelectDemoFadePreset(2);
        RaceBgmListIndex = 1;
    }
    InitSceneOverlayState();
    c = RaceContext;
    MaskBitSetting = 1;
    ((short *)c)[0x4E0 / 2] = 0xD2;
    ((short *)c)[0x4E2 / 2] = 0;
    ((short *)c)[0x4E4 / 2] = 0x8FC;
    HsvToRgb(c + 0x4E0, c + 0x4E8);
    SetSceneOverlayColor(0, c[0x4E8], c[0x4E9], c[0x4EA]);
    ((int *)c)[0x534 / 4] = 0;
    if (a0 == 0) {
        x5 = ((int *)c)[0x3DC / 4];
    } else {
        ((int *)c)[0x3DC / 4] = 0;
        x5 = 0;
    }
    SetSceneOverlayEnabled(x5);
    c2 = RaceContext;
    ((int *)c2)[0x4F0 / 4] = -0x20;
    ((int *)c2)[0x4EC / 4] = 0;
    ((int *)c2)[0x4F4 / 4] = 0;
    ((int *)c2)[0x4FC / 4] = 0;
    ((int *)c2)[0x500 / 4] = 0;
    ((int *)c2)[0x504 / 4] = 0;
    ((int *)c2)[0x514 / 4] = 0x122;
    SoundReverbOffAll();
    ClearTrackObjectStates(0);
    ActiveSceneId = 0x209;
}

void AttractDemoRaceEnter(void) {
    SetDispMask(0);
    ResetScreenCounters();
    MenuScriptId = 8;
    ModeGroup = 1;
    ModeIndex = 0x23;
    SubmodeState = 0;
    InitDemoFadeContext(0);
    SelectDemoFadePreset(1);
    DrawSubOtFlag = 0;
}
