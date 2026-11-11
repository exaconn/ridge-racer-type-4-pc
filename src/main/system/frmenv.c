#include "game/sound/cdpos.h"
#include "rr/emblem_images.h"
#include "rr/save_slots.h"
#include "game/car/carstat.h"
#include "game/data/cdfile2.h"
#include "rr/frame_environment.h"
#include "rr/load_cursor.h"
#include "rr/frame_loop.h"
#include "rr/save_progress.h"
#include "game/car/car.h"
#include "game/car/carstat.h"
#include "game/data/cardini.h"
#include "game/data/cdfile1.h"
#include "game/data/trkld8.h"
#include "game/gfx/fadespr.h"
#include "game/gfx/model.h"
#include "game/menu/menu8.h"
#include "game/race/racesamp.h"
#include "game/race/ranking.h"
#include "psyq/libgpu/gpuenv.h"
#include "psyq/libsio/siostart.h"
#include "psyq/libspu/sinit2.h"
#include "game/data/ldscn1.h"
#include "game/menu/menusave.h"
#include "game/system/frmenv.h"
#include "rr/audio_cd_controller.h"
#include "rr/library.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"
#include "game/data/selection.h"
#include "rr/string_tail_copy.h"
#include "game/menu/mnrows.h"

extern int InitGeom();

#include "rr/cd_directory_buffer.h"
extern char StrTotalInitLog[];
extern char DefaultPlayerName[];
extern char TeamNameDigRacingTeam[];
extern char TeamAbbrevDRT[];
extern char TeamNameMicroMouseMappy[];
extern char TeamAbbrevMicroMouseMappy[];
extern char StrPacRacingClub[];
extern char TeamAbbrevPRC[];
extern char TeamNameRacingTeamSolvalou[];
extern char TeamAbbrevRTS[];
extern char TeamNameAssoluto[];
extern char TeamNameLizard[];
extern char StrTerrazi[];
extern char StrAgeSolo[];
extern RaceVehicle PlayerCars[2];
extern int MirrorRenderFlag;
extern short LinkHostFlag;
extern char SaveHeaderPlayerNameBuf[];
extern int TeamStringPtrTable[];
extern int RivalTeamNamePtrTable[];
extern int NumLaps;
extern int DisplayEnvOffsetX;
extern int DisplayEnvOffsetY;
extern int DialogueSceneMode;
#include "rr/menu_arena_base.h"
extern short GameModeType;
extern void SetGeomOffset(s32,s32);
extern void SetGeomScreen(s32);

void SetupDrawDispEnvs(s32 r,s32 g,s16 b,u32 flags);

void SystemInit(void)
{
    int i;
    signed char *r;
    int k;
    int m;

    DebugPrintStub(StrTotalInitLog);
    CdInit();
    InitCdFileDirectory();
    SsInit();
    SoundSystemInit();
    CdAudioInit();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    InitGeom();
    CardSystemInit(1);
    InitGamePads();
    InitOptionsDefaults();
    ApplyOptionsPadButtonMaps();
    AddSIO();
    _sio_control(1, 1, 0);
    SetupRenderer(5);
    VSyncThreshold = 0x80;
    DisplayEnvOffsetX = 0;
    DisplayEnvOffsetY = 0;
    DrawSubOtFlag = 0;
    NumLaps = 2;
    MirrorRenderFlag = 0;
    PsyqBzero(&SaveProgress, 0x7C);
    PsyqBzero(SelectionRows.bytes, 0x20);
    PsyqBzero(SelectionState.signedBytes, 0x2C);
    SelectionState.signedBytes[0x17] = -1;
    PsyqBzero(RankEntries, 8 * sizeof(*RankEntries));
    i = 7;
    do {
        RankTable[i] = &RankEntries[i];
        i--;
    } while (i >= 0);
    PsyqBzero(&PlayerCars[0], 0x320);
    PsyqBzero(&PlayerCars[1], 0x320);
    UnlockNoticeMask = 0;
    for (i = 0; i < 4; i++) {
        SaveProgress.history6C[i].selection = 5;
        SaveProgress.history6C[i].variant = 5;
        SaveProgress.history6C[i].parameter = 0;
    }
    LinkHostFlag = -1;
    CopyStringClearTail(&SaveProgress, DefaultPlayerName);
    CopyStringClearTail(SaveHeaderPlayerNameBuf, &SaveProgress);
    CopyStringClearTail(SaveHeaderPlayerNameBuf + 0xC, &SaveProgress);
    CopyStringClearTail(SaveHeaderPlayerNameBuf + 0x18, &SaveProgress);
    CopyStringClearTail(SaveHeaderPlayerNameBuf + 0x24, &SaveProgress);
    TeamStringPtrTable[0] = TeamNameDigRacingTeam;
    TeamStringPtrTable[4] = TeamAbbrevDRT;
    TeamStringPtrTable[1] = TeamNameMicroMouseMappy;
    TeamStringPtrTable[5] = TeamAbbrevMicroMouseMappy;
    TeamStringPtrTable[2] = StrPacRacingClub;
    TeamStringPtrTable[6] = TeamAbbrevPRC;
    TeamStringPtrTable[3] = TeamNameRacingTeamSolvalou;
    TeamStringPtrTable[7] = TeamAbbrevRTS;
    RivalTeamNamePtrTable[0] = TeamNameAssoluto;
    RivalTeamNamePtrTable[1] = TeamNameLizard;
    RivalTeamNamePtrTable[2] = StrTerrazi;
    RivalTeamNamePtrTable[3] = StrAgeSolo;
    LoadCursor = CdDirectoryBuffer;
    MenuArenaBase = CdDirectoryBuffer;
    InitMenuArenas();
    k = 0xAF000;
    m = -1;
    i = 2;
    DialogueSceneMode = 0;
    GameModeType = 0;
    LoadDestCursor = CdDirectoryBuffer + k;
    r = (s8 *)&SaveSlotSelections + i * 0x2C;
    do {
        r[0x17] = m;
        i--;
        r -= 0x2C;
    } while (i >= 0);
    InitDefaultTimeRecords();
    InitCourseChoices();
    InitTimTemplate(&CustomEmblemBuffers.images[0]);
    InitTimTemplate(&CustomEmblemBuffers.images[1]);
    InitTimTemplate(&SaveDataWork);
    CustomEmblemTimPtr = &CustomEmblemBuffers.images[0];
    ResetRankEntries();
    InitCarTable(1);
    ResetGraph(3);
}

