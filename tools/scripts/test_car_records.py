"""Disc-backed car layouts, the shared pointer API and original-MIPS result gating.

Phase17 and D_800AD4EF are the same byte. Active stages stay in0..7; out-of-range
phases are exercised only with no ranked finisher and a latched result, so the
production handler does not index the stage-result array in those cases.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import Mips, signed

ROOT = Path(__file__).resolve().parents[2]
RECORD_SIZE = 0x218
STATE_SIZE = 2*RECORD_SIZE + 8*8 + 0x2C + 4 + 8*4 + 8*4
RECORDS = (0x80180010, 0x80190010)
RANKS = 0x801A0010
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/data/bankrec.h"
#include "game/ui/uipart1.h"
#include "rr/menu_state_types.h"
#include "game/menu/menu8.h"
static struct World {
    u32 before[4];CarRecord records[2];RankEntry8 ranks[8];
    CarRecord *table[8];RankEntry8 *rankTable[8];MenuSettings2C selection;
    s32 time;u32 after[4];
} world;
#define CarRecordPtrs world.table
#define RankTable world.rankTable
#define SelectionState world.selection
#define TotalRaceTime world.time
enum {STATE_SIZE=2*0x218+8*8+0x2C+4+8*4+8*4};
static u8 trace[STATE_SIZE];
static unsigned scenario,calls;
static u32 record_address(const CarRecord *p) {
    assert(p==&world.records[0]||p==&world.records[1]);
    return p==&world.records[0]?0x80180010U:0x80190010U;
}
static void snapshot(u8 *out) {
    unsigned i;u32 p;
    memcpy(out,world.records,sizeof(world.records));out+=sizeof(world.records);
    memcpy(out,world.ranks,sizeof(world.ranks));out+=sizeof(world.ranks);
    memcpy(out,&world.selection,0x2C);out+=0x2C;
    memcpy(out,&world.time,4);out+=4;
    for(i=0;i<8;i++){p=record_address(world.table[i]);memcpy(out,&p,4);out+=4;}
    for(i=0;i<8;i++){
        assert(world.rankTable[i]>=world.ranks&&world.rankTable[i]<world.ranks+8);
        p=0x801A0010U+(u32)(world.rankTable[i]-world.ranks)*8U;memcpy(out,&p,4);out+=4;
    }
}
'''
MIDDLE = r'''
int FramesToMilliseconds(s32 frames,s32 subframe) {
    int result=real_milliseconds(frames,subframe);
    assert(calls++==0&&subframe==0);snapshot(trace);
    if(scenario&1){
        world.table[0]=&world.records[world.table[0]==world.records?1:0];
        world.selection.phase17=(world.selection.phase17+3)&7;
        world.selection.result19=(scenario&2)?9:-1;
        {u32 bits=0x80000000U|scenario;memcpy(&world.time,&bits,4);}
    }
    return result;
}
'''
TAIL = r'''
void run_gate(unsigned number,const u8 *input,u8 *output) {
    unsigned i;const u8 *p=input;u32 address;
    scenario=number;calls=0;memset(&world,0xA5,sizeof(world));memset(trace,0,sizeof(trace));
    for(i=0;i<4;i++){world.before[i]=0xA6A6A6A6;world.after[i]=0x6A6A6A6A;}
    memcpy(world.records,p,sizeof(world.records));p+=sizeof(world.records);
    memcpy(world.ranks,p,sizeof(world.ranks));p+=sizeof(world.ranks);
    memcpy(&world.selection,p,0x2C);p+=0x2C;
    memcpy(&world.time,p,4);p+=4;
    for(i=0;i<8;i++){
        memcpy(&address,p,4);p+=4;assert(address==0x80180010||address==0x80190010);
        SetCarRecordPtr(&world.records[address==0x80180010?0:1],i);
    }
    for(i=0;i<8;i++){
        memcpy(&address,p,4);p+=4;assert(address>=0x801A0010&&address<0x801A0050&&(address&7)==0);
        world.rankTable[i]=&world.ranks[(address-0x801A0010)/8];
    }
    assert((uintptr_t)world.table[0]>UINT32_MAX&&(uintptr_t)world.rankTable[0]>UINT32_MAX);
    EvaluateGpQualifyingResult();memcpy(output,trace,STATE_SIZE);snapshot(output+STATE_SIZE);
    memcpy(output+2*STATE_SIZE,&calls,4);
    for(i=0;i<4;i++)assert(world.before[i]==0xA6A6A6A6&&world.after[i]==0x6A6A6A6A);
    {const u8 *bytes=(const u8*)&world;
     for(i=offsetof(struct World,selection)+sizeof(world.selection);i<offsetof(struct World,time);i++)assert(bytes[i]==0xA5);
     for(i=offsetof(struct World,after)+sizeof(world.after);i<sizeof(world);i++)assert(bytes[i]==0xA5);}
}
void test_setter(void) {
    unsigned i,j;CarRecord *expected[8];struct World before;
    memset(&world,0xA5,sizeof(world));
    for(i=0;i<8;i++)world.table[i]=&world.records[i&1];
    for(i=0;i<8;i++)for(j=0;j<3;j++){
        CarRecord *p=j==2?NULL:&world.records[j];
        memcpy(&before,&world,sizeof(world));memcpy(expected,world.table,sizeof(expected));expected[i]=p;
        SetCarRecordPtr(p,(s32)i);
        assert(!memcmp(world.table,expected,sizeof(expected)));memcpy(before.table,expected,sizeof(expected));
        assert(!memcmp(&world,&before,sizeof(world)));
    }
}
void project_record(const void *input,u32 out[30],char name[10]) {
    CarRecord record;unsigned i;memcpy(&record,input,sizeof(record));
    out[0]=record.lastGear00;out[1]=record.handling1C8.steeringGain08;
    out[2]=record.handling1C8.steeringMode0A;out[3]=record.handling1C8.slideThreshold16;
    out[4]=record.handling1C8.headingDamping1C;out[5]=record.targetFrames1E8[0];
    out[6]=record.targetFrames1E8[1];out[7]=record.topSpeed1F0;out[8]=record.stage1F3;
    for(i=0;i<7;i++){
        out[9+i*3]=record.gears04[i].scaledRate00;
        out[10+i*3]=record.gears04[i].speed02;out[11+i*3]=record.gears04[i].accumulator32;
    }
    memcpy(name,record.name1F5,10);
}
'''


class Cpu(Mips):
    def step(self):
        instruction=self.load(self.pc,4)
        if instruction>>26!=1:return super().step()
        assert self.delay is None
        rs=(instruction>>21)&31;rt=(instruction>>16)&31;assert rt in (0,1)
        take=signed(self.r[rs])<0 if rt==0 else signed(self.r[rs])>=0
        self.delay=self.pc+4+(signed(instruction,16)<<2) if take else self.pc+8
        self.pc+=4


class CarRecordsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners=source_owners(ROOT/'src/main/PAL')
        body=lambda name:definition_source(owners[name].read_text(),name)
        milliseconds=body('FramesToMilliseconds').replace('FramesToMilliseconds(', 'real_milliseconds(',1)
        code=PREFIX+milliseconds+MIDDLE+body('SetCarRecordPtr')+body('EvaluateGpQualifyingResult')+body('GetCarFileIndex')+TAIL
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);source=directory/'native.c';library=directory/'native.dylib'
        source.write_text(code)
        built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                              '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                              '-I',str(ROOT/'include'),str(source),'-o',str(library)],capture_output=True,text=True)
        if built.returncode:raise AssertionError(built.stderr)
        cls.lib=C.CDLL(str(library));cls.lib.project_record.argtypes=[C.c_void_p,C.c_void_p,C.c_void_p]
        cls.lib.run_gate.argtypes=[C.c_uint,C.c_void_p,C.c_void_p]
        blob=(ROOT/'disc/PAL/R4.BIN').read_bytes();sectors=struct.unpack_from('<951I',blob,4)
        cls.records=[];indices=set()
        for alternate in range(2):
            for team in range(4):
                for manufacturer in range(4):
                    for model in range(20):
                        index=cls.lib.GetCarFileIndex(manufacturer,team,model,alternate);indices.add(index)
                        entry=blob[sectors[index]*2048:sectors[index+1]*2048]
                        header,start,end=struct.unpack_from('<3I',entry)
                        if header!=5 or end-start!=RECORD_SIZE:raise AssertionError((index,header,start,end))
                        cls.records.append(entry[start:end])
        if indices!=set(range(18,658)):raise AssertionError(indices)

    def test_all_640_archive_records_agree_with_named_layout(self):
        fields=[(0,'i'),(0x1D0,'h'),(0x1D2,'h'),(0x1DE,'h'),(0x1E4,'h'),
                (0x1E8,'i'),(0x1EC,'i'),(0x1F0,'h'),(0x1F3,'B')]
        for gear in range(7):fields.extend([(4+gear*60,'h'),(6+gear*60,'h'),(0x36+gear*60,'h')])
        self.assertEqual(len(self.records),640)
        for record in self.records:
            data=C.create_string_buffer(record);out=(C.c_uint32*30)();name=C.create_string_buffer(10)
            self.lib.project_record(data,out,name)
            expected=[struct.unpack_from('<'+fmt,record,offset)[0]&0xFFFFFFFF for offset,fmt in fields]
            self.assertEqual(list(out),expected);self.assertEqual(name.raw,record[0x1F5:0x1FF])
            self.assertIn(0,name.raw);self.assertIn(out[0],(1,4,5,6));self.assertLessEqual(out[8],3)

    def test_setter_retains_full_pointers_and_only_writes_requested_slot(self):
        self.lib.test_setter()

    def test_result_gate_original_mips_full_owners_and_mutable_service(self):
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
        put32=lambda a,v:write(a,struct.pack('<I',v&0xFFFFFFFF))
        start=address_of('EvaluateGpQualifyingResult');stop=0x803FF000;stack=0x803F0000
        selection=address_of('SelectionState');time=address_of('TotalRaceTime')
        self.assertEqual(address_of('D_800AD4EF'),selection+0x17)
        table=address_of('CarRecordPtrs');rank_table=address_of('RankTable');service=address_of('FramesToMilliseconds')
        executable=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',executable,0x18)[0]
        size=next(int(row.split()[2],16) for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines() if row.split()[0]=='EvaluateGpQualifyingResult')
        write(start,executable[0x800+start-base:0x800+start-base+size])
        def snapshot():
            return (b''.join(read(a,RECORD_SIZE) for a in RECORDS)+read(RANKS,64)+
                    read(selection,0x2C)+read(time,4)+read(table,32)+read(rank_table,32))
        def store(cpu,access,address,size,value,user):
            address&=0x1FFFFFFF
            self.assertTrue((selection&0x1FFFFFFF)<=address and address+size<=(selection&0x1FFFFFFF)+0x2C or
                            (stack&0x1FFFFFFF)-0x100<=address and address+size<=(stack&0x1FFFFFFF), (case,hex(cpu.pc),hex(address),size))
        cpu=Cpu(memory,0,100,101,102,store);randomizer=random.Random(0x36330)
        outcomes=set();calls_seen=set();controls_seen=set();winners_seen=set()
        for case in range(4096):
            records=[self.records[case%640],self.records[(case+137)%640]]
            data=bytearray(b''.join(records));data.extend(randomizer.randbytes(STATE_SIZE-len(data)))
            phase=(0,1,2,3,4,5,6,7,-128,-1,127)[case%11]
            winner=(case//11)%9 if 0<=phase<8 else 8;permutation=[(i+case)%8 for i in range(8)]
            for i,rank in enumerate(permutation):struct.pack_into('<h',data,2*RECORD_SIZE+rank*8+4,0 if i==winner else i+1)
            state=2*RECORD_SIZE+64
            data[state+0x17]=phase&255
            data[state+0x19]=((-128,-1,0,8,9,10,127)[(case//144)%7] if 0<=phase<8 else 9)&255
            selected=case&1;frames=struct.unpack_from('<i',records[selected],0x1E8+(phase%2)*4)[0]
            total=(0,1,-1,0x7FFFFFFF,-0x80000000,frames*40,frames*40-1,frames*40+1)[(case//16)%8]
            struct.pack_into('<i',data,state+0x2C,total)
            struct.pack_into('<8I',data,state+0x30,*(RECORDS[(i+case)%2] for i in range(8)))
            struct.pack_into('<8I',data,state+0x50,*(RANKS+rank*8 for rank in permutation))
            source=C.create_string_buffer(bytes(data));output=(C.c_ubyte*(STATE_SIZE*2+4))()
            self.lib.run_gate(case,source,output);expected=bytes(output)
            for i,address in enumerate(RECORDS):write(address-16,b'\xA6'*16+records[i]+b'\x6A'*16)
            write(RANKS,data[2*RECORD_SIZE:state]);write(selection,data[state:state+0x2C])
            write(time,data[state+0x2C:state+0x30])
            write(table,data[state+0x30:state+0x50]);write(rank_table,data[state+0x50:])
            cpu.reset();cpu.r=[0]+[0xBEEF0000+r for r in range(1,32)];cpu.r[29]=stack;cpu.r[31]=stop;cpu.pc=start
            calls=0
            for instruction in range(500):
                if cpu.pc==stop:break
                if cpu.pc!=service:cpu.step();continue
                self.assertEqual(snapshot(),expected[:STATE_SIZE],(case,'service boundary'))
                self.assertEqual(cpu.r[4],frames&0xFFFFFFFF);self.assertEqual(cpu.r[5],0);self.assertEqual(calls,0)
                if case&1:
                    put32(table,RECORDS[1-selected]);write(selection+0x17,bytes([(phase+3)&7]))
                    write(selection+0x19,bytes([9 if case&2 else 255]));put32(time,0x80000000|case)
                resume=cpu.r[31]
                for register in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[register]=0xDEAD0000+register
                cpu.r[2]=(frames*40)&0xFFFFFFFF;cpu.hi=0x12345678;cpu.lo=0x87654321;cpu.pc=resume;calls+=1
            self.assertEqual(cpu.pc,stop,case);self.assertEqual(calls,int.from_bytes(expected[-4:],'little'))
            self.assertEqual(snapshot(),expected[STATE_SIZE:STATE_SIZE*2],(case,'final owners'))
            self.assertEqual(cpu.r[29],stack);self.assertEqual(cpu.r[31],stop)
            for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
            for address in RECORDS:
                self.assertEqual(read(address-16,16),b'\xA6'*16);self.assertEqual(read(address+RECORD_SIZE,16),b'\x6A'*16)
            outcomes.add(read(selection+0x19,1));calls_seen.add(calls);controls_seen.add(phase);winners_seen.add(winner)
        self.assertEqual(calls_seen,{0,1});self.assertEqual(winners_seen,set(range(9)))
        self.assertEqual(controls_seen,{-128,-1,0,1,2,3,4,5,6,7,127});self.assertGreater(len(outcomes),8)


if __name__=='__main__':
    unittest.main()
