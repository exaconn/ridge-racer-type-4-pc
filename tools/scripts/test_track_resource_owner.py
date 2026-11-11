"""One event directory and workspace: actual C and original/linked MIPS.

Native descriptors contain signed relative offsets before relocation and real
pointers afterwards. Resource sections are complete native objects; PAL transport
maps their identities, never their host pointer bits. No production rewrite or
opaque resource service is used.
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

ROOT=Path(__file__).resolve().parents[2]
BASE,CAR,STACK,STOP=0x80180010,0x801A0010,0x803F0000,0x803FF000
PAIR_OFFSETS=(0x20,0x140,0x160)
EVENT_OFFSETS=(0x50,0x180,0x1C0,0x200)
DIRECTORY,WORKSPACE,IMAGE_SIZE=0x100,0x300,0x1140
NAMES=('RelocateTrackLookupTables','SetTrackWorkspace','CountTrackEntries','FindNearestTrackEntry',
       'FindTrackEventIndex','LookupTrackEventPair','LookupTrackEventPair2',
       'LookupTrackEventTable0','LookupTrackEventTable1','LookupTrackEventTable2','LookupTrackEventTable3','LookupTrackEvent','ApplyTrackLightPreset','SetSceneOverlayEnabled','SetSceneOverlayColor')
PREFIX=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/lookup.h"
#include "rr/track_resources.h"
#include "rr/race_types.h"
#include "game/gfx/dispenv.h"
#include "game/gfx/scene1.h"
#include "game/data/cdfile2.h"
#include "game/course/trkintp2.h"
typedef struct {u8 primary[0xA00];TrackLightPreset8 lightPresets[4];u8 tertiary[0x200];CamNode44 nodes[8];} Workspace;
typedef struct {
    TrackEventPair pairsBefore[8];TrackEventEntry eventsBefore[8];
    TrackEventDirectory directory;
    TrackEventPair pairsAfter[2][8];TrackEventEntry eventsAfter[3][8];
    Workspace workspace;
} Resource;
static struct World {
    u32 before[4];Resource resource;u32 afterResource[4];
    SceneRaceContext context;u32 afterContext[4];RaceVehicle car;u32 after[4];
    CarSlotBufferC8 slots[16];s32 nodeCount;
} world,expected;
#define RaceContext world.context
#define TrackNodeCount world.nodeCount
#define CarSlotBuffers world.slots
static s32 get32(const u8 *p){s32 n;memcpy(&n,p,4);return n;}
static s16 get16(const u8 *p){s16 n;memcpy(&n,p,2);return n;}
static u32 address(const void *p){
    if(p==NULL)return 0;
    if(p==&world.resource.directory)return 0x80180110U;
    if(p==world.resource.pairsBefore)return 0x80180030U;
    if(p==world.resource.pairsAfter[0])return 0x80180150U;
    if(p==world.resource.pairsAfter[1])return 0x80180170U;
    if(p==world.resource.eventsBefore)return 0x80180060U;
    for(unsigned i=0;i<3;i++)if(p==world.resource.eventsAfter[i])return 0x80180190U+64U*i;
    if(p==world.resource.workspace.primary)return 0x80180310U;
    if(p==world.resource.workspace.lightPresets)return 0x80180D10U;
    if(p==world.resource.workspace.tertiary)return 0x80180D30U;
    if(p==world.resource.workspace.nodes)return 0x80180F30U;
    assert(!"pointer outside complete resource owners");return 0;
}
static void put32(u8 *p,u32 n){memcpy(p,&n,4);}
static void snapshot(u8 out[88]){
    TrackEventDirectory *t=&world.resource.directory;
    memcpy(out,t,16);
    put32(out+16,address(t->pairs0.entries));put32(out+20,address(t->pairs1.entries));
    put32(out+24,address(t->table0.entries));put32(out+28,address(t->table1.entries));
    put32(out+32,address(t->table2.entries));put32(out+36,address(t->table3.entries));put32(out+40,address(t->pairs2.entries));
    put32(out+44,address(RaceContext.cameraNodes550));put32(out+48,address(RaceContext.workspace554));
    put32(out+52,address(RaceContext.lightPresets558));put32(out+56,address(RaceContext.workspace55C));
    memcpy(out+60,&RaceContext.model560,4);put32(out+64,address(RaceContext.soundPayload564));
    put32(out+68,address(RaceContext.lookupTables));memcpy(out+72,&RaceContext.sound,16);
}
'''
TAIL=r'''
void run_resource(unsigned scenario,const u8 input[426],u8 output[168]){
    TrackEventDirectory *t=&world.resource.directory;const u8 *p=input;
    memset(&world,0xA5,sizeof(world));world.nodeCount=get32(p);p+=4;
    world.car.trackNodeF0=get32(p);p+=4;s32 key=get32(p);p+=4;
    memcpy(&t->pairCount0,p,14);p+=14;
    memcpy(world.resource.eventsBefore,p,64);p+=64;
    memcpy(world.resource.eventsAfter,p,192);p+=192;
    memcpy(world.resource.pairsBefore,p,32);p+=32;
    memcpy(world.resource.pairsAfter,p,64);p+=64;
    for(unsigned i=0;i<8;i++){world.resource.workspace.nodes[i].node32=get16(p);p+=2;}
    memcpy(world.resource.workspace.lightPresets,p,32);p+=32;
    assert(p-input==426);
    t->pairs0.offset=(s32)offsetof(Resource,pairsBefore)-(s32)offsetof(Resource,directory);
    t->pairs1.offset=(s32)offsetof(Resource,pairsAfter)-(s32)offsetof(Resource,directory);
    t->table0.offset=(s32)offsetof(Resource,eventsBefore)-(s32)offsetof(Resource,directory);
    t->table1.offset=(s32)offsetof(Resource,eventsAfter)-(s32)offsetof(Resource,directory);
    t->table2.offset=t->table1.offset+64;t->table3.offset=t->table1.offset+128;
    t->pairs2.offset=t->pairs1.offset+32;
    RaceContext.standings=NULL;
    RaceContext.cameraEC.tracks138=NULL;RaceContext.camera24C=NULL;
    RaceContext.cameraNodes550=NULL;RaceContext.workspace554=NULL;
    RaceContext.lightPresets558=NULL;RaceContext.workspace55C=NULL;RaceContext.soundPayload564=NULL;RaceContext.lookupTables=NULL;
    world.car.control.record18=NULL;world.car.control.handling1C=NULL;
    memcpy(&expected,&world,sizeof(world));
    {const s32 invalid[]={INT32_MIN,-1,7,8,65535,INT32_MAX};
     for(unsigned i=0;i<6;i++){assert(LookupTrackEvent(invalid[i],key)==0);assert(!memcmp(&world,&expected,sizeof(world)));}}
    expected.context.cameraNodes550=world.resource.workspace.nodes;
    expected.context.workspace554=world.resource.workspace.primary;
    expected.context.lightPresets558=world.resource.workspace.lightPresets;
    expected.context.workspace55C=world.resource.workspace.tertiary;
    expected.context.lookupTables=t;
    expected.resource.directory.pairs0.entries=world.resource.pairsBefore;
    expected.resource.directory.pairs1.entries=world.resource.pairsAfter[0];
    expected.resource.directory.pairs2.entries=world.resource.pairsAfter[1];
    expected.resource.directory.table0.entries=world.resource.eventsBefore;
    expected.resource.directory.table1.entries=world.resource.eventsAfter[0];
    expected.resource.directory.table2.entries=world.resource.eventsAfter[1];
    expected.resource.directory.table3.entries=world.resource.eventsAfter[2];
    _Static_assert(sizeof(void*)==8&&sizeof(unsigned long)==sizeof(void*),"full native relocation addresses");
    assert((uintptr_t)t>UINT32_MAX&&(uintptr_t)&RaceContext>UINT32_MAX);
    assert((uintptr_t)&world.resource.workspace>UINT32_MAX);
    if(scenario&1){SetTrackWorkspace((u8*)&world.resource.workspace);RelocateTrackLookupTables(t);}
    else{RelocateTrackLookupTables(t);SetTrackWorkspace((u8*)&world.resource.workspace);}
    assert(!memcmp(&world,&expected,sizeof(world)));
    s32 results[9];results[0]=CountTrackEntries();results[1]=FindNearestTrackEntry(&world.car);
    for(unsigned i=0;i<7;i++){results[i+2]=LookupTrackEvent(i,key);assert(!memcmp(&world,&expected,sizeof(world)));}
    unsigned selector=scenario%256;
    expected.context.overlayEnabledD4=selector>=1&&selector<=7;
    if(selector>=1&&selector<=7){
        const TrackLightPreset8 *preset=&world.resource.workspace.lightPresets[(selector-1)%4];
        expected.context.overlayModeDC=preset->mode;
        expected.context.overlayColorE4[0]=preset->r;expected.context.overlayColorE4[1]=preset->g;expected.context.overlayColorE4[2]=preset->b;
    }
    for(unsigned i=0;i<8;i++)expected.slots[i].overlay70=selector>=5;
    u32 encoded=(scenario*0x9E3779B9U&0xFFFFF00FU)|(selector<<4);
    s32 encodedIndex;memcpy(&encodedIndex,&encoded,4);ApplyTrackLightPreset(encodedIndex);
    assert(!memcmp(&world,&expected,sizeof(world)));
    memcpy(output,results,36);snapshot(output+36);
    put32(output+124,(u32)RaceContext.overlayEnabledD4);put32(output+128,(u32)RaceContext.overlayModeDC);
    memcpy(output+132,RaceContext.overlayColorE4,4);
    for(unsigned i=0;i<8;i++)put32(output+136+i*4,(u32)CarSlotBuffers[i].overlay70);
}
'''


class TrackResourceOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners=source_owners(ROOT/'src/main/PAL')
        bodies='\n'.join(definition_source(owners[n].read_text(),n) for n in NAMES)
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        source=Path(cls.temp.name)/'resource.c';library=Path(cls.temp.name)/'resource.dylib'
        source.write_text(PREFIX+bodies+TAIL)
        proc=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                             '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(library)],capture_output=True,text=True)
        if proc.returncode:raise AssertionError(proc.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_resource.argtypes=[C.c_uint,C.c_void_p,C.c_void_p]

    def test_relocation_seven_tables_and_camera_owner_on_both_images(self):
        context=address_of('RaceContext');self.assertEqual(address_of('TrackEventTables'),context+0x568)
        self.assertEqual(address_of('TrackCameraNodes'),context+0x550)
        self.assertEqual(address_of('TrackLightPresets'),context+0x558)
        self.assertEqual(address_of('SceneOverlayEnabled'),context+0xD4)
        rng=random.Random(0x4D4DC);cases=[];seen_counts=set();seen_results=set()
        for number in range(2048):
            node_count=17+(number%47);car_node=(number*13)%node_count
            key=(-2147483648,-32768,-1,0,1,32767,2147483647)[number%7] if number%3 else (number//3)%101-50
            counts=[(number//(i+1)+i)%9 for i in range(7)];seen_counts.update(counts)
            events=[];pairs=[]
            for table in range(4):
                keys=sorted(rng.sample(range(-80,81),8),reverse=True)
                events.append([(k,node_count-1 if (number+i+table)%3==0 else k-7,rng.getrandbits(32)) for i,k in enumerate(keys)])
            for table in range(3):
                keys=sorted(rng.sample(range(-80,81),8),reverse=True)
                pairs.append([(k,rng.randrange(-32768,32768)) for k in keys])
            n=number%8;nodes=[(number*7+i*11)%node_count if i<n else -1 for i in range(8)]
            presets=[(rng.getrandbits(32),*rng.randbytes(4)) for _ in range(4)]
            raw=struct.pack('<3i7h',node_count,car_node,key,*counts)+b''.join(struct.pack('<hhI',*row) for table in events for row in table)+b''.join(struct.pack('<hh',*row) for table in pairs for row in table)+struct.pack('<8h',*nodes)+b''.join(struct.pack('<I4B',*preset) for preset in presets)
            self.assertEqual(len(raw),426)
            results=[n,min(range(n),key=lambda i:min(abs(nodes[i]-car_node),node_count-abs(nodes[i]-car_node))) if n else 0]
            for selection in range(7):
                if selection in (0,1,6):
                    table=(0,1,2)[(0,1,6).index(selection)];count=counts[(0,1,6)[table]]
                    chosen=[i for i,(k,_) in enumerate(pairs[table][:count]) if k>=key]
                    index=chosen[-1] if chosen else 0
                    result=pairs[table][index][1] if pairs[table][index][0]==key else -1
                else:
                    table=selection-2;count=counts[table+2]
                    chosen=[i for i,(k,_,_) in enumerate(events[table][:count]) if k>=key]
                    result=-1
                    if chosen:
                        _,threshold,value=events[table][chosen[-1]]
                        if key>=0 if threshold==node_count-1 else key>=threshold:
                            result=(value&0x7FFF)-(value&0x8000) if table==3 else (value&0x7FFFFFFF)-(value&0x80000000)
                results.append(result);seen_results.add(result==-1)
            refs=[BASE+PAIR_OFFSETS[0],BASE+PAIR_OFFSETS[1],*(BASE+x for x in EVENT_OFFSETS),BASE+PAIR_OFFSETS[2]]
            descriptor=struct.pack('<7hH7I',*counts,0xA5A5,*refs)
            tail=struct.pack('<7I',BASE+WORKSPACE+0xC20,BASE+WORKSPACE,BASE+WORKSPACE+0xA00,BASE+WORKSPACE+0xA20,0xA5A5A5A5,0,BASE+DIRECTORY)+b'\xA5'*16
            selector=number%256;enabled=int(1<=selector<=7)
            mode,red,green,blue=(presets[(selector-1)%4][:4] if enabled else (0xA5A5A5A5,0xA5,0xA5,0xA5))
            light=struct.pack('<II4B8I',enabled,mode,red,green,blue,0xA5,*([int(selector>=5)]*8))
            wanted=struct.pack('<9i',*results)+descriptor+tail+light
            inp=C.create_string_buffer(raw);out=C.create_string_buffer(168);self.native.run_resource(number,inp,out)
            self.assertEqual(out.raw,wanted,(number,'native'));cases.append((raw,wanted,events,pairs,nodes,results,presets))
        self.assertEqual(seen_counts,set(range(9)));self.assertEqual(seen_results,{False,True})
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
        slots=address_of('CarSlotBuffers')
        allowed=[(context+0xD4,4),(context+0xDC,4),(context+0xE4,3),*((slots+i*0xC8+0x70,4) for i in range(8)),(context+0x550,16),(context+0x568,4),(BASE+DIRECTORY+0x10,28),(STACK-0x80,0x80)]
        def store(cpu,access,address,length,value,user):
            address&=0x1FFFFFFF
            assert any((a&0x1FFFFFFF)<=address and address+length<=(a&0x1FFFFFFF)+n for a,n in allowed),(hex(cpu.pc),hex(address),length)
        cpu=Cpu(memory,0,100,101,102,store)
        def invoke(name,args):
            write(STACK-0x90,b'\xA5'*0xA0);cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
            for i,arg in enumerate(args):cpu.r[4+i]=arg&0xFFFFFFFF
            cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(name)
            for step in range(4096):
                if cpu.pc==STOP:break
                cpu.step()
            self.assertEqual((cpu.pc,cpu.r[29],cpu.r[31]),(STOP,STACK,STOP),name)
            for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
            self.assertEqual(read(STACK-0x90,16),b'\xA5'*16);self.assertEqual(read(STACK,16),b'\xA5'*16)
            return (cpu.r[2]&0x7FFFFFFF)-(cpu.r[2]&0x80000000)
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            image=path.read_bytes();write(struct.unpack_from('<I',image,0x18)[0],image[0x800:])
            for number,(raw,wanted,events,pairs,nodes,results,presets) in enumerate(cases):
                node_count,car_node,key=struct.unpack_from('<3i',raw);counts=struct.unpack_from('<7h',raw,12)
                resource=bytearray(b'\xA5'*IMAGE_SIZE)
                struct.pack_into('<7h',resource,DIRECTORY,*counts)
                ref_offsets=[PAIR_OFFSETS[0],PAIR_OFFSETS[1],*EVENT_OFFSETS,PAIR_OFFSETS[2]]
                struct.pack_into('<7i',resource,DIRECTORY+16,*(off-DIRECTORY for off in ref_offsets))
                for off,table in zip(PAIR_OFFSETS,pairs):resource[off:off+32]=b''.join(struct.pack('<hh',*row) for row in table)
                for off,table in zip(EVENT_OFFSETS,events):resource[off:off+64]=b''.join(struct.pack('<hhI',*row) for row in table)
                for i,value in enumerate(nodes):struct.pack_into('<h',resource,WORKSPACE+0xC20+i*0x44+0x32,value)
                resource[WORKSPACE+0xA00:WORKSPACE+0xA20]=b''.join(struct.pack('<I4B',*preset) for preset in presets)
                slot_bytes=bytearray(b'\xA5'*(16*0xC8))
                write(slots-16,b'\xA6'*16+slot_bytes+b'\x6A'*16)
                ctx=bytearray(b'\xA5'*0x57C)
                for off in (0xE8,0x224,0x24C,0x550,0x554,0x558,0x55C,0x564,0x568):struct.pack_into('<I',ctx,off,0)
                car=bytearray(b'\xA5'*0x320);struct.pack_into('<i',car,0xF0,car_node)
                for a,data in ((context,ctx),(BASE,resource),(CAR,car)):write(a-16,b'\xA6'*16+data+b'\x6A'*16)
                write(address_of('TrackNodeCount'),struct.pack('<i',node_count))
                for selector in (-2147483648,-1,7,8,65535,2147483647):
                    self.assertEqual(invoke('LookupTrackEvent',[selector,key]),0,(number,label,'invalid selector'))
                    self.assertEqual(read(context,0x57C),bytes(ctx));self.assertEqual(read(BASE,IMAGE_SIZE),bytes(resource))
                calls=[('RelocateTrackLookupTables',[BASE+DIRECTORY]),('SetTrackWorkspace',[BASE+WORKSPACE])]
                if number&1:calls.reverse()
                for name,args in calls:invoke(name,args)
                got=[invoke('CountTrackEntries',[]),invoke('FindNearestTrackEntry',[CAR])]+[invoke('LookupTrackEvent',[i,key]) for i in range(7)]
                self.assertEqual(got,results,(number,label))
                selector=number%256;encoded=(number*0x9E3779B9&0xFFFFF00F)|(selector<<4)
                invoke('ApplyTrackLightPreset',[encoded])
                light=read(context+0xD4,4)+read(context+0xDC,4)+read(context+0xE4,4)+b''.join(read(slots+i*0xC8+0x70,4) for i in range(8))
                self.assertEqual(light,wanted[124:],(number,label,'light'))
                ctx[0xD4:0xD8]=wanted[124:128];ctx[0xDC:0xE0]=wanted[128:132];ctx[0xE4:0xE8]=wanted[132:136]
                for i in range(8):slot_bytes[i*0xC8+0x70:i*0xC8+0x74]=wanted[136+4*i:140+4*i]
                self.assertEqual(read(slots,len(slot_bytes)),bytes(slot_bytes))
                self.assertEqual(read(slots-16,16),b'\xA6'*16);self.assertEqual(read(slots+len(slot_bytes),16),b'\x6A'*16)
                self.assertEqual(struct.pack('<9i',*got)+read(BASE+DIRECTORY,44)+read(context+0x550,44)+light,wanted,(number,label,'snapshot'))
                resource[DIRECTORY+16:DIRECTORY+44]=wanted[36+16:36+44];ctx[0x550:0x57C]=wanted[80:124]
                self.assertEqual(read(BASE,IMAGE_SIZE),bytes(resource));self.assertEqual(read(context,0x57C),bytes(ctx));self.assertEqual(read(CAR,0x320),bytes(car))
                for a,n in ((context,0x57C),(BASE,IMAGE_SIZE),(CAR,0x320)):
                    self.assertEqual(read(a-16,16),b'\xA6'*16);self.assertEqual(read(a+n,16),b'\x6A'*16)


if __name__=='__main__':unittest.main()
