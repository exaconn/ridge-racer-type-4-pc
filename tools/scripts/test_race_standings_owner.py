"""Actual standings producer and distance consumer share one full native owner.

Independent stable-ranking/speed/distance models check complete native objects.
The same cases exercise original and linked MIPS, including full PAL vehicles,
both standings buffers, exterior guards, immutable inputs and register/stack ABI.
No production body is rewritten and no service is replaced.
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
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
TABLES = (0x1F80007C, 0x80190010)
NODES, STACK, STOP = 0x80180010, 0x803F0000, 0x803FF000
FRAME = 2*0x78 + 13*0x320
PREFIX = r'''
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/race.h"
#include "rr/track_sample_types.h"
#include "vehicle_wire.h"
typedef struct {u32 before[4];RaceStandingsTable value;u32 after[4];} TableOwner;
typedef struct {
    u32 before[4];SceneRaceContext context;u32 afterContext[4];
    TableOwner tables[2];
    RaceVehicle computers[11];u32 afterComputers[4];
    RaceVehicle players[2];u32 afterPlayers[4];
    RaceVehicle *cars[8];u32 afterCars[4];
    TrackSample3C nodes[4];u32 afterNodes[4];
    s16 sine[1025];s16 count;u32 after[4];
} World;
static World actual,expected;
static unsigned selected;
#define RaceContext actual.context
#define ComCars actual.computers
#define PlayerCars actual.players
#define CarTable actual.cars
#define TrackNodes actual.nodes
#define SinTable actual.sine
#define NumPlayers actual.context.playerCount14
#define NumCars actual.count
'''
TAIL = r'''
static s32 bits(u32 value){s32 result;memcpy(&result,&value,4);return result;}
static RaceVehicle *car(World *w,unsigned i){assert(i<13);return i<11?&w->computers[i]:&w->players[i-11];}
static unsigned identity(const RaceVehicle *p){
    for(unsigned i=0;i<13;i++)if(p==car(&actual,i))return i;
    assert(!"unknown vehicle identity");return 0;
}
static u32 address(const RaceVehicle *p){
    if(!p)return 0;unsigned i=identity(p);
    return i<11?@COMPUTERS@+i*0x320U:@PLAYERS@+(i-11)*0x320U;
}
static s32 speed(const RaceVehicle *v){
    unsigned phase=(unsigned)(1024-(int)v->heading1D4-(int)expected.nodes[v->trackNodeF0].road.heading)&4095;
    unsigned half=phase&2047,index=half<=1024?half:2048-half;
    int64_t sine=expected.sine[index];if(phase&2048)sine=-sine;
    return (s32)(-(sine*v->input/4096));
}
static void reference_standings(void){
    RaceStandingsTable *t=&expected.tables[selected].value;
    for(unsigned i=0;i<11;i++)if((s16)expected.computers[i].flags1E4>0)
        expected.computers[i].control.lateralSpeed58=speed(&expected.computers[i]);
    if(expected.context.playerCount14>0)t->playerSpeed[0]=speed(&expected.players[0]);
    if(expected.context.playerCount14==2)t->playerSpeed[1]=speed(&expected.players[1]);
    unsigned used=0;
    for(int row=0;row<expected.count;row++){
        int best=-1;int64_t longest=INT64_MIN;
        for(int i=0;i<expected.count;i++)if(!(used&(1U<<i))){
            const RaceVehicle *v=car(&expected,identity(expected.cars[i]));
            int64_t d=(int64_t)v->first+v->second;
            assert(d>=INT32_MIN&&d<=INT32_MAX);
            if(d>longest){best=i;longest=d;}
        }
        assert(best>=0);used|=1U<<best;
        t->entries18[row].unk00=best;t->entries18[row].totalDist04=(s32)longest;
        t->entries18[row].vehicle08=expected.cars[best];
    }
}
static int64_t magnitude(s32 value){
    /* The original's signed wrapped magnitude keeps INT_MIN negative. */
    if(value==INT32_MIN)return INT32_MIN;
    return value<0?-(int64_t)value:value;
}
static void reference_distance(unsigned current){
    RaceStandingsTable *t=&expected.tables[selected].value;
    int mode=expected.context.playerCount14;
    t->playerDistance[0]=0;t->playerDistance[1]=0;t->closestPlayer=0;
    if(mode!=1&&mode!=2)return;
    const RaceVehicle *v=car(&expected,current);
    int64_t own=(int64_t)v->first+v->second;
    for(int i=0;i<mode;i++)t->playerDistance[i]=bits((u32)((int64_t)expected.players[i].first+expected.players[i].second-own));
    if(mode==1)t->playerDistance[1]=INT32_MAX;
    else t->closestPlayer=magnitude(t->playerDistance[1])<magnitude(t->playerDistance[0]);
}
static void put32(u8 *p,u32 value){memcpy(p,&value,4);}
static void snapshot(u8 *out){
    assert(!memcmp(&actual,&expected,sizeof(actual)));
    for(unsigned j=0;j<2;j++){
        const RaceStandingsTable *t=&actual.tables[j].value;
        memcpy(out,t,24);out+=24;
        for(unsigned i=0;i<8;i++){
            memcpy(out,&t->entries18[i],8);put32(out+8,address(t->entries18[i].vehicle08));out+=12;
        }
    }
    for(unsigned i=0;i<13;i++){rr_vehicle_wire_store(out,car(&actual,i));out+=0x320;}
}
void run_standings(unsigned number,const u8 *input,u8 *output){
    const u8 *p=input;selected=number&1;
    memset(&actual,0xA5,sizeof(actual));rr_vehicle_wire_reset();
    memcpy(&actual.context.playerCount14,p,4);p+=4;
    s32 count;memcpy(&count,p,4);p+=4;assert(count>=0&&count<=8);actual.count=(s16)count;
    for(unsigned i=0;i<13;i++){
        RaceVehicle *v=car(&actual,i);
        memcpy(&v->first,p,4);memcpy(&v->second,p+4,4);memcpy(&v->heading1D4,p+8,2);
        s16 node;memcpy(&node,p+10,2);v->trackNodeF0=node;
        memcpy(&v->input,p+12,2);memcpy(&v->flags1E4,p+14,2);p+=16;
        v->control.record18=NULL;v->control.handling1C=NULL;
    }
    for(unsigned i=0;i<8;i++){assert(*p<13);actual.cars[i]=car(&actual,*p++);}
    for(unsigned i=0;i<4;i++){memcpy(&actual.nodes[i].road.heading,p,2);p+=2;}
    memcpy(actual.sine,p,2050);p+=2050;assert(p-input==2282);
    for(unsigned j=0;j<2;j++)for(unsigned i=0;i<8;i++)actual.tables[j].value.entries18[i].vehicle08=NULL;
    actual.context.standings=&actual.tables[selected].value;
    actual.context.cameraEC.tracks138=NULL;actual.context.camera24C=NULL;
    actual.context.cameraNodes550=NULL;actual.context.workspace554=NULL;
    actual.context.lightPresets558=NULL;actual.context.workspace55C=NULL;
    actual.context.soundPayload564=NULL;actual.context.lookupTables=NULL;
    assert(sizeof(void*)==8&&(uintptr_t)actual.context.standings>UINT32_MAX);
    for(unsigned i=0;i<8;i++)assert((uintptr_t)actual.cars[i]>UINT32_MAX);
    memcpy(&expected,&actual,sizeof(actual));reference_standings();
    UpdateRaceStandings();snapshot(output);output+=@FRAME@;
    unsigned ids[]={number%11,(number+3)%11,11,12};
    for(unsigned i=0;i<4;i++){
        reference_distance(ids[i]);
        CalcDistanceToPlayer(NumPlayers==1||NumPlayers==2?car(&actual,ids[i]):NULL);
        snapshot(output);output+=@FRAME@;
    }
}
'''



class RaceStandingsOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners=source_owners(ROOT/'src/main/PAL')
        source=owners['UpdateRaceStandings'].read_text()
        macro=re.search(r'^#define SIN\(x\)[^\n]*',source,re.M).group()
        bodies='\n'.join(definition_source(source,n) for n in
                         ('UpdateRaceStandings','WrappingMagnitude','CalcDistanceToPlayer'))
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        path=Path(cls.temp.name)/'standings.c';lib=path.with_suffix('.dylib')
        text=PREFIX+'\n'+macro+'\n'+bodies+TAIL
        text=text.replace('@COMPUTERS@',hex(address_of('ComCars'))+'U').replace('@PLAYERS@',hex(address_of('PlayerCars'))+'U').replace('@FRAME@',str(FRAME))
        path.write_text(text)
        result=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                               '-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),str(path),'-o',str(lib)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.native=C.CDLL(str(lib));cls.native.run_standings.argtypes=[C.c_uint,C.c_void_p,C.c_void_p]

    def test_shared_producer_distance_aliases_and_both_mips_images(self):
        context=address_of('RaceContext');computers=address_of('ComCars');players=address_of('PlayerCars')
        self.assertEqual(address_of('RaceStandings'),context+0xE8)
        self.assertEqual(address_of('NumPlayers'),context+0x14)
        rng=random.Random(0xAC738);cases=[];modes=(-1,0,1,2,3)
        edges=((-2147483648,0),(2147483647,0),(-2147483647,-1),(2147483646,1),(0,0),(7,-7))
        for number in range(1024):
            mode=modes[number%len(modes)];count=(number//len(modes))%9
            values=[]
            for i in range(13):
                first,second=edges[(number+i)%len(edges)] if number%3==0 else (rng.randrange(-1000000,1000001),rng.randrange(-1000000,1000001))
                values.append((first,second,rng.randrange(-32768,32768),rng.randrange(4),rng.randrange(-32768,32768),(0,0x8000,i+1)[(number+i)%3]))
            ids=rng.sample(range(13),8)
            heads=[rng.randrange(-32768,32768) for _ in range(4)]
            sine=[rng.randrange(-32768,32768) for _ in range(1025)]
            raw=struct.pack('<ii',mode,count)+b''.join(struct.pack('<iihhhH',*v) for v in values)+bytes(ids)+struct.pack('<4h',*heads)+struct.pack('<1025h',*sine)
            self.assertEqual(len(raw),2282)
            result=C.create_string_buffer(5*FRAME);self.native.run_standings(number,C.create_string_buffer(raw),result)
            cases.append((mode,count,values,ids,heads,sine,result.raw))
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0,0x400000);memory.mem_map(0x1F800000,0x1000)
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        write=lambda a,data:memory.mem_write(a&0x1FFFFFFF,bytes(data))
        active_table=TABLES[0]
        def store(cpu,access,address,length,value,user):
            address&=0x1FFFFFFF
            allowed=[(active_table,0x78),(STACK-0x100,0x100)]+[(computers+i*0x320+0x1F4+0x58,4) for i in range(11)]
            self.assertTrue(any((a&0x1FFFFFFF)<=address and address+length<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(cpu.pc),hex(address),length))
        cpu=Cpu(memory,0,100,101,102,store)
        def invoke(name,arg=None):
            write(STACK-0x110,b'\xA5'*0x120);cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
            if arg is not None:cpu.r[4]=arg
            cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(name)
            for _ in range(16384):
                if cpu.pc==STOP:break
                cpu.step()
            self.assertEqual((cpu.pc,cpu.r[29],cpu.r[31]),(STOP,STACK,STOP))
            for r in (*range(16,24),28,30):self.assertEqual(cpu.r[r],0xBEEF0000+r)
            self.assertEqual(read(STACK-0x110,16),b'\xA5'*16);self.assertEqual(read(STACK,16),b'\xA5'*16)
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            image=path.read_bytes();write(struct.unpack_from('<I',image,0x18)[0],image[0x800:])
            for number,(mode,count,values,ids,heads,sine,wanted) in enumerate(cases):
                active_table=TABLES[number&1]
                ctx=bytearray(b'\xA5'*0x57C);struct.pack_into('<i',ctx,0x14,mode);struct.pack_into('<I',ctx,0xE8,active_table)
                for off in (0x224,0x24C,0x550,0x554,0x558,0x55C,0x564,0x568):struct.pack_into('<I',ctx,off,0)
                cars=[];addresses=[computers+i*0x320 for i in range(11)]+[players+i*0x320 for i in range(2)]
                for first,second,heading,node,input_value,flags in values:
                    car=bytearray(b'\xA5'*0x320);struct.pack_into('<ii',car,0x184,first,second)
                    struct.pack_into('<h',car,0x1D4,heading);struct.pack_into('<i',car,0xF0,node)
                    struct.pack_into('<h',car,0x1D8,input_value);struct.pack_into('<H',car,0x1E4,flags)
                    struct.pack_into('<II',car,0x20C,0,0);cars.append(car)
                nodes=bytearray(b'\xA5'*(4*0x3C))
                for i,heading in enumerate(heads):struct.pack_into('<h',nodes,i*0x3C+0x12,heading)
                table=bytearray(b'\xA5'*0x78)
                for i in range(8):struct.pack_into('<I',table,0x18+i*12+8,0)
                regions=[(context,ctx),(computers,b''.join(cars[:11])),(players,b''.join(cars[11:])),*( (a,table) for a in TABLES),
                         (NODES,nodes),(address_of('CarTable'),struct.pack('<8I',*(addresses[i] for i in ids))),
                         (address_of('SinTable'),struct.pack('<1025h',*sine))]
                for a,data in regions:write(a-16,b'\xA6'*16+data+b'\x6A'*16)
                write(address_of('TrackNodes'),struct.pack('<I',NODES));write(address_of('NumCars'),struct.pack('<h',count))
                calls=[('UpdateRaceStandings',None)]+[('CalcDistanceToPlayer',addresses[i] if mode in (1,2) else 0) for i in (number%11,(number+3)%11,11,12)]
                for stage,(name,arg) in enumerate(calls):
                    invoke(name,arg)
                    got=b''.join(read(a,0x78) for a in TABLES)+read(computers,11*0x320)+read(players,2*0x320)
                    self.assertEqual(got,wanted[stage*FRAME:(stage+1)*FRAME],(number,label,name,stage))
                    self.assertEqual(read(context,0x57C),bytes(ctx))
                    for a,data in regions:
                        self.assertEqual(read(a-16,16),b'\xA6'*16);self.assertEqual(read(a+len(data),16),b'\x6A'*16)
                    for a,data in regions[-3:]:self.assertEqual(read(a,len(data)),bytes(data))
                    self.assertEqual(read(address_of('TrackNodes'),4),struct.pack('<I',NODES))
                    self.assertEqual(read(address_of('NumCars'),2),struct.pack('<h',count))


if __name__=='__main__':
    unittest.main()
