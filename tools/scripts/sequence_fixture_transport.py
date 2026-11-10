"""Fixture-only PAL scalar/field transport for complete native sequence rows.

The existing wire records, cases and oracles stay unchanged. Production bodies
run on complete SequenceChannel objects; untouched native pointer cells and
padding are guarded. Pointer arguments and table roots map by their owning row.
"""
from pathlib import Path
import re



_LIBRARY_WIRE = r"""
typedef struct {
    u32 c0;
    s32 c4;
    u32 c8; /* Saved loop cursor, restored by controller 30. */
    s32 cC;
    u32 p10; /* Stream marker checked before repeat/reset dispatch. */
    u8 b14, b15, b16;
    u8 b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f;
    /* b20 is compared as signed; b21 increments modulo 256. */
    s8 b20;
    u8 b21;
    s8 b22[2];
    char p24[2];
    s8 b26;
    u8 b27[0x10];
    u8 b37[0x10];
    char p47[1];
    s16 h48[8];
    u16 h58[4];
    u16 h60[0x10];
    s16 h80;
    char p82[6];
    s32 w88;
    s32 w8c;
    s32 w90;
    s32 w94;
    s32 w98;
    s32 w9c;
    s32 wa0, wa4, wa8, wac;
} SequenceWire176;
"""

_VALUE_WIRE = r"""
typedef struct {
    u8 pad00[0x17];
    u8 unk17;
    u8 pad18[0x26 - 0x18];
    s8 unk26;
    u8 pad27[0x48 - 0x27];
    s16 rampDelta48, rampApplied4A;
    u8 pad4C[0x58 - 0x4C];
    u16 value58, value5A;
    u16 volume5C, volume5E;
    s16 unk60[16];
    u8 pad80[0x98 - 0x80];
    s32 mode98, rampDuration9C, rampElapsedA0;
    u8 padA4[0xB0 - 0xA4];
} SequenceWire176;
"""

def wire_definition(kind="library"):
    """Original 176-byte scalar oracle protocol; never used as a native owner."""
    assert kind in ("library", "value")
    return _LIBRARY_WIRE if kind == "library" else _VALUE_WIRE


def field_only_body(body):
    # These original fixtures exercise scalar handlers. Extending them to a
    # pointer-consuming handler requires an explicit new pointer-field adapter.
    assert not re.search(r'->(?:c0|c4|c8|cC|p10)\b', body), body
    return body


def transport(wire_rows='test_records', wire_table='test_table', banks=9, rows=17,
              table_size=0x4100):
    return r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/sequence_channel.h"
_Static_assert(sizeof(void *) == 8, "full native fixture");
_Static_assert(sizeof(SequenceWire176) == 176, "original PAL wire model");
_Static_assert(sizeof(SequenceChannel) == 200, "complete native sequence row");
_Static_assert(offsetof(SequenceChannel,b14) == 5*sizeof(u8 *), "five native pointers");

