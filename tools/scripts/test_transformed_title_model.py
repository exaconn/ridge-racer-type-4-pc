"""Actual transformed-title caller with real angle/vector owners and MIPS traces.

The older rotating-title module and all its cases remain unchanged. This adds
another actual caller using its established scratch serialization and service
models. Builders/transform/submission remain instrumented boundaries.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
import test_model_submission_caller as shared

ROOT = Path(__file__).resolve().parents[2]
CASES, STATE, EVENT, OUTPUT = 2048, shared.STATE, shared.EVENT, shared.OUTPUT
SCRATCH, STATES, STACK, STOP = shared.SCRATCH, shared.STATES, shared.STACK, shared.STOP
POOL, TABLES = shared.POOL, shared.TABLES


def main():
    owners = source_owners(ROOT/'src/main/PAL')
    body = definition_source(owners['DrawTransformedTitleModel'].read_text(), 'DrawTransformedTitleModel')
    position = address_of('D_8001013C');entry = address_of('DrawTransformedTitleModel')
    pointer, format_at = address_of('IntroSceneStatePtr'), address_of('ModelBankFormat')
    prefix = shared.PREFIX.replace('s16 positions[8][8];',
        'union {Vec3s8 angles[2];VectorWords16 position;} inputs[8];')
    prefix = prefix.replace('world.positions', 'world.inputs')
    prefix = prefix.replace('static u32 control[32];',
                            'static const u32 positionAddress=%dU;\nstatic u32 control[32];' % position)
    tail = shared.TAIL.replace('unsigned number,s32 index,s32 animate,', 'unsigned number,s32 index,')
    tail = tail.replace('world.positions', 'world.inputs')
    tail = tail.replace('s32 yaw=DrawRotatingTitleModel(index,animate);',
        '''assert((uintptr_t)&world.inputs[(scenario>>3)%8].angles[0]>UINT32_MAX);
 DrawTransformedTitleModel(index,&world.inputs[(scenario>>3)%8].angles[0],
                          &world.inputs[(scenario>>6)%8].position);s32 yaw=0;''')
    assert tail != shared.TAIL and 'DrawRotatingTitleModel(' not in tail
    executable = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', executable, 24)[0]
    with TemporaryDirectory() as temp:
        source, library = Path(temp)/'native.c', Path(temp)/'native.dylib'
        source.write_text(prefix+body+tail)
        subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-I'+str(ROOT/'include'),
                        str(source), '-o', str(library)], check=True)
        native = C.CDLL(str(library));native.run_case.argtypes = [C.c_uint, C.c_int32, C.c_void_p, C.c_void_p]
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000);memory.mem_map(SCRATCH, 0x1000)
        write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
        read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
        put = lambda a, v: write(a, struct.pack('<I', v & 0xFFFFFFFF))
        write(base, executable[0x800:])
        services = {address_of(n): i+1 for i, n in enumerate(('SelectModelBank', 'RotMatrix', 'MulRotMatrix0', 'SetGteObjectTransform', 'SubmitModel'))}
        rng = random.Random(0x1BFA8164);cases = []
        indices = (-2147483648, -65537, -1, 0, 1, 2, 3, 65535, 2147483647)
        for case in range(CASES):
            data = bytearray(rng.randbytes(STATE))
            for offset, value in [(0, POOL[0]), (4, POOL[1]), (72, POOL[2]), (80, TABLES[0]),
                                  (84, POOL[4]), (88, POOL[6]), (116, POOL[2]), (284, position), (472, position+16), (1508, STATES[0])]:
                struct.pack_into('<I', data, offset, value)
            for i in range(4):struct.pack_into('<I', data, 1492+i*4, POOL[2+i%2])
            for i in (4, 12):struct.pack_into('<i', data, 1516+i*4, C.c_int16(struct.unpack_from('<I', data, 1516+i*4)[0]).value)
            cases.append((data, indices[case % len(indices)]))
        segments = [(SCRATCH,220), *((a,188) for a in STATES), (position,128),
                    *((a,n) for a,n in zip(POOL,(256,256,64,64,32,32,32,32))),
                    *((a,8) for a in TABLES), (pointer,4), (format_at,4)]
        assert sum(n for a,n in segments)+128 == STATE
        for version in ('original', 'linked'):
            code = executable[0x800+entry-base:0x800+entry-base+0x150]
            if version == 'linked':
                with (ROOT/'build/PAL/main.elf').open('rb') as stream:
                    elf = ELFFile(stream);symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('DrawTransformedTitleModel')[0]
                    assert symbol['st_value'] == entry and symbol['st_size'] == 0x150
                    section = elf.get_section(symbol['st_shndx']);offset = entry-section['sh_addr'];code = section.data()[offset:offset+0x150]
            write(entry, code);covered = set();aliases = set();selections = set()
            for case, (data, index) in enumerate(cases):
                output = (C.c_ubyte*OUTPUT)()
                native.run_case(case, index, C.create_string_buffer(bytes(data)), output);expected = bytes(output)
                unused, events = struct.unpack_from('<II', expected);assert unused == 0
                at = 0
                for address, size in segments:write(address, data[at:at+size]);at += size
                control = list(struct.unpack_from('<32I', data, 1516));write(STACK-256, b'\xA6'*512)
                event = multiplies = 0;temporary = None
                angles = position+((case>>3) % 8)*16;point = position+((case>>6) % 8)*16
                aliases.add(angles == point)
                def snapshot():return b''.join(read(a,n) for a,n in segments)+struct.pack('<32I', *control)
                def record(kind, *args):
                    nonlocal event
                    assert event < events;covered.add(kind);words = (kind, *args, *([0]*(4-len(args))))
                    assert struct.pack('<5I', *words)+snapshot() == expected[8+event*EVENT:8+(event+1)*EVENT], (version, case, event, kind)
                    event += 1
                def store(cpu, access, address, size, value, user):
                    assert STACK-0x40 <= address and address+size <= STACK or address == SCRATCH+0x78 and size == 4, (hex(address), size)
                cpu = shared.TransportCpu(memory, 0, 100, 101, 102, store)
                cpu.control = control;cpu.record_hardware = lambda k,v: record(k,v)
                cpu.r = [0]+[0xBEEF0000+r for r in range(1,32)]
                cpu.r[4:7] = [index & 0xFFFFFFFF, angles, point];cpu.r[29] = STACK;cpu.r[31] = STOP;cpu.pc = entry
                for instruction in range(1500):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    assert cpu.delay is None;kind = services[cpu.pc];a,b,c,d = cpu.r[4:8]
                    if kind == 1:
                        record(kind,a);bank = case & 1
                        for offset,value in [(72,POOL[2+bank]),(80,TABLES[bank]),(84,POOL[4+bank]),(88,POOL[6+bank])]:put(SCRATCH+offset,value)
                        put(pointer,STATES[bank]);result = 0
                    elif kind == 2:
                        assert a == angles and b == SCRATCH+0x84;record(kind,a,b)
                        seed = shared.hash_bytes(read(a,6)) ^ case
                        for i in range(8):seed = shared.next_word(seed);put(b+i*4,seed)
                        if case & 2:put(pointer,STATES[1-(case&1)])
                        result = 0
                    elif kind == 3:
                        assert a == SCRATCH+0x84 and multiplies < 2
                        if multiplies == 0:assert b == SCRATCH+0xA4
                        else:temporary = b
                        record(kind,a,multiplies);multiplies += 1;seed = shared.hash_bytes(read(a,32)) ^ case
                        for value in control[:5]:seed ^= value
                        for i in range(8):seed = shared.next_word(seed);put(b+i*4,seed)
                        result = b
                    elif kind == 4:
                        assert a == SCRATCH+0x7C and b == point and c == temporary
                        record(kind,a,b,shared.hash_bytes(read(c,32)));seed = shared.hash_bytes(read(b,6)) ^ shared.hash_bytes(read(c,32))
                        for i in range(14):seed = shared.next_word(seed);put(a+i*4,seed)
                        put(format_at,(-2147483647,-1,0,1,2,2147483647)[case%6]);result = 0
                    else:
                        assert a == SCRATCH and c == d == 0
                        selections.add(b == (index & 0xFFFFFFFF))
                        record(kind,a,b,c,d);result = 0
                    resume = cpu.r[31]
                    for register in (2,3,*range(4,16),24,25):cpu.r[register] = 0xDEAD0000+register
                    cpu.r[2] = result & 0xFFFFFFFF;cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321
                assert cpu.pc == STOP and event == events
                assert snapshot() == expected[8+shared.MAX_EVENTS*EVENT:], (version,case,'final')
                assert cpu.r[29] == STACK and cpu.r[31] == STOP
                for r in (*range(16,24),28,30):assert cpu.r[r] == 0xBEEF0000+r
                assert read(STACK-256,256-0x40) == b'\xA6'*(256-0x40) and read(STACK,256) == b'\xA6'*256
            assert covered == {1,2,3,4,5,*range(100,105),*range(108,113)} and aliases == {False,True} and selections == {False,True}
            print('PASS',version,CASES,'actual transformed-title flows; typed full-width angles/positions, alias cases and selection boundaries')


if __name__ == '__main__':main()
