#ifndef RR_GAME_DATA_SAVETYPE_H
#define RR_GAME_DATA_SAVETYPE_H

#include "rr/save_progress.h"
#include "rr/runtime_settings.h"
#include "rr/course_choice_types.h"

/* Layout of the save buffer shared by the save builder and the save writer. */

typedef struct {
    s16 halves00[8];
    s16 checksum10;
    s16 halves12[13];
} Struct2C;

typedef struct {
    int w[3];
} StructC;

typedef struct {
    int w[4];
} Struct10;

typedef struct {
    unsigned char header00[0x280];
    SaveProgressRecord7C f280;
    RuntimeSettings f2FC;
    CourseChoice4 f33C[9];
    Struct2C f360[3];
    int f3E4[2][8][3];
    Struct10 f4A4[2][8][5];
    Struct10 f9A4[2][8][5];
    int fEA4[0x210];
    unsigned char reserved16E4[0x3C0];
    int f1AA4;
} SaveBuf;

typedef struct {
    unsigned char header00[0x280];
    int f280;
    unsigned char reserved284[0x7C];
    unsigned char f300[2][0x840];
} SaveBuf11;

typedef struct {
    unsigned char b[0x840];
} Struct840;

#endif
