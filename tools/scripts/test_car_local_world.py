"""Typed vehicle-to-world boundary, live owners and original/linked MIPS.

The six matrix services are explicit adapters. This checks the caller's pointer,
scalar, snapshot and wrapping contract; it does not model GTE matrix arithmetic.
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
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
SCENE, CAR, VECTORS, OFFSETS = 0xEC, 0x1F4, 32, 16
STATE = SCENE + CAR + VECTORS + OFFSETS
EVENT = 16 + 36 + 6 + STATE
OUTPUT = 4 + 6 * EVENT + STATE
CAR_ADDRESS, VECTOR_ADDRESS, OFFSET_ADDRESS = 0x80180010, 0x80190010, 0x801A0010
STACK, STOP = 0x803F0000, 0x803FF000
SERVICES = ('BuildRotMatrixY', 'BuildRotMatrixX', 'MulMatrix', 'BuildRotMatrixZ', 'MulMatrix2', 'ApplyMatrix')

PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/scene_camera_state.h"
#include "scene_context_wire.h"
#include "rr/race_types.h"
#include "game/course/trkintp2.h"
#include "game/math/rotmat.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat5.h"
enum {SCENE=0xEC,CAR=0x1F4,STATE=SCENE+CAR+32+16,EVENT=16+36+6+STATE};
static struct {
    u32 before[4];SceneRaceContext scene;u32 sceneGuard[4];
    RaceVehicle car;u32 carGuard[4];
    Vector vectors[2];u32 vectorGuard[4];Vec3s8 offsets[2];u32 after[4];
} world;
static unsigned scenario, calls, outputIndex, offsetIndex;
static const u8 *replacements;
static s32 result[3];
static u8 *trace;
static MatrixWords32 *yawMatrix,*orientationMatrix;
static Vector *output(void){
    switch(outputIndex){
    case 0:return &world.scene.cameraFollowPosition20.vector;
    case 1:return &world.scene.view60.pose00.position00.vector;
    case 2:return &world.car.worldPosition10.vector;
    case 3:return &world.car.position20.vector;
    default:assert(outputIndex<6);return &world.vectors[outputIndex-4];
    }
}
static const Vec3s8 *offset(void){
    if(offsetIndex==0)return &world.scene.cameraFollowOffset30;
    assert(offsetIndex<3);return &world.offsets[offsetIndex-1];
}
static u32 offsetAddress(void){
    return offsetIndex?0x801A0010U+8U*(offsetIndex-1):@CONTEXT@+0x30;
}
static void load(const u8 *p){
    rr_scene_context_prefix_load(&world.scene,p);p+=SCENE;memcpy(&world.car,p,CAR);p+=CAR;
    memcpy(world.vectors,p,32);p+=32;memcpy(world.offsets,p,16);
}
static void snapshot(u8 *p){
    rr_scene_context_prefix_store(p,&world.scene);p+=SCENE;memcpy(p,&world.car,CAR);p+=CAR;
    memcpy(p,world.vectors,32);p+=32;memcpy(p,world.offsets,16);
}
static void boundary(unsigned kind,u32 a,u32 b,u32 c){
    u32 words[4]={kind,a,b,c};assert(kind==calls+1);memcpy(trace,words,16);
    memset(trace+16,0,42);
    if(calls>=1)memcpy(trace+16,yawMatrix,18);
    if(calls>=2)memcpy(trace+34,orientationMatrix,18);
    if(kind==6)memcpy(trace+52,offset(),6);
    snapshot(trace+58);trace+=EVENT;calls++;
    if(scenario&(1U<<(kind-1)))load(replacements+(kind-1)*STATE);
}
static void rotate(MatrixWords32 *matrix,unsigned kind){
    for(unsigned i=0;i<18;i++)((u8 *)matrix)[i]=(u8)(scenario*29U+kind*47U+i*13U);
}
void BuildRotMatrixY(MatrixWords32 *matrix,s32 angle){
    assert(calls==0&&(uintptr_t)matrix>UINT32_MAX&&((uintptr_t)matrix&3)==0);
    yawMatrix=matrix;boundary(1,1,(u32)angle,0);rotate(matrix,1);
}
void BuildRotMatrixX(MatrixWords32 *matrix,s32 angle){
    assert(calls==1&&(uintptr_t)matrix>UINT32_MAX&&((uintptr_t)matrix&3)==0);
    assert(matrix!=yawMatrix);orientationMatrix=matrix;boundary(2,2,(u32)angle,0);rotate(matrix,2);
}
MatrixWords32 *MulMatrix(MatrixWords32 *basis,const MatrixWords32 *right){
    assert(basis==yawMatrix&&right==orientationMatrix);boundary(3,1,2,0);rotate(basis,3);return basis;
}
void BuildRotMatrixZ(MatrixWords32 *matrix,s32 angle){
    assert(matrix==orientationMatrix);boundary(4,2,(u32)angle,0);rotate(matrix,4);
}
MatrixWords32 *MulMatrix2(const MatrixWords32 *basis,MatrixWords32 *right){
    assert(basis==yawMatrix&&right==orientationMatrix);boundary(5,1,2,0);rotate(right,5);return right;
}
Vec3i *ApplyMatrix(const MatrixWords32 *matrix,const Vec3s8 *input,Vec3i *out){
    assert(matrix==orientationMatrix&&input==offset());
    assert((uintptr_t)input>UINT32_MAX&&(uintptr_t)out>UINT32_MAX);
    assert(((uintptr_t)input&3)==0&&((uintptr_t)out&3)==0);
    boundary(6,2,offsetAddress(),3);out->x=result[0];out->y=result[1];out->z=result[2];return out;
}
'''
TAIL = r'''
void run_local_world(unsigned number,const u8 *states,const s32 values[3],u8 *out){
    SceneRaceContext oldScene;RaceVehicle oldCar;
    scenario=number;outputIndex=number%6;offsetIndex=(number/6)%3;
    calls=0;trace=out+4;replacements=states+STATE;memcpy(result,values,12);
    memset(&world,0xA5,sizeof(world));load(states);
    world.scene.cameraEC.tracks138=NULL;world.scene.camera24C=NULL;
    world.car.control.record18=NULL;world.car.control.handling1C=NULL;
    memcpy(&oldScene,&world.scene,sizeof(oldScene));memcpy(&oldCar,&world.car,sizeof(oldCar));
    assert(sizeof(void*)==8&&(uintptr_t)&world.car>UINT32_MAX&&(uintptr_t)output()>UINT32_MAX);
    _Static_assert(offsetof(RaceVehicle,worldPosition10)==0x10,"world vector prefix");
    _Static_assert(sizeof(world.car.worldPosition10)==16,"complete source vector");
    _Static_assert(offsetof(SceneRaceContext,cameraFollowOffset30)==0x30,"packed offset");
    void (*entry)(RaceVehicle*,const Vec3s8*,Vector*)=CarLocalToWorld;
    entry(&world.car,offset(),output());
    assert(calls==6);memcpy(out,&calls,4);snapshot(trace);
    assert(!memcmp((u8*)&world.scene+0xF0,(u8*)&oldScene+0xF0,sizeof(oldScene)-0xF0));
    assert(!memcmp((u8*)&world.car+CAR,(u8*)&oldCar+CAR,sizeof(oldCar)-CAR));
    for(unsigned i=0;i<4;i++)assert(world.before[i]==0xA5A5A5A5&&world.sceneGuard[i]==0xA5A5A5A5&&world.carGuard[i]==0xA5A5A5A5&&world.vectorGuard[i]==0xA5A5A5A5&&world.after[i]==0xA5A5A5A5);
}
'''



def get32(data, offset):
    return struct.unpack_from('<i', data, offset)[0]


def set32(data, offset, value):
    struct.pack_into('<I', data, offset, value & 0xFFFFFFFF)


class CarLocalWorldTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owner=source_owners(ROOT/'src/main/PAL')['CarLocalToWorld']
        body=definition_source(owner.read_text(),'CarLocalToWorld')
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);source=directory/'local.c';library=directory/'local.dylib'
        source.write_text(PREFIX.replace('@CONTEXT@',hex(address_of('RaceContext'))+'U')+body+TAIL)
        proc=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                             '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                             '-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),str(source),'-o',str(library)],capture_output=True,text=True)
        if proc.returncode:raise AssertionError(proc.stderr)
        cls.library=C.CDLL(str(library))
        cls.library.run_local_world.argtypes=[C.c_uint,C.c_void_p,C.c_void_p,C.c_void_p]

    def test_live_rotations_full_vector_copy_outputs_and_wrapping(self):
        context=address_of('RaceContext');start=address_of('CarLocalToWorld')
        service_addresses={address_of(name):kind for kind,name in enumerate(SERVICES,1)}
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
        read=lambda address,n:bytes(memory.mem_read(address&0x1FFFFFFF,n))
        write=lambda address,data:memory.mem_write(address&0x1FFFFFFF,bytes(data))
        original=(ROOT/'assets/PAL/main.exe').read_bytes();linked=(ROOT/'build/PAL/main.exe').read_bytes()
        base=struct.unpack_from('<I',original,0x18)[0]
        size=next(int(row.split()[2],16) for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines() if row.split()[0]=='CarLocalToWorld')
        images=[(label,image[0x800+start-base:0x800+start-base+size]) for label,image in [('original',original),('linked',linked)]]
        randomizer=random.Random(0x439BC);edges=(-2147483648,-2147483647,-65537,-1,0,1,65535,2147483646,2147483647)
        seen=set();wraps=set()
        output_offsets=(0x20,0x60,SCENE+0x10,SCENE+0x20,SCENE+CAR,SCENE+CAR+16)
        output_addresses=(context+0x20,context+0x60,CAR_ADDRESS+0x10,CAR_ADDRESS+0x20,VECTOR_ADDRESS,VECTOR_ADDRESS+16)
        offset_offsets=(0x30,SCENE+CAR+32,SCENE+CAR+40)
        offset_addresses=(context+0x30,OFFSET_ADDRESS,OFFSET_ADDRESS+8)

        def load(state):
            write(context,state[:SCENE]);write(CAR_ADDRESS,state[SCENE:SCENE+CAR]);write(VECTOR_ADDRESS,state[SCENE+CAR:SCENE+CAR+32]);write(OFFSET_ADDRESS,state[-16:])

        def snapshot():return read(context,SCENE)+read(CAR_ADDRESS,CAR)+read(VECTOR_ADDRESS,32)+read(OFFSET_ADDRESS,16)

        def stores(cpu,access,address,width,value,user):
            address&=0x1FFFFFFF
            regions=(((STACK-0x80)&0x1FFFFFFF,0x80),(output_addresses[case%6]&0x1FFFFFFF,16))
            assert any(a<=address and address+width<=a+n for a,n in regions),(case,hex(cpu.pc),hex(address),width)

        cpu=Cpu(memory,0,100,101,102,stores)
        for case in range(4096):
            states=[]
            for phase in range(7):
                state=bytearray(randomizer.randbytes(STATE))
                for axis in range(3):
                    set32(state,SCENE+0x10+4*axis,edges[(case+phase*3+axis)%len(edges)])
                    struct.pack_into('<h',state,SCENE+0x80+2*axis,(-32768,-32767,-1,0,1,32767)[(case+phase+axis)%6])
                states.append(state)
            values=[edges[(case//3+axis*2)%len(edges)] for axis in range(3)]
            current=bytearray(states[0]);events=[];matrices=[bytes(18),bytes(18)]
            oi,ii=case%6,(case//6)%3;seen.add((oi,ii,case&63))
            for kind in range(1,7):
                if kind==1:args=(1,2048-struct.unpack_from('<h',current,SCENE+0x82)[0],0)
                elif kind==2:args=(2,struct.unpack_from('<h',current,SCENE+0x80)[0],0)
                elif kind==4:args=(2,struct.unpack_from('<h',current,SCENE+0x84)[0],0)
                elif kind in (3,5):args=(1,2,0)
                else:args=(2,offset_addresses[ii],3)
                packed=current[offset_offsets[ii]:offset_offsets[ii]+6] if kind==6 else bytes(6)
                events.append(struct.pack('<4I',kind,*(a&0xFFFFFFFF for a in args))+b''.join(matrices)+packed+current)
                if case&(1<<(kind-1)):current=bytearray(states[kind])
                if kind<6:matrices[0 if kind in (1,3) else 1]=bytes((case*29+kind*47+i*13)&255 for i in range(18))
            origin=bytes(current[SCENE+0x10:SCENE+0x20]);off=output_offsets[oi];current[off:off+16]=origin
            for axis in range(3):
                summed=get32(origin,axis*4)+values[axis]
                if summed>2147483647:wraps.add('positive')
                if summed< -2147483648:wraps.add('negative')
                set32(current,off+4*axis,summed)
            wanted=struct.pack('<I',6)+b''.join(events)+current
            native_in=C.create_string_buffer(b''.join(states));native_values=(C.c_int32*3)(*values);native_out=C.create_string_buffer(OUTPUT)
            self.library.run_local_world(case,native_in,native_values,native_out)
            self.assertEqual(native_out.raw,wanted,(case,'native'))
            for label,code in images:
                write(start,code)
                for address,n in ((context,0x564),(CAR_ADDRESS,CAR),(VECTOR_ADDRESS,32),(OFFSET_ADDRESS,16)):
                    write(address-16,b'\xA6'*16+b'\xA5'*n+b'\x6A'*16)
                load(states[0]);write(STACK-0x90,b'\xA5'*0xA0)
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[5],cpu.r[6],cpu.r[29],cpu.r[31],cpu.pc=CAR_ADDRESS,offset_addresses[ii],output_addresses[oi],STACK,STOP,start
                calls=0
                for step in range(256):
                    if cpu.pc==STOP:break
                    if cpu.pc not in service_addresses:cpu.step();continue
                    kind=service_addresses[cpu.pc];self.assertEqual(kind,calls+1,(case,label))
                    matrix=(STACK-0x80+0x10,STACK-0x80+0x30)
                    first,second=cpu.r[4],cpu.r[5]
                    if kind in (1,2,4):
                        index=0 if kind==1 else 1;self.assertEqual(first,matrix[index]);args=(index+1,second,0)
                    elif kind in (3,5):
                        self.assertEqual((first,second),matrix);args=(1,2,0)
                    else:
                        self.assertEqual((first,second,cpu.r[6]),(matrix[1],offset_addresses[ii],STACK-0x28));args=(2,second,3)
                    rotations=(read(matrix[0],18) if calls>=1 else bytes(18))+(read(matrix[1],18) if calls>=2 else bytes(18))
                    packed=read(second,6) if kind==6 else bytes(6)
                    self.assertEqual(struct.pack('<4I',kind,*args)+rotations+packed+snapshot(),events[calls],(case,label,'service',kind))
                    if case&(1<<(kind-1)):load(states[kind])
                    if kind<6:write(matrix[0 if kind in (1,3) else 1],bytes((case*29+kind*47+i*13)&255 for i in range(18)))
                    else:write(cpu.r[6],struct.pack('<3i',*values))
                    resume=cpu.r[31]
                    for reg in (2,3,*range(4,16),24,25):cpu.r[reg]=0xDEAD0000+reg
                    cpu.r[2],cpu.hi,cpu.lo,cpu.pc=0xDEAD0002,0x12345678,0x87654321,resume;calls+=1
                self.assertEqual((cpu.pc,calls,cpu.r[29],cpu.r[31]),(STOP,6,STACK,STOP),(case,label))
                self.assertEqual(snapshot(),bytes(current),(case,label,'final'))
                for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
                for address,n in ((context,0x564),(CAR_ADDRESS,CAR),(VECTOR_ADDRESS,32),(OFFSET_ADDRESS,16)):
                    self.assertEqual(read(address-16,16),b'\xA6'*16);self.assertEqual(read(address+n,16),b'\x6A'*16)
                self.assertEqual(read(context+SCENE,0x564-SCENE),b'\xA5'*(0x564-SCENE))
                self.assertEqual(read(STACK-0x90,16),b'\xA5'*16);self.assertEqual(read(STACK,16),b'\xA5'*16)
        self.assertEqual(seen,{(o,i,m) for o in range(6) for i in range(3) for m in range(64) if any(n%6==o and (n//6)%3==i and n%64==m for n in range(4096))})
        self.assertEqual(len({(o,i) for o,i,m in seen}),18);self.assertEqual({m for o,i,m in seen},set(range(64)))
        self.assertEqual(wraps,{'positive','negative'})


if __name__=='__main__':unittest.main()
