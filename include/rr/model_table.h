#ifndef RR_MODEL_TABLE_H
#define RR_MODEL_TABLE_H

#include "rr/car_render_types.h"

/* A bank owns one vertex pool, one normal pool and a counted array of
 * command streams. Each relocation slot holds an encoded signed32 displacement
 * before registration and a complete pointer afterwards. Native owners allocate
 * these actual slots rather than treating a PAL word array as native pointers. */
typedef union {u32 relative; const Vec3s8 *pointer;} RroBankVertices;
typedef union {u32 relative; const u32 *pointer;} RroBankCommand;
typedef struct {
    u32 streamCount;
    RroBankVertices vertices, normals;
#if defined(__GNUC__) && __GNUC__ < 3
    RroBankCommand streams[0];
#else
    RroBankCommand streams[];
#endif
} RroModelBank;

/* Signed displacements are serialized as PAL words. Keep the complete base
 * address on an LP64 host; PAL's unsigned address addition wraps modulo32. */
typedef char rr_model_bank_address_width[(sizeof(unsigned long)==sizeof(void*))?1:-1];

#if defined(mips)
RR_ASSERT_SIZE(RroBankVertices, 4);
RR_ASSERT_SIZE(RroBankCommand, 4);
RR_ASSERT_SIZE(RroModelBank, 12);
RR_ASSERT_OFFSET(RroModelBank, vertices, 4);
RR_ASSERT_OFFSET(RroModelBank, normals, 8);
RR_ASSERT_OFFSET(RroModelBank, streams, 12);
#endif

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

/* RelocateModelTable publishes entries, immediately after the signed count.
 * A native decoded table keeps the same owner with full-width mesh pointers.
 * The file loader's encoded 32-bit offsets are a separate wire representation. */
typedef struct {
    s32 count;
    /* The original GCC predates C99 flexible array member syntax. */
#if defined(__GNUC__) && __GNUC__ < 3
    RroEffectMesh entries[0];
#else
    RroEffectMesh entries[];
#endif
} RroModelTable;

#if defined(mips)
RR_ASSERT_SIZE(RroModelTable, 4);
RR_ASSERT_OFFSET(RroModelTable, entries, 4);
#endif

#endif