static struct {
    u32 before[4];
    SequenceChannel values[BANK_COUNT][ROW_COUNT];
    u32 after[4];
} sequence_native;
static SequenceChannel *sequence_native_table[TABLE_COUNT];
static u8 sequence_pointer_targets[BANK_COUNT*ROW_COUNT][5];
static int sequence_initialized;
static size_t sequence_first_record,sequence_record_count=BANK_COUNT*ROW_COUNT;
static size_t sequence_first_slot,sequence_slot_count=TABLE_COUNT;
static SequenceWire176 *sequence_wire_record(size_t index) {
    assert(index<BANK_COUNT*ROW_COUNT);
    return (SequenceWire176 *)((u8 *)&WIRE_ROWS+index*sizeof(SequenceWire176));
}
static void *sequence_native_pointer(const void *pointer) {
    uintptr_t value=(uintptr_t)pointer,first=(uintptr_t)&WIRE_ROWS;
    if(value>=first && value-first<sizeof(WIRE_ROWS)) {
        size_t offset=value-first,index=offset/sizeof(SequenceWire176);
        size_t field=offset%sizeof(SequenceWire176);
        assert(field>=20 && "extend the pointer-field transport explicitly");
        return (u8 *)&sequence_native.values+index*sizeof(SequenceChannel)
             +offsetof(SequenceChannel,b14)+field-20;
    }
    return (void *)pointer;
}
static void *sequence_wire_pointer(const void *pointer) {
    uintptr_t value=(uintptr_t)pointer,first=(uintptr_t)&sequence_native.values[0][0];
    if(value>=first && value-first<sizeof(sequence_native.values)) {
        size_t offset=value-first,index=offset/sizeof(SequenceChannel);
        size_t field=offset%sizeof(SequenceChannel);
        assert(field>=offsetof(SequenceChannel,b14) && field<offsetof(SequenceChannel,wac)+4);
        return (u8 *)&WIRE_ROWS+index*sizeof(SequenceWire176)+20
             +field-offsetof(SequenceChannel,b14);
    }
    return (void *)pointer;
}
static SequenceChannel *sequence_native_root(const SequenceWire176 *pointer) {
    uintptr_t first=(uintptr_t)&WIRE_ROWS,value=(uintptr_t)pointer;
    if(!pointer)return 0;
    assert(value>=first && value-first<sizeof(WIRE_ROWS));
    assert((value-first)%sizeof(SequenceWire176)==0);
    {size_t index=(value-first)/sizeof(SequenceWire176);
     return &sequence_native.values[index/ROW_COUNT][index%ROW_COUNT];}
}
static SequenceWire176 *sequence_wire_root(const SequenceChannel *pointer) {
    uintptr_t first=(uintptr_t)&sequence_native.values[0][0],value=(uintptr_t)pointer;
    if(!pointer)return 0;
    assert(value>=first && value-first<sizeof(sequence_native.values));
    assert((value-first)%sizeof(SequenceChannel)==0);
    {size_t index=(value-first)/sizeof(SequenceChannel);
     return sequence_wire_record(index);}
}
static void sequence_initialize(void) {
    size_t bank,row;
    if(sequence_initialized)return;
    memset(&sequence_native,0xA7,sizeof(sequence_native));
    assert((uintptr_t)&sequence_native>UINT32_MAX);
    assert((uintptr_t)sequence_pointer_targets>UINT32_MAX);
    for(bank=0;bank<BANK_COUNT;bank++)for(row=0;row<ROW_COUNT;row++) {
        SequenceChannel *record=&sequence_native.values[bank][row];
        u8 *targets=sequence_pointer_targets[bank*ROW_COUNT+row];
        record->c0=targets;record->c4=targets+1;record->c8=targets+2;
        record->cC=targets+3;record->p10=targets+4;
    }
    sequence_initialized=1;
}
static void sequence_guard(void) {
    size_t bank,row,index;
    const u8 *a=(const u8 *)sequence_native.before;
    const u8 *z=(const u8 *)sequence_native.after;
    for(index=0;index<16;index++)assert(a[index]==0xA7 && z[index]==0xA7);
    for(index=sequence_first_record;index<sequence_first_record+sequence_record_count;index++) {
        size_t tail;
        bank=index/ROW_COUNT;row=index%ROW_COUNT;
        SequenceChannel *record=&sequence_native.values[bank][row];
        u8 *targets=sequence_pointer_targets[index];
        assert(record->c0==targets && record->c4==targets+1 && record->c8==targets+2);
        assert(record->cC==targets+3 && record->p10==targets+4);
        for(tail=offsetof(SequenceChannel,wac)+4;tail<sizeof(*record);tail++)
            assert(((u8 *)record)[tail]==0xA7);
    }
}
static void sequence_guard_all(void) {
    size_t first=sequence_first_record,count=sequence_record_count;
    sequence_initialize();
    sequence_first_record=0;sequence_record_count=BANK_COUNT*ROW_COUNT;
    sequence_guard();sequence_first_record=first;sequence_record_count=count;
}
static void sequence_select(size_t index,size_t slot) {
    assert(index<BANK_COUNT*ROW_COUNT && slot<TABLE_COUNT);
    sequence_first_record=index;sequence_record_count=1;
    sequence_first_slot=slot;sequence_slot_count=1;
}
static void sequence_import(void) {
    size_t index;
    sequence_initialize();sequence_guard();
    for(index=sequence_first_record;index<sequence_first_record+sequence_record_count;index++)
        memcpy((u8 *)&sequence_native.values[index/ROW_COUNT][index%ROW_COUNT]+offsetof(SequenceChannel,b14),
               (const u8 *)sequence_wire_record(index)+20,156);
    for(index=sequence_first_slot;index<sequence_first_slot+sequence_slot_count;index++)
        sequence_native_table[index]=sequence_native_root(WIRE_TABLE[index]);
}
static void sequence_export(void) {
    size_t index;
    sequence_guard();
    for(index=sequence_first_record;index<sequence_first_record+sequence_record_count;index++)
        memcpy((u8 *)sequence_wire_record(index)+20,
               (const u8 *)&sequence_native.values[index/ROW_COUNT][index%ROW_COUNT]+offsetof(SequenceChannel,b14),156);
    for(index=sequence_first_slot;index<sequence_first_slot+sequence_slot_count;index++)
        WIRE_TABLE[index]=sequence_wire_root(sequence_native_table[index]);
}
'''.replace(
        'BANK_COUNT', str(banks)).replace('ROW_COUNT', str(rows)).replace(
        'TABLE_COUNT', str(table_size)).replace('WIRE_ROWS', wire_rows).replace(
        'WIRE_TABLE', wire_table)


def current_body(root, address):
    """Extract the current implementation rather than its historical view."""
    from c_functions import definition_source, source_owners, name_at
    name = name_at(address)
    owner = source_owners(root / 'src/main/PAL')[name]
    return definition_source(owner.read_text(), name)


def compile_library(root, directory, source, name='functions'):
    """Build an actual native owner with sanitizer failures made fatal."""
    import subprocess
    path = directory / (name + '.c')
    path.write_text(source)
    result = subprocess.run(['cc', '-shared', '-fPIC', '-O2',
                             '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                             '-I', str(root / 'include'), str(path),
                             '-o', str(directory / (name + '.so'))],
                            capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
