from selection_fixture_transport import transport as transport_selection
"""InitPlayerCar: complete owners, real pointer capture and original MIPS traces."""
import ctypes as C
import os
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN

from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import Mips, signed

ROOT = Path(__file__).resolve().parents[2]
SOURCE = source_owners(ROOT/'src/main/PAL')['InitPlayerCar']
CASES = 4096
RECORD_SIZE = 0x218
STATE_SIZE = 800 + RECORD_SIZE*2 + 32 + 4 + 32
VEHICLE, RECORDS = 0x80180010, (0x80190010, 0x801A0010)
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "vehicle_wire.h"
#include "game/car/carsetup.h"
CarRecord *CarRecordPtrs[8];
u8 SelectionRows[8][4];
s32 RaceState;
static struct {u8 before[16];RaceVehicle value;u8 after[16];} world;
static unsigned scenario, slot, event;
static s32 grid;
enum {STATE_SIZE=800+2*0x218+32+4+32};
static u8 trace[3][STATE_SIZE];
static void snapshot(u8 *out, u8 padding) {
    unsigned i;u32 pointer;
    rr_vehicle_wire_padding_value(&world.value,padding);
    rr_vehicle_wire_store_fields(out,&world.value);out+=800;
    for(i=0;i<2;i++){memcpy(out,&rr_test_records[i].record,0x218);out+=0x218;}
    memcpy(out,SelectionRows,32);out+=32;memcpy(out,&RaceState,4);out+=4;
    for(i=0;i<8;i++){pointer=rr_vehicle_record_address(CarRecordPtrs[i]);memcpy(out,&pointer,4);out+=4;}
}
void PsyqBzero(void *buffer,s32 size) {
    assert(event++==0&&buffer==&world.value&&size==sizeof(world.value));
    snapshot(trace[0],0xA5);
    memset(buffer,0,(size_t)size);
    if(scenario&1){
        u16 bits;CarRecord *captured=CarRecordPtrs[slot];
        CarRecordPtrs[slot]=&rr_test_records[1-(slot&1)].record;
        SelectionRows[slot][3]^=1;
        memcpy(&bits,&captured->gears04[0].speed02,2);bits^=0xA55A;
        memcpy(&captured->gears04[0].speed02,&bits,2);
    }
}
void PlaceCarOnGrid(RaceVehicle *vehicle,s32 position) {
    assert(event++==1&&vehicle==&world.value&&position==grid);
    assert(vehicle->control.record18==&rr_test_records[slot&1].record);
    assert(vehicle->control.handling1C==&vehicle->control.record18->handling1C8);
    assert((uintptr_t)vehicle->control.record18>UINT32_MAX);
    snapshot(trace[1],0);
    vehicle->first=(s32)((u32)vehicle->first^scenario*0x9E3779B9U);
}
'''
TAIL = r'''
void run_case(unsigned number,s32 recordIndex,s32 position,const u8 *input,u8 *output) {
    unsigned i;const u8 *p=input;
    _Static_assert(sizeof(CarRecord)==0x218,"whole car record");
    assert((uintptr_t)&world.value>UINT32_MAX);
    scenario=number;slot=(unsigned)recordIndex;grid=position;event=0;
    rr_vehicle_wire_reset();memset(rr_test_records,0xA6,sizeof(rr_test_records));
    for(i=0;i<2;i++)rr_vehicle_record_resolve(i?0x801A0010:0x80190010);
    memset(&world,0xA5,sizeof(world));memset(world.before,0xA6,16);memset(world.after,0x6A,16);
    rr_vehicle_wire_load(&world.value,p);p+=800;
    for(i=0;i<2;i++){memcpy(&rr_test_records[i].record,p,0x218);p+=0x218;}
    memcpy(SelectionRows,p,32);p+=32;memcpy(&RaceState,p,4);
    for(i=0;i<8;i++)CarRecordPtrs[i]=&rr_test_records[i&1].record;
    InitPlayerCar(&world.value,recordIndex,position);
    assert(event==2&&rr_test_record_count==2&&rr_test_handling_count==0);
    snapshot(trace[2],0);memcpy(output,trace,sizeof(trace));
    for(i=0;i<16;i++)assert(world.before[i]==0xA6&&world.after[i]==0x6A);
    assert(rr_test_records[0].address==0x80190010&&rr_test_records[1].address==0x801A0010);
    {const u8 *unused=(const u8*)&rr_test_records[2];
     for(i=0;i<sizeof(rr_test_records)-2*sizeof(rr_test_records[0]);i++)assert(unused[i]==0xA6);}
}
'''


class Cpu(Mips):
    def step(self):
        instruction = self.load(self.pc,4)
        if instruction>>26 != 1:
            return super().step()
        assert self.delay is None
        rs=(instruction>>21)&31;rt=(instruction>>16)&31
        assert rt in (0,1)
        take=signed(self.r[rs])<0 if rt==0 else signed(self.r[rs])>=0
        self.delay=self.pc+4+(signed(instruction,16)<<2) if take else self.pc+8
        self.pc+=4


def main():
    os.chdir(ROOT)
    body=definition_source(SOURCE.read_text(),'InitPlayerCar')
    with TemporaryDirectory() as temporary:
        directory=Path(temporary);source=directory/'native.c';library=directory/'native.dylib'
        source.write_text(transport_selection(PREFIX+'\n#define SelectionRows SelectionRows\n'+body+TAIL,body,'InitPlayerCar','&SelectionRows',32,[('PsyqBzero','1'),('PlaceCarOnGrid','1')]))
        subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                        '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-Iinclude',
                        '-Itools/scripts',str(source),'-o',str(library)],check=True)
        native=C.CDLL(str(library))
        native.run_case.argtypes=[C.c_uint,C.c_int32,C.c_int32,C.c_void_p,C.c_void_p]
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0,0x400000)
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        put32=lambda a,n:write(a,struct.pack('<I',n&0xFFFFFFFF))
        table=address_of('CarRecordPtrs');selection=address_of('SelectionRows');race=address_of('RaceState')
        start=address_of('InitPlayerCar');clear=address_of('PsyqBzero');place=address_of('PlaceCarOnGrid')
        stack=0x803F0000;stop=0x803FF000
        executable=(ROOT/'assets/PAL/main.exe').read_bytes()
        base=struct.unpack_from('<I',executable,0x18)[0]
        write(start,executable[0x800+start-base:0x800+start-base+0x1E4])
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'],'rb') as stream:
                elf=ELFFile(stream);symbol=elf.get_section_by_name('.symtab').get_symbol_by_name('InitPlayerCar')[0]
                section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                assert symbol['st_value']==start
                write(start,section.data()[offset:offset+symbol['st_size']])
        writes=set()
        def store(cpu,access,address,size,value,user):
            address &= 0x1FFFFFFF
            allowed=[(VEHICLE,800),(stack-0x30,0x30),(race,4)]+[(a,RECORD_SIZE) for a in RECORDS]
            assert any((a&0x1FFFFFFF)<=address and address+size<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(address),size)
            writes.add((address,size))
        cpu=Cpu(memory,0,100,101,102,store)
        def snapshot():
            return (read(VEHICLE,800)+b''.join(read(a,RECORD_SIZE) for a in RECORDS)+
                    read(selection,32)+read(race,4)+read(table,32))
        randomizer=random.Random(0x22980)
        gear_profiles=(-2147483648,-1,0,1,2,3,4,5,6)
        thresholds=(-32768,-1,0,9999,10000,10001,32767)
        speeds=(-32768,-606,-605,-1,0,1,605,606,32767)
        counts=set();indices=set();mutations=0
        for case in range(CASES):
            data=bytearray(randomizer.randbytes(STATE_SIZE))
            struct.pack_into('<II',data,0x20C,0,0)
            for record in range(2):
                at=800+record*RECORD_SIZE
                count=gear_profiles[(case//2+record)%len(gear_profiles)]
                struct.pack_into('<i',data,at,count)
                struct.pack_into('<h',data,at+0x1DE,thresholds[(case//7+record)%len(thresholds)])
                for gear in range(7):
                    speed=speeds[(case+gear)%len(speeds)] if case<2048 else randomizer.randrange(-32768,32768)
                    struct.pack_into('<h',data,at+6+gear*0x3C,speed)
            slot=(case//18)%8;grid=(0,1,7,-1,-2147483648,2147483647)[(case//144)%6]
            indices.add(slot);counts.add(gear_profiles[(case//2+(slot&1))%len(gear_profiles)])
            struct.pack_into('<8I',data,STATE_SIZE-32,*(RECORDS[i&1] for i in range(8)))
            output=(C.c_ubyte*(STATE_SIZE*3))();input_buffer=C.create_string_buffer(bytes(data))
            native.run_case(case,slot,grid,input_buffer,output)
            expected=bytes(output)
            write(VEHICLE-16,b'\xA6'*16+data[:800]+b'\x6A'*16)
            for i,address in enumerate(RECORDS):
                write(address-16,b'\xA6'*16+data[800+i*RECORD_SIZE:800+(i+1)*RECORD_SIZE]+b'\x6A'*16)
            at=800+2*RECORD_SIZE;write(selection,data[at:at+32]);write(race,data[at+32:at+36]);write(table,data[-32:])
            cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)]
            cpu.r[4:7]=[VEHICLE,slot,grid&0xFFFFFFFF];cpu.r[29]=stack;cpu.r[31]=stop;cpu.pc=start
            event=0
            for instruction in range(2000):
                if cpu.pc==stop:break
                if cpu.pc not in (clear,place):cpu.step();continue
                assert cpu.delay is None
                assert snapshot()==expected[event*STATE_SIZE:(event+1)*STATE_SIZE],(case,event,'service entry')
                assert cpu.r[4]==VEHICLE
                if cpu.pc==clear:
                    assert event==0 and cpu.r[5]==800
                    write(VEHICLE,bytes(800))
                    if case&1:
                        mutations+=1;put32(table+slot*4,RECORDS[1-(slot&1)])
                        address=selection+slot*4+3;write(address,bytes([read(address,1)[0]^1]))
                        address=RECORDS[slot&1]+6;bits=int.from_bytes(read(address,2),'little')^0xA55A
                        write(address,struct.pack('<H',bits))
                else:
                    assert event==1 and cpu.r[5]==grid&0xFFFFFFFF
                    address=VEHICLE+0x184;put32(address,int.from_bytes(read(address,4),'little')^(case*0x9E3779B9))
                event+=1;resume=cpu.r[31]
                for register in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[register]=0xDEAD0000+register
                cpu.pc=resume;cpu.hi=0x12345678;cpu.lo=0x87654321
            assert cpu.pc==stop and event==2,(case,hex(cpu.pc),event)
            assert snapshot()==expected[2*STATE_SIZE:],(case,'final state')
            assert cpu.r[29]==stack and cpu.r[31]==stop
            for register in (*range(16,24),28,30):assert cpu.r[register]==0xBEEF0000+register
            for address,size in ((VEHICLE,800),*((a,RECORD_SIZE) for a in RECORDS)):
                assert read(address-16,16)==b'\xA6'*16 and read(address+size,16)==b'\x6A'*16
        assert counts==set(gear_profiles) and indices==set(range(8)) and mutations==CASES//2
        print('PASS',CASES,('compiled' if os.getenv('RR_COMPILED_ELF') else 'original'),'MIPS/native cases; all gear counts, 8 record slots, clear-time root replacement, full owners, guards, UBSan')


if __name__=='__main__':
    main()
