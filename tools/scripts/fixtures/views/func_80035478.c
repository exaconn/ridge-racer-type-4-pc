#include "rr/player_status.h"

/* legacy typing of shared globals in this part of the unit */
#define LinkHostFlag (*(short (*))&LinkHostFlag)

#undef LinkHostFlag

/* legacy typing of shared globals in this part of the unit */
#define LinkEntryState (*(char (*)[])&LinkEntryState)

#undef LinkEntryState

/* legacy typing of shared globals in this part of the unit */
#define LinkHostFlag (*(s16 (*))&LinkHostFlag)

#undef LinkHostFlag

/* legacy typing of shared globals in this part of the unit */
#define ModeFrame (*(unsigned int (*))&ModeFrame)

#define ModeIndex (*(unsigned short (*))&ModeIndex)

#define SubmodeState (*(unsigned short (*))&SubmodeState)

#undef ModeFrame

#undef ModeIndex

#undef SubmodeState

/* legacy typing of shared globals in this part of the unit */
#define SubmodeState (*(unsigned short (*))&SubmodeState)

#define ModeFrame (*(unsigned int (*))&ModeFrame)

#undef SubmodeState

#undef ModeFrame

/* legacy typing of shared globals in this part of the unit */
#define SubmodeState (*(unsigned short (*))&SubmodeState)

#undef SubmodeState

/* legacy typing of shared globals in this part of the unit */
#define ModeFrame (*(u32 (*))&ModeFrame)

#undef ModeFrame

/* legacy typing of shared globals in this part of the unit */
#define SelectionState (*(struct w3g2s (*))&SelectionState)

#undef SelectionState

void DrawLinkEntryScreen(s32 mode) {
    u8 text[4];
    s32 remaining,row,statusY,waitY,labelY;
    s8 status;
    unsigned long address;

    if (mode>=0) {
        /* Integer cursor retains native pointer width. Only the four in-owner
         * addresses are converted back to pointers; the final decrement is dead. */
        row=3;
        address=(unsigned long)&text[3];
        do {
            *(u8*)address=0;
            row--;
            address--;
        } while (row>=0);
        row=0;
        do {
            labelY=row*194+64;
            waitY=row*194+150;
            statusY=row*194+120;
            text[0]=row+((s32)((u32)(LinkHostFlag^1)<<16)>>15)+49;
            DrawBigFontString(12,labelY,StrPlayerLabel);
            DrawBigFontString(88,labelY,text);
            status=LinkEntryState[1-mode][row];
            switch (status) {
            case 0:
                DrawBigFontString(60,statusY,StrNowAcceptingEntries);
                break;
            case 1:
                DrawBigFontString(60,statusY,EntryAcceptedStr);
                DrawBigFontString(60,waitY,StrStarPleaseWait);
                break;
            case -1:
                DrawBigFontString(60,statusY,StrNoPlayer);
                break;
            }
            row++;
        } while (row<2);
        row=RaceTimeRemaining/50;
        text[0]=row/10+48;
        text[1]=row%10+48;
        DrawBigFontString(44,244,text);
        DrawBigFontString(32,244,SecondsUntilCloseText);
        remaining=(s32)(500U-(u32)RaceTimeRemaining);
        DrawBigFontString(((s32)((u32)remaining<<8)/500)+32,224,StrLinkProgressBarGlyph);
    }
}
