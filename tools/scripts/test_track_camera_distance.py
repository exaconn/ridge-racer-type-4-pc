"""Shared camera owner, native C and original/linked MIPS service traces.

Root and look-ahead services may replace live scalar state. The legacy
RaceContextAlias spelling is normalized to the same complete native owner;
no separate progress/direction globals or manufactured host pointers exist.
"""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from mips_preview_engine import signed
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
CONTEXT_SIZE, VEHICLE_SIZE, NODE_SIZE = 0xEC, 0x1F4, 0x44
STATE = CONTEXT_SIZE + VEHICLE_SIZE + NODE_SIZE
EVENT = STATE + 8
OUTPUT = 8 + 2 * EVENT + STATE
CAR, NODE, STACK, STOP = 0x80180010, 0x80190010, 0x803F0000, 0x803FF000

PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/scene_camera_state.h"
#include "scene_context_wire.h"
#include "rr/race_types.h"
#include "game/gfx/dispenv.h"
#include "game/course/trkintp2.h"
#include "psyq/libgte/gtecord.h"
enum {CONTEXT=0xEC, VEHICLE=0x1F4, NODE=0x44, STATE=CONTEXT+VEHICLE+NODE};
static struct {
    u32 before[4]; SceneRaceContext context; u32 afterContext[4];
    RaceVehicle car; u32 afterCar[4]; CamNode44 node; u32 afterNode[4];
} world;
static const u8 *changedState[2];
static u8 *trace;
static unsigned scenario, calls;
static s32 rootResult;
#define RaceContext world.context
static void load(const u8 *p) {
    rr_scene_context_prefix_load(&world.context,p);p+=CONTEXT;
    memcpy(&world.car,p,VEHICLE);p+=VEHICLE;
    memcpy(&world.node,p,NODE);
}
static void snapshot(u8 *p) {
    rr_scene_context_prefix_store(p,&world.context);p+=CONTEXT;
    memcpy(p,&world.car,VEHICLE);p+=VEHICLE;
    memcpy(p,&world.node,NODE);
}
static void event(unsigned kind,s32 arg) {
    u32 words[2]={kind,(u32)arg};assert(calls<2);
    memcpy(trace,words,8);snapshot(trace+8);trace+=8+STATE;calls++;
}
s32 CordicSqrt(s32 squared) {
    event(1,squared);
    if(scenario&1)load(changedState[0]);
    return rootResult;
}
void GetTrackPointAhead(RaceVehicle *vehicle,s32 *output,s32 distance) {
    assert(vehicle==&world.car&&(uintptr_t)output>UINT32_MAX);
    assert(((uintptr_t)output&3)==0);
    event(2,distance);
    if(scenario&2)load(changedState[1]);
    for(unsigned i=0;i<3;i++)output[i]=(s32)((u32)scenario*17U+i);
}
'''
TAIL = r'''
void run_distance(unsigned number,const u8 *input,s32 root,u8 *output) {
    SceneRaceContext oldContext;RaceVehicle oldCar;
    scenario=number;rootResult=root;calls=0;trace=output+8;
    memset(&world,0xA5,sizeof(world));load(input);
    world.context.cameraEC.tracks138=NULL;world.context.camera24C=NULL;
    world.car.control.record18=NULL;world.car.control.handling1C=NULL;
    memcpy(&oldContext,&world.context,sizeof(oldContext));
    memcpy(&oldCar,&world.car,sizeof(oldCar));
    changedState[0]=input+STATE;changedState[1]=input+2*STATE;
    assert(sizeof(void*)==8&&(uintptr_t)&world.context>UINT32_MAX);
    assert((uintptr_t)&world.car>UINT32_MAX&&(uintptr_t)&world.node>UINT32_MAX);
    _Static_assert(offsetof(SceneRaceContext,cameraPreviousDistance40)==0x40,"previous distance");
    _Static_assert(offsetof(SceneRaceContext,cameraLimitReached44)==0x44,"limit flag");
    _Static_assert(offsetof(RaceVehicle,half1EA)==0x1EA,"node mode");
    _Static_assert(sizeof(CamNode44)==NODE,"complete camera node");
    s32 result=ComputeCameraNodeDist(&world.car,&world.node);
    memcpy(output,&result,4);memcpy(output+4,&calls,4);snapshot(trace);
    assert(calls==2);
    assert(!memcmp((u8*)&world.context+0xF0,(u8*)&oldContext+0xF0,sizeof(oldContext)-0xF0));
    assert(!memcmp((u8*)&world.car+VEHICLE,(u8*)&oldCar+VEHICLE,sizeof(oldCar)-VEHICLE));
    for(unsigned i=0;i<4;i++)assert(world.before[i]==0xA5A5A5A5&&world.afterContext[i]==0xA5A5A5A5&&world.afterCar[i]==0xA5A5A5A5&&world.afterNode[i]==0xA5A5A5A5);
}
'''


def get(data, offset, size=4):
    return int.from_bytes(data[offset:offset+size], 'little', signed=True)


def put(data, offset, value):
    struct.pack_into('<I', data, offset, value & 0xFFFFFFFF)


def trunc4(value):
    return abs(value)//4 * (-1 if value < 0 else 1)


class TrackCameraDistanceTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owner=source_owners(ROOT/'src/main/PAL')['ComputeCameraNodeDist']
        body=definition_source(owner.read_text(),'ComputeCameraNodeDist')
        assert 'CarTrackProgressDelta' not in body and 'CarTrackDirectionSign' not in body
        body,count=re.subn(r'\bRaceContextAlias\b','RaceContext',body)
        if count!=1:
            raise AssertionError(('review remaining symbol constraint',count))
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);source=directory/'camera.c';library=directory/'camera.dylib'
        source.write_text(PREFIX+body+TAIL)
        result=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                               '-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),str(source),'-o',str(library)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.library=C.CDLL(str(library))
        cls.library.run_distance.argtypes=[C.c_uint,C.c_void_p,C.c_int32,C.c_void_p]

    def test_alias_fields_wrapping_and_live_service_mutations(self):
        context=address_of('RaceContext')
        self.assertEqual(address_of('CarTrackProgressDelta'),context+0x40)
        self.assertEqual(address_of('CarTrackDirectionSign'),context+0x44)
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
        start=address_of('ComputeCameraNodeDist')
        services={address_of('CordicSqrt'):1,address_of('GetTrackPointAhead'):2}
        original=(ROOT/'assets/PAL/main.exe').read_bytes();linked=(ROOT/'build/PAL/main.exe').read_bytes()
        base=struct.unpack_from('<I',original,0x18)[0]
        size=next(int(r.split()[2],16) for r in (ROOT/'configs/PAL/functions.txt').read_text().splitlines() if r.split()[0]=='ComputeCameraNodeDist')
        images=[(name,image[0x800+start-base:0x800+start-base+size]) for name,image in [('original',original),('linked',linked)]]
        randomizer=random.Random(0xCA4E4044)
        edges=(-2147483648,-2147483647,-65537,-65536,-32769,-32768,-1,0,1,32767,32768,65535,65536,1073741825,2147483647)
        seen=set()

        def state_load(data):
            write(context,data[:CONTEXT_SIZE]);write(CAR,data[CONTEXT_SIZE:CONTEXT_SIZE+VEHICLE_SIZE]);write(NODE,data[-NODE_SIZE:])

        def snapshot():return read(context,CONTEXT_SIZE)+read(CAR,VEHICLE_SIZE)+read(NODE,NODE_SIZE)

        def store(cpu,access,address,width,value,user):
            address&=0x1FFFFFFF
            fields=tuple(((context+off)&0x1FFFFFFF,4) for off in (0x38,0x3C,0x40,0x44))
            stack=(STACK&0x1FFFFFFF)-0x48
            assert any(a<=address and address+width<=a+n for a,n in (*fields,(stack,0x48))),(case,hex(cpu.pc),hex(address),width)

        cpu=Cpu(memory,0,100,101,102,store)
        for case in range(2048):
            states=[]
            for phase in range(3):
                data=bytearray(randomizer.randbytes(STATE))
                put(data,0x1C,(0,1,-1,-2147483648)[(case+phase)%4])
                put(data,0x3C,edges[(case//3+phase)%len(edges)])
                put(data,0x40,edges[(case//7+phase)%len(edges)])
                put(data,0x44,(0,0,1,-1)[(case//4+phase)%4])
                put(data,CONTEXT_SIZE+0xF0,edges[(case//5+phase)%len(edges)])
                struct.pack_into('<h',data,CONTEXT_SIZE+0x1EA,(0,0,1,-1)[(case//8+phase)%4])
                struct.pack_into('<h',data,CONTEXT_SIZE+VEHICLE_SIZE+0x32,(-32768,-1,0,32767)[(case//16+phase)%4])
                states.append(data)
            value=edges[case%len(edges)]
            delta=[trunc4(signed(trunc4(get(states[0],0x20+4*i))-get(states[0],CONTEXT_SIZE+VEHICLE_SIZE+4*i))) for i in range(3)]
            squared=sum(x*x for x in delta)&0xFFFFFFFF;distance=signed(value*2)
            state=bytearray(states[1] if case&1 else states[0]);events=[struct.pack('<II',1,squared)+states[0]]
            changed=get(state,0x1C)!=0;seen.add(('changed',changed))
            if changed:
                quarter=distance>>2;put(state,0x3C,quarter);put(state,0x38,distance-quarter)
                put(state,0x40,distance);put(state,0x44,0)
                bypass=distance<=65535 or get(state,CONTEXT_SIZE+0x1EA,2)!=0;seen.add(('bypass',bypass))
                if bypass:put(state,0x44,1)
                forward=signed(get(state,CONTEXT_SIZE+VEHICLE_SIZE+0x32,2)-get(state,CONTEXT_SIZE+0xF0))>=0
                seen.add(('forward',forward))
                if forward:put(state,0x44,1)
            reached=get(state,0x44)!=0;seen.add(('reached',reached))
            if not reached:
                below=signed(distance-get(state,0x3C))<0;seen.add(('below',below))
                if below:put(state,0x44,1)
                else:
                    increasing=signed(get(state,0x40)-distance)<0;seen.add(('increasing',increasing))
                    if increasing:put(state,0x44,1)
            argument=get(state,CONTEXT_SIZE+VEHICLE_SIZE+0x14)
            events.append(struct.pack('<II',2,argument&0xFFFFFFFF)+state)
            if case&2:state=bytearray(states[2])
            put(state,0x40,distance)
            wanted=struct.pack('<II',distance&0xFFFFFFFF,2)+b''.join(events)+state
            native_in=C.create_string_buffer(b''.join(states));native_out=C.create_string_buffer(OUTPUT)
            self.library.run_distance(case,native_in,value,native_out)
            self.assertEqual(native_out.raw,wanted,(case,'native'))
            for name,code in images:
                write(start,code);write(context-16,b'\xA6'*16+b'\xA5'*0x564+b'\x6A'*16)
                write(CAR-16,b'\xA6'*16+b'\xA5'*VEHICLE_SIZE+b'\x6A'*16)
                write(NODE-16,b'\xA6'*16+b'\xA5'*NODE_SIZE+b'\x6A'*16);state_load(states[0])
                write(STACK-0x60,b'\xA5'*0x70)
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[5],cpu.r[29],cpu.r[31],cpu.pc=CAR,NODE,STACK,STOP,start
                calls=0
                for step in range(512):
                    if cpu.pc==STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    kind=services[cpu.pc];self.assertEqual(kind,calls+1,(case,name))
                    arg=cpu.r[4] if kind==1 else cpu.r[6]
                    self.assertEqual(struct.pack('<II',kind,arg)+snapshot(),events[calls],(case,name,'service',kind))
                    if kind==1:
                        if case&1:state_load(states[1])
                        result=value&0xFFFFFFFF
                    else:
                        self.assertEqual(cpu.r[4],CAR);self.assertEqual(cpu.r[5],STACK-0x28)
                        if case&2:state_load(states[2])
                        for i in range(3):write(cpu.r[5]+i*4,struct.pack('<I',case*17+i))
                        result=0xDEAD0002
                    resume=cpu.r[31]
                    for register in (2,3,*range(4,16),24,25):cpu.r[register]=0xDEAD0000+register
                    cpu.r[2],cpu.hi,cpu.lo,cpu.pc=result,0x12345678,0x87654321,resume;calls+=1
                self.assertEqual((cpu.pc,cpu.r[2],calls),(STOP,distance&0xFFFFFFFF,2),(case,name))
                self.assertEqual(snapshot(),bytes(state),(case,name,'final'))
                self.assertEqual((cpu.r[29],cpu.r[31]),(STACK,STOP))
                for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                self.assertEqual(read(context-16,16),b'\xA6'*16);self.assertEqual(read(context+CONTEXT_SIZE,0x564-CONTEXT_SIZE),b'\xA5'*(0x564-CONTEXT_SIZE))
                self.assertEqual(read(context+0x564,16),b'\x6A'*16)
                self.assertEqual(read(CAR-16,16),b'\xA6'*16);self.assertEqual(read(CAR+VEHICLE_SIZE,16),b'\x6A'*16)
                self.assertEqual(read(NODE-16,16),b'\xA6'*16);self.assertEqual(read(NODE+NODE_SIZE,16),b'\x6A'*16)
                self.assertEqual(read(STACK-0x60,0x18),b'\xA5'*0x18);self.assertEqual(read(STACK,16),b'\xA5'*16)
        self.assertEqual(seen,{(name,value) for name in ('changed','bypass','forward','reached','below','increasing') for value in (False,True)})


if __name__=='__main__':unittest.main()
