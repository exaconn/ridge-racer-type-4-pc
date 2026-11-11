#ifndef RR_SELECTION_H
#define RR_SELECTION_H
#include "rr/save_slots.h"

#include "rr/layout.h"

#include "rr/selection_types.h"
#include "game/menu/menuwork.h"

typedef struct {
    u8 pad00[0x1E];
    u8 parameter;
} SelectionConfigView;

/* Prefix of SelectionState. History records follow this prefix at byte 0x20;
 * their writable extent is selected by the unsigned history index. */
typedef struct {
    s8 choices[4];
    u8 opaque04[0x10];
    u8 rank,index,byte16;
    s8 phase;
    u8 byte18,byte19,byte1A,byte1B;
    SelectionRow current;
} SelectionStatePrefix;
/* Alias of the rank byte at offset 0x14, read before its new value is stored. */
extern u8 MenuSelectionLockedFlag;
extern u8 CarSpecStatWeights[];
extern s8 CarSpecStatThresholds[];
/* Positive phases update rank/parameter and publish both history and rows.
 * History receives the old parameter; the displayed rows receive the new one.
 * Signed choices and group indices must select readable table addresses. */
void UpdateSelectionRank(void);

#include "game/data/selection.h"
void InitSelectionRows(void);

RR_ASSERT_SIZE(SelectionRow, 4);
RR_ASSERT_OFFSET(SelectionRow, parameter, 2);
RR_ASSERT_SIZE(SelectionRows32, 0x20);
RR_ASSERT_OFFSET(SelectionRows32, rows, 4);
RR_ASSERT_OFFSET(SelectionConfigView, parameter, 0x1E);

RR_ASSERT_SIZE(SelectionStatePrefix, 0x20);
RR_ASSERT_OFFSET(SelectionStatePrefix, rank, 0x14);
RR_ASSERT_OFFSET(SelectionStatePrefix, phase, 0x17);
RR_ASSERT_OFFSET(SelectionStatePrefix, current, 0x1C);
#endif