void InitFrameEnvStub(s32 bank) {
}

void SetupDrawDispEnvs(s32 r,s32 g,s16 b,u32 flags) {
    u32 input=flags;
    s32 red=r;
    s32 green;
    u32 temp;
    s32 interlace;
    u32 height;
    u32 other_y;
    s32 width;
    u32 rgb24;
    u32 saved_flags;
    FrameStorage *env;
    GpuRectangleFields window;
    s32 i;
    u16 *settings;
    s32 one;
    u32 loopRgb24;
    u32 background;
    s32 center_x;
    u32 center_y;
    green=g;
    center_x=160;
    temp=input>>1;
    interlace=temp&1;
    temp=interlace+1;
    height=temp<<8;
    center_y=height>>1;
    temp=1-interlace;
    other_y=temp<<8;
    SetGeomOffset(center_x,center_y);
    SetGeomScreen(290);
    width=320;
    rgb24=input&FRAME_RGB24;
    saved_flags=input;
    if(rgb24)width=480;
    env=DrawBuffers;
    SetDefDrawEnv(&env->buffer.environment00.draw00,0,0,width,height);
    SetDefDrawEnv(&DrawBuffers[1].buffer.environment00.draw00,0,other_y,width,height);
    SetDefDispEnv(&env->buffer.environment00.display5C,0,other_y,width,height);
    SetDefDispEnv(&DrawBuffers[1].buffer.environment00.display5C,0,0,width,height);
    i=0;
    background=saved_flags&FRAME_CLEAR_BACKGROUND;
    one=1;
    settings=Options.halves;
    loopRgb24=rgb24;
    window.y=0;
    window.x=0;
    window.h=0;
    window.w=0;
loop:
    {
        env->buffer.environment00.draw00.r=red;env->buffer.environment00.draw00.g=green;env->buffer.environment00.draw00.b=b;
        env->buffer.environment00.draw00.background=background;
        env->buffer.environment00.draw00.dither=one;
        env->buffer.environment00.draw00.window=window;
        env->buffer.environment00.display5C.screen.x=settings[0x34/2];
        env->buffer.environment00.display5C.screen.y=settings[0x38/2];
        env->buffer.environment00.display5C.display.h=height;
        env->buffer.environment00.display5C.flags10.bytes.interlace=interlace;
        env->buffer.environment00.display5C.screen.h=256;
        if(loopRgb24) {
            env->buffer.environment00.display5C.flags10.bytes.rgb24=one;
            env->buffer.environment00.display5C.display.w=(env->buffer.environment00.display5C.display.w*2)/3;
        }
        env++;
    }
    i++;
    if(i<2)goto loop;
    InitFrameEnvStub(0);
    InitFrameEnvStub(1);
}
