#include "rr/overlay668_resource.h"
void ovl668_func_80114910(s32 arg0, StrCdFileInfo *arg1, u32 *arg2) {
    /* Existing PAL compiler frame reserves this area; it carries no data. */
    u8 reserved_stack10[8];
    /* Emit 0x0FFFFFFF mask in $3 before &SoundState and offset math */
    u32 mask = 0x0FFFFFFF;
    /* Hold &SoundState in $2 across offset calculation */
    AudioState *base;
    /* Keep offset addition in $4 before loading archive2638 */
    s32 offset;
    StrArchiveEntry *entry;
    u32 v0;
    (void)reserved_stack10;
    base = &SoundState;
    offset = (arg0 * 8U + 0x130U);
    entry = (StrArchiveEntry *)(base->streamBanks[0].table + offset);
    CdIntToPos(((entry->position & mask) + base->streamBanks[0].sector), arg1->location.c);
    arg1->byteSize = entry->sectors << 11;
    *arg2 = entry->sectors;
    RaceContext.locationBC = arg1->location;
    RaceContext.currentC0 = RaceContext.locationBC;
    v0 = (*arg2 / 6) - 20;
    RaceContext.thresholdC8 = v0;
    RaceContext.budgetCC = v0 * 2;
}
