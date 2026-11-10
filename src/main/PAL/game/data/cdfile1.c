#include "psyq/libc/exit.h"
/* RODATA 0x80010014 0x14 */
#include "types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"

/* Read the two-sector R4 directory, then convert relative boundaries to entries.
 * The disc supplies count + 1 valid offsets; sector arithmetic must fit s32. */
#include "rr/cd.h"
#include "psyq/libcd/cdfile.h"
#include "psyq/libcd/dsfinish.h"
#include "psyq/libcd/s_1.h"
#include "game/data/cdfile1.h"
#include "rr/course_choices.h"
#include "rr/cd_read_request.h"
#include "rr/cd_poll.h"
#include "rr/trig.h"
#include "rr/gpu.h"
#include "rr/gpu_scratch_types.h"

extern s32 CdPosToInt(void *arg0);
extern const char NowLoadingFmt[];
extern const char R4BinPath[];
extern const char StrFileNotFoundFmt[];
extern const char StrReadSectorFmt[];
#include "rr/cd_directory_buffer.h"
extern CdFileEntry CdFileTable[];
extern int LoadingTriangleAngle;
extern int LoadingTriangleFlip;
extern struct LoadingTriG3Packet {
    GpuDmaTag tag;
    int w4;
    short x0, y0;
    int c1;
    short x1, y1;
    int c2;
    short x2, y2;
} LoadingIndicatorTriBuf[];
extern struct LoadingTriG3Packet *ActiveLoadingTriPacket;

int GetCarFileIndex(int a0, int a1, int a2, int a3);
int GetR4FileIndex(int a0, int a1);
void DrawLoadingTriangle(void);

s32 GetCourseFileIndex(s32 arg0) {
    return arg0 * 2;
}

int GetCarFileIndex(int a0, int a1, int a2, int a3) {
    int v = ((a1 * 4 + a0) * 0x14) + a2;
    if (a3 == 0) return v + 0x12;
    return v + 0x152;
}

int GetR4FileIndex(int a0, int a1) {
    switch (a0) {
    case 1: return a1 + 0x292;
    case 2: return a1 + 0x2D9;
    case 3: return a1 + 0x384;
    case 4: return a1 + 0x397;
    case 0: return a1 + 0x3A0;
    }
}

void LoadR4Directory(void)
{
    CdlFILE file;
    s32 status;
    CdlFILE *found;
    /* ASM_BARRIER: retain the two-sector transfer count across CD calls. */
    register s32 sectors __asm__("$16");
    /* ASM_BARRIER: preserve the PAL diagnostic page across retries. */
    register u8 *message_page __asm__("$18");
    u32 count;
    /* ASM_BARRIER: retain the directory index independently of its cursor. */
    register u32 i __asm__("$17");
    s32 base, sector, next;
    u32 *directory;
    CdFileEntry *entry;
    DebugPrintStub(NowLoadingFmt,R4BinPath,CdDirectoryBuffer);
    found=CdSearchFile(&file,(char *)R4BinPath);
    message_page=(u8 *)0x80010000U;
    if (!found) {
        DebugPrintStub(StrFileNotFoundFmt,R4BinPath);
        PsyqExit(1);
    }
    sectors=2;
    /* ASM_BARRIER: keep the saved count and address opaque to constant folding. */
    __asm__("" : "+r"(sectors), "+r"(message_page));
    do {
        CdControl(2,file.pos,0);
        CdRead(sectors,(u8 *)CdDirectoryBuffer,0x80);
        do { status=CdReadSync(1,0); } while(status>0);
    } while(status);
    DebugPrintStub((const char *)(message_page+0x68),sectors);
    base=CdPosToInt(file.pos);
    directory=(u32 *)CdDirectoryBuffer;
    i=0;
    count=*directory++;
    sector=base+*directory++;
    entry=CdFileTable;
    while(i<count) {
        CdIntToPos(sector,entry->location.c);
        next=base+*directory++;
        entry->sectors=next-sector;
        sector=next;
        i++;
        entry++;
    }
}

void InitCdFileDirectory(void) {
    CdSetDebug(1);
    LoadR4Directory();
    LoadCursor = CdDirectoryBuffer;
}


void DrawLoadingTriangle(void)
{
    int a, v;
    a = LoadingTriangleAngle + 0x10;
    v = LoadingTriangleFlip ^ 1;
    LoadingTriangleFlip = v;
    LoadingTriangleAngle = a;
    ActiveLoadingTriPacket = &LoadingIndicatorTriBuf[v];
    ActiveLoadingTriPacket->x0 = (rsin(a) >> 7) + 0x50;
    ActiveLoadingTriPacket->y0 = (rcos(LoadingTriangleAngle) >> 7) + 0x80;
    ActiveLoadingTriPacket->x1 = (rsin(LoadingTriangleAngle + 0x555) >> 7) + 0x50;
    ActiveLoadingTriPacket->y1 = (rcos(LoadingTriangleAngle + 0x555) >> 7) + 0x80;
    ActiveLoadingTriPacket->x2 = (rsin(LoadingTriangleAngle - 0x555) >> 7) + 0x50;
    ActiveLoadingTriPacket->y2 = (rcos(LoadingTriangleAngle - 0x555) >> 7) + 0x80;
    ActiveLoadingTriPacket->c1 = 0xFF00;
    ActiveLoadingTriPacket->c2 = 0xFF0000;
    ActiveLoadingTriPacket->w4 = 0x300000FF;
    ActiveLoadingTriPacket->tag.bytes.length = 6;
    ActiveLoadingTriPacket->tag.bits.address = ((GpuDmaTag *)((GpuScratchPointers *)SCRATCHPAD)->order)->bits.address;
    ((GpuDmaTag *)((GpuScratchPointers *)SCRATCHPAD)->order)->bits.address = (unsigned long)ActiveLoadingTriPacket;
}
