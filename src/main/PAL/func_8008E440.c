/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern char D_8011095C[];
extern s32 func_80096404(const char *, const char *);
s32 func_8008E440(s32 key, const char *name) {
    s32 i = 0;
    register char *entry_name;
    /* ASM_BARRIER: Retain the original table-offset register across comparisons. */
    register s32 offset __asm__("$17");
    /* ASM_BARRIER: Keep independently initialized scan count and byte offset without folding their zeros. */
    __asm__("" : "=r"(i) : "0"(i) : "$18");
    entry_name = D_8011095C;
    offset = 0;
    do {
        register u8 *page;
        s32 entry_key;
        /* PAL mapping: page + offset + 0x954 addresses D_80110954[i]. */
        page = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL page without folding its table offset into a macro load. */
        __asm__("" : "=r"(page) : "0"(page));
        page += offset;
        entry_key = ((RrLibNamedEntry *)(page + 0x954))->key;
        if (entry_key == 0) break;
        if (entry_key == key) {
            if (func_80096404(name, entry_name) == 0) return i + 1;
        }
        entry_name += 0x2C;
        offset += 0x2C;
        i++;
    } while (i < 128);
    return -1;
}
