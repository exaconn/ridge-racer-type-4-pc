#ifndef RR_MODEL_TABLE_H
#define RR_MODEL_TABLE_H

#include "rr/car_render_types.h"

/* A registered model bank: one vertex pool, one normal pool and a counted
 * array of command streams, as host pointers into the loaded bank image.
 * RegisterModelBank (gfx/model.c) decodes the serialized RroModelBankWire
 * (below) into this loader-owned host struct and publishes it in
 * ModelBankTable[slot]; the loaded image is not modified. */
typedef struct {const Vec3s8 *pointer;} RroBankVertices;
typedef struct {const u32 *pointer;} RroBankCommand;
typedef struct RroModelBank {
    u32 streamCount;
    RroBankVertices vertices, normals;
    u8 *image; /* serialized bank this was decoded from */
#if defined(__GNUC__) && __GNUC__ < 3
    RroBankCommand streams[0];
#else
    RroBankCommand streams[];
#endif
} RroModelBank;
enum { RRO_MODEL_BANK_SLOTS = 16 };

#if defined(mips)
RR_ASSERT_SIZE(RroBankVertices, 4);
RR_ASSERT_SIZE(RroBankCommand, 4);
RR_ASSERT_SIZE(RroModelBank, 12);
RR_ASSERT_OFFSET(RroModelBank, vertices, 4);
RR_ASSERT_OFFSET(RroModelBank, normals, 8);
RR_ASSERT_OFFSET(RroModelBank, streams, 12);
#endif

/* Serialized bank as loaded from R4.BIN: stream count, vertex and normal pool
 * offsets and streamCount command-stream offsets, all u32 displacements from
 * the bank start (resolve with RrWireAddress, rr/relative_file.h). */
typedef struct {
    u32 streamCount;
    u32 vertices, normals;
#if defined(__GNUC__) && __GNUC__ < 3
    u32 streams[0];
#else
    u32 streams[];
#endif
} RroModelBankWire;
RR_ASSERT_SIZE(RroModelBankWire, 12);

/* On disk each mesh is three 32-bit words. Relocation changes the vertex and
 * command offsets into PAL address words; it does not decode native pointers. */
enum {
    RRO_MODEL_VERTEX_WORD = 0,
    RRO_MODEL_COMMAND_WORD = 2,
    RRO_MODEL_ENTRY_WORDS = 3
};

typedef struct {
    s32 count;
#if defined(__GNUC__) && __GNUC__ < 3
    u32 words[0];
#else
    u32 words[];
#endif
} RroModelTableWire;

RR_ASSERT_SIZE(RroModelTableWire, 4);
RR_ASSERT_OFFSET(RroModelTableWire, words, 4);

/* Host mesh table. The serialized form is RroModelTableWire: a signed count
 * followed by count {vertexOffset, vertexCount, commandOffset} u32 triples,
 * offsets relative to the table start. RelocateOffsetTable (data/cdfile2.c)
 * decodes a wire table into a loader-owned RroModelTable with full host
 * pointers into the loaded file and returns it; the file bytes are left
 * unchanged. Consumers use the returned table, never the wire words. */
typedef struct RroModelTable {
    s32 count;
    /* The original GCC predates C99 flexible array member syntax. */
#if defined(__GNUC__) && __GNUC__ < 3
    RroEffectMesh entries[0];
#else
    RroEffectMesh entries[];
#endif
} RroModelTable;

/* Decode a wire mesh table (see above). The result stays valid until the
 * wire bytes it was decoded from are decoded again or overlapped by another
 * decoded table. */
RroModelTable *RelocateOffsetTable(const void *wire);

#if defined(mips)
RR_ASSERT_SIZE(RroModelTable, 4);
RR_ASSERT_OFFSET(RroModelTable, entries, 4);
#endif

#endif
