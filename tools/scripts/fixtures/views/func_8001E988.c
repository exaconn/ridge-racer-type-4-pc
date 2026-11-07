#include "rr/frame_loop.h"

extern int func_8001E84C();

extern int CdInit();

extern int InitCdFileDirectory();

extern int func_8007FA54();

extern int SoundSystemInit();

extern int CdAudioInit();

extern int ResetGraph();

extern int SetGraphDebug();

extern int SetDispMask();

extern int InitGeom();

extern int CardSystemInit();

extern int func_8004A55C();

extern int func_8005C0C4();

extern int func_8004A678();

extern int AddSIO();

extern int _sio_control();

extern int SetupRenderer();

extern int func_80096434();

extern int CopyStringClearTail();

extern int InitMenuArenas();

extern int func_8001EEE4();

extern int func_8001F194();

extern int func_8006F5F0();

extern int func_80031DB0();

extern int func_800373DC();

extern int CdDirectoryBuffer;

extern char D_800102F4[];

extern char D_80010300[];

extern char D_8001030C[];

extern char D_80010320[];

extern char D_80010328[];

extern char D_80010348[];

extern char D_80010350[];

extern char D_80010364[];

extern char D_8001036C[];

extern char D_80010384[];

extern char D_8001038C[];

extern char D_80010398[];

extern char D_800103A0[];

extern char D_800103AC[];

extern char PlayerCars[];

extern int D_800AC5A0;

extern int LoadCursor;

extern int D_800AD568;

extern signed char D_800AD4D8[];

extern short D_800AD508;

extern char D_800F2A68[];

extern char D_800F2FE0[];

extern char D_800F3920[];

extern char D_800F39D8[];

extern char D_800F3B28[];

extern char D_800F4D98[];

extern signed char D_800F2E30[];

extern int RankTable[];

extern int D_800F2E08[];

extern int D_800F4BD0[];

extern int NumLaps;

extern int D_800F4C24;

extern int D_800F4C2C;

extern int D_800F4CA8;

extern int D_800F4D40;

extern short D_800F4BE0;

extern short D_800F4D44;

extern char *D_800F3B20;

void SystemInit(void)
{
    int i;
    int *p;
    char *s, *q;
    signed char *r;
    int k, c5;
    int m;

    func_8001E84C(D_800102F4);
    CdInit();
    InitCdFileDirectory();
    func_8007FA54();
    SoundSystemInit();
    CdAudioInit();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    InitGeom();
    CardSystemInit(1);
    func_8004A55C();
    func_8005C0C4();
    func_8004A678();
    AddSIO();
    _sio_control(1, 1, 0);
    SetupRenderer(5);
    VSyncThreshold = 0x80;
    D_800F4C24 = 0;
    D_800F4C2C = 0;
    D_800ACBF8 = 0;
    NumLaps = 2;
    D_800AD568 = 0;
    func_80096434(D_800F3920, 0x7C);
    func_80096434(D_800F4D98, 0x20);
    func_80096434(D_800AD4D8, 0x2C);
    D_800AD4D8[0x17] = -1;
    func_80096434(D_800F2A68, 0x40);
    i = 7;
    do {
        RankTable[i] = (int)(D_800F2A68 + i * 8);
        i--;
    } while (i >= 0);
    func_80096434(PlayerCars, 0x320);
    func_80096434(PlayerCars + 0x320, 0x320);
    D_800F4BE0 = 0;
    i = 0;
    c5 = 5;
    q = D_800F3920;
    do {
        *(volatile char *)(q + 0x6C) = c5;
        *(volatile char *)(q + 0x6D) = c5;
        *(volatile char *)(q + 0x6E) = 0;
        __asm__ volatile("" : : : "memory"); /* ASM_BARRIER: keep i++ after the byte stores like the original schedule */
        i++;
        q += 4;
    } while (i < 4);
    D_800AD508 = -1;
    CopyStringClearTail(D_800F3920, D_80010300);
    CopyStringClearTail(D_800F39D8, D_800F3920);
    CopyStringClearTail(D_800F39D8 + 0xC, D_800F3920);
    CopyStringClearTail(D_800F39D8 + 0x18, D_800F3920);
    CopyStringClearTail(D_800F39D8 + 0x24, D_800F3920);
    D_800F2E08[0] = (int)D_8001030C;
    D_800F2E08[4] = (int)D_80010320;
    D_800F2E08[1] = (int)D_80010328;
    D_800F2E08[5] = (int)D_80010348;
    D_800F2E08[2] = (int)D_80010350;
    D_800F2E08[6] = (int)D_80010364;
    D_800F2E08[3] = (int)D_8001036C;
    D_800F2E08[7] = (int)D_80010384;
    D_800F4BD0[0] = (int)D_8001038C;
    D_800F4BD0[1] = (int)D_80010398;
    D_800F4BD0[2] = (int)D_800103A0;
    D_800F4BD0[3] = (int)D_800103AC;
    LoadCursor = CdDirectoryBuffer;
    D_800F4D40 = CdDirectoryBuffer;
    InitMenuArenas();
    k = 0xAF000;
    m = -1;
    i = 2;
    D_800F4CA8 = 0;
    D_800F4D44 = 0;
    D_800AC5A0 = CdDirectoryBuffer + k;
    r = D_800F2E30 + i * 0x2C;
    do {
        r[0x17] = m;
        i--;
        r -= 0x2C;
    } while (i >= 0);
    func_8001EEE4(k, i, r, m);
    func_8001F194();
    func_8006F5F0(D_800F3B28);
    func_8006F5F0(D_800F3B28 + 0x840);
    func_8006F5F0(D_800F2FE0);
    D_800F3B20 = D_800F3B28;
    func_80031DB0();
    func_800373DC(1);
    ResetGraph(3);
}
