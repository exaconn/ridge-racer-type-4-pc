#ifndef RR_RELATIVE_FILE_H
#define RR_RELATIVE_FILE_H
#include <stddef.h>
#include "rr/layout.h"
#include "rr/course_view_types.h"

/* Serialized offsets in R4.BIN files and relative archives.
 *
 * The PS1 loaders "relocated" a file in place: every 32-bit offset word was
 * replaced by base+offset, so later code read the word back as an address.
 * On the host a 4-byte slot cannot hold a pointer, so the loaded buffer is
 * never rewritten. The rule for every loader and consumer is:
 *
 *   - wire structs keep their offsets as u32 (signed displacement from the
 *     file/record base the original relocation used);
 *   - code resolves an offset at use with RrWireAddress(base, offset), or the
 *     loader publishes the resolved address in an ordinary host pointer
 *     (a global, or a host-side table it owns, see RelocateOffsetTable);
 *   - a 64-bit pointer is never stored into a 32-bit serialized field.
 */
static inline u8 *RrWireAddress(const void *base, u32 offset) {
    return (u8 *)base + (ptrdiff_t)(s32)offset;
}
/* Offset word `index` of a u32 offset table at base, resolved against base. */
static inline u8 *RrWireTableAddress(const void *base, size_t index) {
    return RrWireAddress(base, ((const u32 *)base)[index]);
}

/* Course .rel archive header: segment count, three record tables and sixteen
 * section offsets, all relative to the header. count*8 u32 segment-view
 * offsets follow the header (eight yaw octants per segment). */
typedef struct {
    u32 count;
    u32 first[3];
    u32 offsets[16];
} RelativeHeader;
RR_ASSERT_SIZE(RelativeHeader,0x50);
RR_ASSERT_OFFSET(RelativeHeader,first,4);
RR_ASSERT_OFFSET(RelativeHeader,offsets,0x10);

extern RelativeHeader *TrackRelFileHeaderPtr;
extern s32 RelFileSegmentCount;
/* Wire offset words (relative to TrackRelFileHeaderPtr); resolve them with
 * RelFileSegmentView. */
extern u32 *RelFileSegmentViewSlots;
extern u8 *TrackRelSecondTablePtr;
extern CourseViewRecord *RelFileViewRecords;
extern u8 *RelFileSection00;
extern u8 *RelFileSection01;
extern u8 *TrackRelSegment2Ptr;
extern u8 *RelFileOffsetPtr3;
extern u8 *TrackRelSegment4Ptr;
extern u8 *RelFileSection05;
extern u8 *RelFileOffsetPtr6;
extern u8 *RelFileOffsetPtr7;
extern u8 *TrackRelSegment8Ptr;
extern u8 *RelFileOffsetPtr9;
extern u8 *TrackCollisionGridPtr;
extern u8 *RelFileSection11;
extern u8 *RelFileOffsetPtr12;
extern u8 *RelFileOffsetPtr13;
extern u8 *TrackSectionBoundsPtr;
extern u8 *RelFileSection15;

/* Kept for existing callers: offset resolved against the archive header. */
static inline u8 *rr_relative_file_address(const void *base, u32 offset) {
    return RrWireAddress(base, offset);
}

/* Segment-view slot `index` (segment*8 + yaw octant) of the current archive. */
static inline u8 *RelFileSegmentView(size_t index) {
    return RrWireAddress(TrackRelFileHeaderPtr, RelFileSegmentViewSlots[index]);
}

/* Publishes the archive header, its section addresses and the segment-view
 * slot table. The image is left unmodified (see above). */
void RelocateRelativeFile(RelativeHeader *image);
#endif
