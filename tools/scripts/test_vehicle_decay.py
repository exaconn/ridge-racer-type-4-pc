"""Actual decay handler: original/linked MIPS and complete native vehicle owners.

The handler runs through the public stage-table type. Other stage algorithms
are outside this fixture. Pointees and all unselected vehicles stay intact.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CASES = 8192
RECORDS = (0x80200010, 0x80201010)
STATE = 2400 + 2 * 0x218
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/car/cartimer.h"
#include "game/race/racemai2.h"
static struct {u8 before[16];RaceVehicle cars[3];u8 after[16];} world;
CarUpdateStage CarUpdateStages[11]={[2]=DecayCarOffset};
'''
TAIL = r'''
void run_case(unsigned selected,const u8 *input,u8 *output) {
    unsigned i;assert(selected<3);rr_vehicle_wire_reset();
    memset(&world,0xA6,sizeof(world));
    for(i=0;i<3;i++)rr_vehicle_wire_load(&world.cars[i],input+i*800);
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
    assert(rr_test_records[0].address==0x80200010U&&rr_test_records[1].address==0x80201010U);
    for(i=0;i<2;i++)memcpy(&rr_test_records[i].record,input+2400+i*0x218,0x218);
    assert(sizeof(void*)==8&&(uintptr_t)world.cars>UINT32_MAX);
    assert(sizeof(CarUpdateStages[2])==sizeof(void*)&&(uintptr_t)CarUpdateStages[2]>UINT32_MAX);
    assert(offsetof(RaceVehicle,control.pathDeltaX30)>0x224);
    assert(world.cars[0].control.record18==world.cars[1].control.record18);
    assert(world.cars[0].control.handling1C==world.cars[1].control.handling1C);
    for(i=0;i<3;i++) {
        assert((uintptr_t)world.cars[i].control.record18>UINT32_MAX);
        assert(world.cars[i].control.handling1C==&world.cars[i].control.record18->handling1C8);
    }
    CarUpdateStages[2](&world.cars[selected]);
    for(i=0;i<3;i++)rr_vehicle_wire_store(output+i*800,&world.cars[i]);
    for(i=0;i<2;i++)memcpy(output+2400+i*0x218,&rr_test_records[i].record,0x218);
    for(i=0;i<16;i++)assert(world.before[i]==0xA6&&world.after[i]==0xA6);
    assert(rr_test_record_count==2&&rr_test_handling_count==0);
}
'''


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    owner = owners['DecayCarOffset']
    body = definition_source(owner.read_text(), 'DecayCarOffset')
    # Preserve the existing identity-copy asm; only the CPU register name is
    # target-specific. No data access, operation or field view is rewritten.
    body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
    with TemporaryDirectory() as temp:
        directory = Path(temp)
        source, library = directory/'native.c', directory/'native.dylib'
        source.write_text(PREFIX+body+TAIL)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC',
                        '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        '-I'+str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library))
        native.run_case.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
        executable = (ROOT/'assets/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I', executable, 24)[0]
        entry = address_of('DecayCarOffset')
        stages = address_of('CarUpdateStages')
        original_table = struct.unpack_from('<11I', executable, 0x800+stages-base)
        assert original_table[2] == entry
        symbols = {int(address, 16): name for name, address, size in
                   map(str.split, (ROOT/'configs/PAL/functions.txt').read_text().splitlines())}
        for address in original_table:
            name = symbols[address]
            definition = definition_source(owners[name].read_text(), name)
            signature = definition[:definition.index('{')]
            assert re.search(r'\bvoid\s+'+re.escape(name)+r'\s*\(\s*RaceVehicle\s*\*\s*\w+\s*\)', signature), (name, signature)

        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000)
        write = lambda address, data: memory.mem_write(address & 0x1FFFFFFF, bytes(data))
        read = lambda address, size: bytes(memory.mem_read(address & 0x1FFFFFFF, size))
        cars = (0x80180010, 0x80180330, 0x80180650)
        stack, stop = 0x803F0000, 0x803FF000
        rng = random.Random(0x2C138224)
        active_values = (-32768, -1, 0, 1, 2, 32767)
        timers = (-32768, -1, 0, 1, 2, 15, 32767)
        flags = (0, 0x10, 0x11, 0x8000, 0xFFFF)
        offsets = (-32768, -32767, -9, -8, -7, -1, 0, 1, 7, 8, 9, 32766, 32767)
        positions = (-2147483648, -2147483647, -1, 0, 1, 2147483646, 2147483647)
        cases = []
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for index in range(3):
                at = index*800;record = RECORDS[index//2]
                struct.pack_into('<II', data, at+0x20C, record, record+0x1C8)
                struct.pack_into('<h', data, at+0x19C, active_values[(case//3+index) % 6])
                struct.pack_into('<h', data, at+0x19E, timers[(case//18+index) % 7])
                struct.pack_into('<H', data, at+0x1E4, flags[(case//126+index) % 5])
                struct.pack_into('<h', data, at+0x198, offsets[(case+index) % len(offsets)])
                struct.pack_into('<h', data, at+0x19A, offsets[(case//13+index) % len(offsets)])
                struct.pack_into('<i', data, at, positions[(case+index) % 7])
                struct.pack_into('<i', data, at+8, positions[(case//7+index) % 7])
            selected = case % 3
            if case == 0:
                # Regression: the private native prefix clears playerFlags24,
                # while the PAL handler clears the genuine pathDeltaX30 field.
                struct.pack_into('<4h', data, 0x198, 32767, -32768, 1, 1)
                struct.pack_into('<H', data, 0x1E4, 0x10)
                struct.pack_into('<I', data, 0x218, 0x11223344)
                struct.pack_into('<h', data, 0x224, -1234)
            cases.append((data, selected))

        def snapshot():
            return b''.join(read(address, 800) for address in cars)+b''.join(read(address, 0x218) for address in RECORDS)

        for version in ('original', 'linked'):
            code = executable[0x800+entry-base:0x800+entry-base+0x9C]
            table = struct.pack('<11I', *original_table)
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream);symtab = elf.get_section_by_name('.symtab')
                    function = symtab.get_symbol_by_name('DecayCarOffset')[0]
                    section = elf.get_section(function['st_shndx']);offset = function['st_value']-section['sh_addr']
                    assert function['st_value'] == entry and function['st_size'] == 0x9C
                    code = section.data()[offset:offset+function['st_size']]
                    symbol = symtab.get_symbol_by_name('CarUpdateStages')[0]
                    assert symbol['st_value'] == stages
                    section = next(s for s in elf.iter_sections()
                                   if s['sh_type'] == 'SHT_PROGBITS' and
                                   s['sh_addr'] <= stages and stages+44 <= s['sh_addr']+s['sh_size'])
                    offset = stages-section['sh_addr']
                    table = section.data()[offset:offset+44]
                assert table == struct.pack('<11I', *original_table)
            write(entry, code);write(stages, table)
            path_clears = position_writes = 0
            wrapped_axes = set()
            for case, (data, selected) in enumerate(cases):
                output = (C.c_ubyte*STATE)()
                native.run_case(selected, C.create_string_buffer(bytes(data)), output)
                expected = bytes(output)
                for index, address in enumerate(cars):write(address, data[index*800:(index+1)*800])
                for index, address in enumerate(RECORDS):write(address-16, b'\xA6'*16+data[2400+index*0x218:2400+(index+1)*0x218]+b'\x6A'*16)
                write(cars[0]-16, b'\xA6'*16);write(cars[2]+800, b'\x6A'*16)
                write(stack-256, b'\xA6'*512);writes = []
                address = cars[selected]
                def store(cpu, access, at, size, value, user):
                    offset = at-address
                    assert (offset, size) in ((0, 4), (8, 4), (0x198, 2), (0x19A, 2),
                                               (0x19C, 2), (0x19E, 2), (0x224, 2)), (hex(at), size)
                    writes.append((offset, size))
                cpu = Cpu(memory, 0, 100, 101, 102, store)
                cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)]
                cpu.r[4] = address;cpu.r[29] = stack;cpu.r[31] = stop
                cpu.pc = int.from_bytes(read(stages+2*4, 4), 'little')
                assert cpu.pc == entry
                for instruction in range(100):
                    if cpu.pc == stop:break
                    cpu.step()
                assert cpu.pc == stop and snapshot() == expected, (version, case)
                assert cpu.r[29] == stack and cpu.r[31] == stop
                for register in (*range(16, 24), 28, 30):assert cpu.r[register] == 0xBEEF0000+register
                assert read(stack-256, 512) == b'\xA6'*512
                for record in RECORDS:
                    assert read(record-16, 16) == b'\xA6'*16 and read(record+0x218, 16) == b'\x6A'*16
                assert read(cars[0]-16, 16) == b'\xA6'*16 and read(cars[2]+800, 16) == b'\x6A'*16
                at = selected*800
                assert expected[at+0x218:at+0x21C] == data[at+0x218:at+0x21C]
                assert expected[at+0x20C:at+0x214] == data[at+0x20C:at+0x214]
                path_clears += (0x224, 2) in writes
                position_writes += (0, 4) in writes
                for position, kick in ((0, 0x198), (8, 0x19A)):
                    if (position, 4) in writes:
                        start = struct.unpack_from('<i', data, at+position)[0]
                        displacement = struct.unpack_from('<h', data, at+kick)[0]
                        if not -2147483648 <= start-displacement <= 2147483647:
                            wrapped_axes.add(position)
                if case == 0:
                    assert expected[0x218:0x21C] == struct.pack('<I', 0x11223344)
                    assert expected[0x224:0x226] == bytes(2)
            assert path_clears > 0 and position_writes > path_clears
            assert wrapped_axes == {0, 8}
            print('PASS', version, CASES, 'decay flows; path clears', path_clears,
                  'moving cases', position_writes, '; full owners/aliased pointees, typed table, ABI, UBSan')


if __name__ == '__main__':
    main()
