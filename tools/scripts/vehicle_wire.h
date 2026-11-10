#ifndef RR_TEST_VEHICLE_WIRE_H
#define RR_TEST_VEHICLE_WIRE_H

/* Test-only PAL serialization. The production owner keeps real native pointers.
 * Console addresses are resolved to complete native pointees in this fixture's
 * address map; they are never stored in a truncated native pointer member. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/race_types.h"

enum {RR_VEHICLE_WIRE_SIZE=0x320, RR_VEHICLE_WIRE_CONTROL=0x1F4};
typedef struct {u32 address; VehicleHandlingPrefix info;} TestHandlingBinding;
typedef struct {u32 address; CarRecord record;} TestRecordBinding;
static TestHandlingBinding rr_test_handling[128];
static TestRecordBinding rr_test_records[128];
static unsigned rr_test_handling_count, rr_test_record_count;

static inline void rr_vehicle_wire_reset(void) {rr_test_handling_count=rr_test_record_count=0;}

static inline CarRecord *rr_vehicle_record_resolve(u32 address) {
    unsigned i;
    if (!address) return NULL;
    for (i=0;i<rr_test_record_count;i++)
        if (rr_test_records[i].address==address) return &rr_test_records[i].record;
    assert(rr_test_record_count<128);
    i=rr_test_record_count++;
    rr_test_records[i].address=address;
    memset(&rr_test_records[i].record,0xA5,sizeof(CarRecord));
    return &rr_test_records[i].record;
}

static inline u32 rr_vehicle_record_address(const CarRecord *pointer) {
    unsigned i;
    if (!pointer) return 0;
    for (i=0;i<rr_test_record_count;i++)
        if (pointer==&rr_test_records[i].record) return rr_test_records[i].address;
    assert(!"Record pointer is missing from the fixture address map");
    return 0;
}

static inline VehicleHandlingPrefix *rr_vehicle_wire_resolve(u32 address) {
    unsigned i;
    if (!address) return NULL;
    for (i=0;i<rr_test_record_count;i++)
        if (rr_test_records[i].address+0x1C8U==address)
            return &rr_test_records[i].record.handling1C8;
    for (i=0;i<rr_test_handling_count;i++)
        if (rr_test_handling[i].address==address) return &rr_test_handling[i].info;
    assert(rr_test_handling_count<128);
    i=rr_test_handling_count++;
    rr_test_handling[i].address=address;
    memset(&rr_test_handling[i].info,0xA5,sizeof(VehicleHandlingPrefix));
    assert(sizeof(void*)==4 || (uintptr_t)&rr_test_handling[i].info>UINT32_MAX);
    return &rr_test_handling[i].info;
}

static inline u32 rr_vehicle_wire_address(const VehicleHandlingPrefix *pointer) {
    unsigned i;
    if (!pointer) return 0;
    for (i=0;i<rr_test_record_count;i++)
        if (pointer==&rr_test_records[i].record.handling1C8)
            return rr_test_records[i].address+0x1C8U;
    for (i=0;i<rr_test_handling_count;i++)
        if (pointer==&rr_test_handling[i].info) return rr_test_handling[i].address;
    assert(!"Handling pointer is missing from the fixture address map");
    return 0;
}

static inline void rr_vehicle_wire_load(RaceVehicle *vehicle, const u8 bytes[0x320]) {
    u32 address;
    memset(vehicle,0xA5,sizeof(*vehicle));
    memcpy(vehicle,bytes,0x1F4);
    memcpy(&vehicle->control,bytes+0x1F4,0x18);
    memcpy(&address,bytes+0x20C,4);
    vehicle->control.record18=rr_vehicle_record_resolve(address);
    memcpy(&address,bytes+0x210,4);
    vehicle->control.handling1C=rr_vehicle_wire_resolve(address);
    memcpy((u8*)&vehicle->control+offsetof(RaceVehicleControl,reserved20),bytes+0x214,0x10C);
}

static inline void rr_vehicle_wire_padding_value(const RaceVehicle *vehicle, u8 expected) {
    const u8 *bytes=(const u8*)vehicle;
    size_t i;
    for (i=0x1F4;i<offsetof(RaceVehicle,control);i++) assert(bytes[i]==expected);
    for (i=offsetof(RaceVehicle,control.reserved20)+0x10C;
         i<sizeof(*vehicle);i++) assert(bytes[i]==expected);
}

static inline void rr_vehicle_wire_store_fields(u8 bytes[0x320], const RaceVehicle *vehicle) {
    u32 address=rr_vehicle_wire_address(vehicle->control.handling1C);
    memcpy(bytes,vehicle,0x1F4);
    memcpy(bytes+0x1F4,&vehicle->control,0x18);
    {u32 record=rr_vehicle_record_address(vehicle->control.record18);memcpy(bytes+0x20C,&record,4);}
    memcpy(bytes+0x210,&address,4);
    memcpy(bytes+0x214,(const u8*)&vehicle->control+offsetof(RaceVehicleControl,reserved20),0x10C);
}

static inline void rr_vehicle_wire_store(u8 bytes[0x320], const RaceVehicle *vehicle) {
    rr_vehicle_wire_padding_value(vehicle,0xA5);
    rr_vehicle_wire_store_fields(bytes,vehicle);
}

_Static_assert(offsetof(RaceVehicle,contactFlags1F0)==0x1F0,"fixed vehicle prefix");
_Static_assert(offsetof(RaceVehicleControl,metric12A)+2-
               offsetof(RaceVehicleControl,reserved20)==0x10C,"fixed control tail");
_Static_assert(sizeof(((RaceVehicleControl*)0)->handling1C)==sizeof(void*),"real pointer");

#endif
