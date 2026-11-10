"""PAL/native vehicle serialization for service-trace fixtures.

Keep console offsets in the independent MIPS oracle. Native arrays retain their
real stride and pointers; only fixture I/O passes through the explicit wire map.
"""
import ctypes as C

C_BRIDGE = r'''
#include "vehicle_wire.h"
size_t rr_test_vehicle_size(void) {return sizeof(RaceVehicle);}
size_t rr_test_control_offset(void) {return offsetof(RaceVehicle,control);}
size_t rr_test_record_pointer_offset(void) {return offsetof(RaceVehicle,control.record18);}
size_t rr_test_handling_pointer_offset(void) {return offsetof(RaceVehicle,control.handling1C);}
size_t rr_test_control_tail_offset(void) {return offsetof(RaceVehicle,control.reserved20);}
void rr_test_vehicle_reset(void) {rr_vehicle_wire_reset();}
void rr_test_vehicle_load(void *owner, const void *wire, unsigned count) {
    unsigned i;RaceVehicle *v=owner;const u8 *p=wire;
    for(i=0;i<count;i++) rr_vehicle_wire_load(v+i,p+i*0x320);
}
void rr_test_vehicle_store(void *wire, const void *owner, unsigned count) {
    unsigned i;const RaceVehicle *v=owner;u8 *p=wire;
    for(i=0;i<count;i++) rr_vehicle_wire_store(p+i*0x320,v+i);
}
'''


class NativeVehicles:
    def __init__(self, library):
        self.lib = library
        for name in ('vehicle_size', 'control_offset', 'record_pointer_offset', 'handling_pointer_offset', 'control_tail_offset'):
            function = getattr(library, 'rr_test_'+name)
            function.restype = C.c_size_t
            setattr(self, name, function())
        library.rr_test_vehicle_load.argtypes = [C.c_void_p, C.c_void_p, C.c_uint]
        library.rr_test_vehicle_store.argtypes = [C.c_void_p, C.c_void_p, C.c_uint]
        self.banks = []

    def add(self, native, address, count, guard=0):
        self.banks.append((native, address, count, guard))

    def reset(self):
        self.lib.rr_test_vehicle_reset()

    def to_native(self, address):
        for native, base, count, guard in self.banks:
            offset = address-base
            if -guard <= offset < 0:
                return native+offset
            if count*800 <= offset < count*800+guard:
                return native+count*self.vehicle_size+offset-count*800
            if 0 <= offset < count*800:
                index, field = divmod(offset, 800)
                if field < 0x1F4:
                    actual = field
                elif field < 0x20C:
                    actual = self.control_offset+field-0x1F4
                elif field < 0x210:
                    actual = self.record_pointer_offset+field-0x20C
                elif field < 0x214:
                    actual = self.handling_pointer_offset+field-0x210
                else:
                    actual = self.control_tail_offset+field-0x214
                return native+index*self.vehicle_size+actual
        return None

    def to_pal(self, pointer):
        for native, base, count, guard in self.banks:
            offset = pointer-native
            if -guard <= offset < 0:
                return base+offset
            if count*self.vehicle_size <= offset < count*self.vehicle_size+guard:
                return base+count*800+offset-count*self.vehicle_size
            if not 0 <= offset < count*self.vehicle_size:
                continue
            index, field = divmod(offset, self.vehicle_size)
            if field < 0x1F4:
                actual = field
            elif self.control_offset <= field < self.control_offset+0x18:
                actual = 0x1F4+field-self.control_offset
            elif self.record_pointer_offset <= field < self.record_pointer_offset+4:
                actual = 0x20C+field-self.record_pointer_offset
            elif self.handling_pointer_offset <= field < self.handling_pointer_offset+4:
                actual = 0x210+field-self.handling_pointer_offset
            elif self.control_tail_offset <= field < self.control_tail_offset+0x10C:
                actual = 0x214+field-self.control_tail_offset
            else:
                raise AssertionError(('Pointer into native vehicle padding', hex(pointer)))
            return base+index*800+actual
        return None

    def _store(self, native, count):
        output = (C.c_ubyte*(count*800))()
        self.lib.rr_test_vehicle_store(output, native, count)
        return bytes(output)

    def _load(self, native, data, count):
        assert len(data) == count*800
        source = C.create_string_buffer(data)
        self.lib.rr_test_vehicle_load(native, source, count)

    def read(self, native, address, size):
        for start, base, count, guard in self.banks:
            end = base+count*800
            if address >= end+guard or address+size <= base-guard:
                continue
            assert base-guard <= address and address+size <= end+guard
            data = C.string_at(start-guard, guard)+self._store(start, count)
            data += C.string_at(start+count*self.vehicle_size, guard)
            offset = address-(base-guard)
            return data[offset:offset+size]
        return C.string_at(native, size)

    def write(self, native, address, data):
        data = bytes(data)
        for start, base, count, guard in self.banks:
            end = base+count*800
            if address >= end+guard or address+len(data) <= base-guard:
                continue
            assert base-guard <= address and address+len(data) <= end+guard
            if address <= base and address+len(data) >= end:
                payload = data[base-address:end-address]
            else:
                payload = bytearray(self._store(start, count))
                lo, hi = max(base,address), min(end,address+len(data))
                payload[lo-base:hi-base] = data[lo-address:hi-address]
            self._load(start, bytes(payload), count)
            if address < base:
                length = base-address
                C.memmove(start-length, data[:length], length)
            if address+len(data) > end:
                length = address+len(data)-end
                C.memmove(start+count*self.vehicle_size, data[-length:], length)
            return
        C.memmove(native, data, len(data))
