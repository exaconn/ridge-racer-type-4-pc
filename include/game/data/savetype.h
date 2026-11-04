#ifndef RR_GAME_DATA_SAVETYPE_H
#define RR_GAME_DATA_SAVETYPE_H

/* Layout of the save buffer shared by the save builder and the save writer. */

typedef struct {
    int w[31];
} Struct7C;

typedef struct {
    int w[16];
} Struct40;

typedef struct {
    char pad0[0x10];
    short f10;
    char pad12[0x1A];
} Struct2C;

typedef struct {
    char b[4];
} Struct4;

typedef struct {
    char pad0[0x24];
    Struct4 f24[9];
} Struct48;

typedef struct {
    int w[3];
} StructC;

typedef struct {
    int w[4];
} Struct10;

typedef struct {
    unsigned char pad0[0x280];
    Struct7C f280;
    Struct40 f2FC;
    Struct4 f33C[9];
    Struct2C f360[3];
    int f3E4[2][8][3];
    Struct10 f4A4[2][8][5];
    Struct10 f9A4[2][8][5];
    int fEA4[0x210];
    unsigned char pad16E4[0x3C0];
    int f1AA4;
} SaveBuf;

typedef struct {
    unsigned char pad0[0x280];
    int f280;
    unsigned char pad284[0x7C];
    unsigned char f300[2][0x840];
} SaveBuf11;

typedef struct {
    unsigned char b[0x840];
} Struct840;

#endif
