#include "rr/overlay668_resource.h"
void ovl668_func_80114910(s32 arg0, RrStrCdFileInfo *arg1, u32 *arg2) {
    /* Existing PAL compiler frame reserves this area; it carries no data. */
    u8 reserved_stack10[8];
    /* Emit 0x0FFFFFFF mask in $3 before &D_800F4DB8 and offset math */
    register u32 mask __asm__("$3") = 0x0FFFFFFF;
    /* Hold &D_800F4DB8 in $2 across offset calculation */
    register RrStrArchiveAudioState *base __asm__("$2");
    /* Keep offset addition in $4 before loading archive2638 */
    register s32 offset __asm__("$4");
    RrStrArchiveEntry *entry;
    u32 v0;
    (void)reserved_stack10;
    __asm__("" : "=r"(mask) : "0"(mask));
    base = &D_800F4DB8;
    __asm__("" : "=r"(base) : "0"(base));
    offset = (s32)((u32)arg0 * 8U + 0x130U);
    __asm__("" : "=r"(offset) : "0"(offset));
    entry = (RrStrArchiveEntry *)(base->archive2638 + offset);
    func_8008C5A0((s32)((entry->position & mask) + base->sector2634), arg1->location.c);
    arg1->byteSize = entry->sectors << 11;
    *arg2 = entry->sectors;
    D_800AC650.locationBC = arg1->location;
    D_800AC650.currentC0 = D_800AC650.locationBC;
    v0 = (*arg2 / 6) - 20;
    D_800AC650.thresholdC8 = v0;
    D_800AC650.budgetCC = v0 * 2;
}
