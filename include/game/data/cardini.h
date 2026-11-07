#ifndef RR_GAME_DATA_CARDINI_H
#define RR_GAME_DATA_CARDINI_H

#include "types.h"

void CardSystemInit();
s32 GetHwCardEvent(void);
int CountCardFiles(int a, int b);
int CardFreeBlocks(int a0);
int CardHasFile(int a0, unsigned char *a1);
int CardReadFile();
int CardReadFileAt(int a0, int a1, int a2, int a3, int a4, int a5);
int CardWriteFile();
int CardUpdate(int p, char *a1);

#endif /* RR_GAME_DATA_CARDINI_H */
