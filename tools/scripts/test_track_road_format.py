"""Actual wheel-contact prefix: canonical resource-to-scratch subrecord copies.

Runs the current complete C definition until its first sine service boundary,
after both axle searches and all four copies. One sweep uses mutable search
adapters; another executes the actual search with a scalar NCLIP model. The
remaining contact arithmetic is outside this test. Original and linked MIPS
execute to the same boundary. The actual search's three existing register
annotations are removed only for host compilation; its body is otherwise intact.
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
from test_wheel_bump_owner import Cpu
from mips_preview_engine import signed

ROOT = Path(__file__).resolve().parents[2]
VEHICLE, CONTACT = 0x80180010, 0x1F800080
RESOURCES = (0x801A0010, 0x801B0010)
PREFIX, WORK, TABLE = 0x1F4, 0x260, 8*0x3C
STATE = PREFIX + WORK + 2*TABLE + 12

PREFIX_C = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "game/car/carsp.h"
#include "game/car/carmove1.h"
enum {PREFIX=0x1F4, WORK=0x260, TABLE=8*0x3C, STATE=PREFIX+WORK+2*TABLE+12};
typedef struct {u32 before[4];TrackSample3C values[8];u32 after[4];} Track;
static struct {
    u32 beforeVehicle[4];RaceVehicle vehicle;u32 afterVehicle[4];
    u32 beforeWork[4];CarTrackWork contact;u32 afterWork[4];
    Track tracks[2];TrackSample3C *root;s32 count,bank;
} world;
static unsigned number,events,searches,copyBanks[2];
static s32 chosen[2];static u8 *trace;static jmp_buf boundary;
#define SCRATCHPAD ((uintptr_t)&world.bank-0x7CU)
#define SCRATCH_CAR_WORK (&world.contact)
#define TrackNodes world.root
#define TrackNodeCount world.count
/* The observed boundary precedes every GTE operation. */
#define RR_GTE_SETROT(p) abort()
#define RR_GTE_MVMVA_ROT_V0_NOCV(p) abort()
#define RR_GTE_STORE_IR(p) abort()
static void snapshot(u8 *out) {
    u32 address=0x80180010,root;
    assert(world.contact.vehicle00==&world.vehicle);
    assert(world.root==world.tracks[0].values||world.root==world.tracks[1].values);
    root=world.root==world.tracks[0].values?0x801A0010:0x801B0010;
    memcpy(out,&world.vehicle,PREFIX);out+=PREFIX;
    memcpy(out,&address,4);memcpy(out+4,&world.contact.w4,WORK-4);out+=WORK;
    memcpy(out,world.tracks[0].values,TABLE);out+=TABLE;
    memcpy(out,world.tracks[1].values,TABLE);out+=TABLE;
    memcpy(out,&root,4);memcpy(out+4,&world.count,4);memcpy(out+8,&world.bank,4);
}
static void event(u32 kind,s32 value) {
    u32 words[2]={kind,(u32)value};assert(events<3);
    memcpy(trace,words,8);snapshot(trace+8);trace+=8+STATE;events++;
}
void FindAxleTrackNode(void) {
    assert(searches<2&&world.contact.wheelIndex08==(s32)searches*2);
    event(1,world.contact.wheelIndex08);
    copyBanks[searches]=(number>>(searches+1))&1;
    world.root=world.tracks[copyBanks[searches]].values;
    world.contact.wheelState150.trackNodes[world.contact.wheelIndex08]=chosen[searches];
    searches++;
}
s32 rsin(s32 angle) {
    assert(searches==2);event(2,angle);longjmp(boundary,1);
}
s32 rcos(s32 angle) {(void)angle;abort();}
'''

TAIL_C = r'''
void run_contact(unsigned scenario,const u8 *input,u8 *output) {
    unsigned i,j;RaceVehicle before;CarTrackWork oldContact;
    number=scenario;events=searches=0;trace=output+STATE+4;
    memset(&world,0xA5,sizeof(world));
    memcpy(&world.vehicle,input,PREFIX);input+=PREFIX;
    world.vehicle.control.record18=NULL;world.vehicle.control.handling1C=NULL;
    memcpy(&world.contact.w4,input+4,WORK-4);input+=WORK;
    world.contact.vehicle00=&world.vehicle;
    for(i=0;i<2;i++){memcpy(world.tracks[i].values,input,TABLE);input+=TABLE;}
    memcpy(&world.count,input,4);memcpy(chosen,input+4,8);
    world.bank=scenario&1;world.root=world.tracks[scenario&1].values;
    assert(sizeof(void*)==8&&(uintptr_t)world.root>UINT32_MAX);
    assert((uintptr_t)&world.contact>UINT32_MAX&&(uintptr_t)&world.vehicle>UINT32_MAX);
    memcpy(&before,&world.vehicle,sizeof(before));memcpy(&oldContact,&world.contact,sizeof(oldContact));
    if(!setjmp(boundary)){FindWheelTrackNodes(&world.vehicle);abort();}
    assert(events==3&&searches==2);
    for(i=0;i<2;i++)for(j=0;j<2;j++){
        unsigned index=(chosen[i]+j)%world.count;
        assert(!memcmp(&world.contact.road180[2*i+j],
                       &world.tracks[copyBanks[i]].values[index].road,sizeof(TrackRoadSample34)));
    }
    assert(!memcmp((u8*)&world.vehicle+PREFIX,(u8*)&before+PREFIX,sizeof(before)-PREFIX));
    assert(!memcmp((u8*)&world.contact+0x264,(u8*)&oldContact+0x264,sizeof(oldContact)-0x264));
    for(i=0;i<4;i++){
        assert(world.beforeVehicle[i]==0xA5A5A5A5&&world.afterVehicle[i]==0xA5A5A5A5);
        assert(world.beforeWork[i]==0xA5A5A5A5&&world.afterWork[i]==0xA5A5A5A5);
        for(j=0;j<2;j++)assert(world.tracks[j].before[i]==0xA5A5A5A5&&world.tracks[j].after[i]==0xA5A5A5A5);
    }
    snapshot(output);memcpy(output+STATE,&events,4);
}
'''


NCLIP_C = r'''
static s32 nclip(s32 a,s32 b,s32 c) {
    s16 x[3],z[3];s32 values[3]={a,b,c},result;u32 bits;unsigned i;
    for(i=0;i<3;i++){
        u16 low=(u16)(u32)values[i],high=(u16)((u32)values[i]>>16);
        memcpy(&x[i],&low,2);memcpy(&z[i],&high,2);
    }
    bits=(u32)((int64_t)x[0]*(z[1]-z[2])+(int64_t)x[1]*(z[2]-z[0])+
               (int64_t)x[2]*(z[0]-z[1]));
    memcpy(&result,&bits,4);event(3,result);return result;
}
#define RR_GTE_NCLIP3(a,b,c,out) (*(out)=nclip(a,b,c))
'''


class SearchCpu(Cpu):
    def reset(self):
        super().reset();self.gte = [0]*32

    def nclip(self):
        x = [signed(self.gte[i],16) for i in (12,13,14)]
        z = [signed(self.gte[i] >> 16,16) for i in (12,13,14)]
        return (x[0]*(z[1]-z[2])+x[1]*(z[2]-z[0])+x[2]*(z[0]-z[1])) & 0xFFFFFFFF

    def step(self):
        word = self.load(self.pc,4)
        if word == 0x4B400006:
            self.gte[24] = self.nclip()
        elif word >> 26 == 0x12 and (word >> 21) & 31 == 4:
            self.gte[(word >> 11) & 31] = self.r[(word >> 16) & 31]
        elif word >> 26 == 0x3A:
            address = (self.r[(word >> 21) & 31] + signed(word,16)) & 0xFFFFFFFF
            self.store(address,self.gte[(word >> 16) & 31],4)
        else:
            super().step();return
        old,self.delay = self.delay,None
        self.pc = old if old is not None else self.pc+4


class TrackRoadFormatTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners = source_owners(ROOT/'src/main/PAL')
        body = definition_source(owners['FindWheelTrackNodes'].read_text(), 'FindWheelTrackNodes')
        cls.temporary = TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory = Path(cls.temporary.name)
        cls.libraries = {}
        for actual_search in (False,True):
            prefix,tail = PREFIX_C,TAIL_C
            if actual_search:
                search = definition_source(owners['FindAxleTrackNode'].read_text(),'FindAxleTrackNode')
                search,pins = re.subn(r'\s+__asm__\("\$\d+"\)','',search)
                if pins != 3:
                    raise AssertionError(('review changed search constraints',pins))
                prefix = re.sub(r'void FindAxleTrackNode\(void\) \{.*?^\}',NCLIP_C+search,prefix,flags=re.S|re.M)
                prefix = prefix.replace('assert(events<3)','assert(events<40)')
                prefix = prefix.replace('assert(searches==2);event(2,angle)','event(2,angle)')
                tail = tail.replace('if(!setjmp(boundary))','copyBanks[0]=copyBanks[1]=scenario&1;\n    if(!setjmp(boundary))')
                tail = tail.replace('assert(events==3&&searches==2)','assert(events>2)')
            source,library = directory/f'native{int(actual_search)}.c',directory/f'native{int(actual_search)}.dylib'
            source.write_text(prefix + body + tail)
            built = subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                                    '-Wno-unused-variable', # Existing search locals are separate cleanup debt.
                                    '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                                    '-I', str(ROOT/'include'), str(source), '-o', str(library)],
                                   capture_output=True, text=True)
            if built.returncode:
                raise AssertionError(built.stderr)
            lib = C.CDLL(str(library));lib.run_contact.argtypes = [C.c_uint, C.c_void_p, C.c_void_p]
            cls.libraries[actual_search] = lib

    def test_four_subrecord_copies_wrapping_indices_and_replaced_roots(self):
        self.exercise(False)

    def test_real_search_and_contact_share_the_complete_native_owner(self):
        self.exercise(True)

    def exercise(self,actual_search):
        memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0, 0x400000);memory.mem_map(0x1F800000, 0x1000)
        read = lambda a,n: bytes(memory.mem_read(a & 0x1FFFFFFF,n))
        write = lambda a,b: memory.mem_write(a & 0x1FFFFFFF,bytes(b))
        put32 = lambda a,v: write(a,struct.pack('<I',v & 0xFFFFFFFF))
        start, search, sine = (address_of(n) for n in ('FindWheelTrackNodes','FindAxleTrackNode','rsin'))
        root, count_address = address_of('TrackNodes'), address_of('TrackNodeCount')
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        linked = (ROOT/'build/PAL/main.exe').read_bytes()
        base = struct.unpack_from('<I',original,0x18)[0]
        size = next(int(row.split()[2],16) for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines()
                    if row.split()[0]=='FindWheelTrackNodes')
        search_size = next(int(row.split()[2],16) for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines()
                           if row.split()[0]=='FindAxleTrackNode')
        stack, stop = 0x803F0000, 0x803FF000
        randomizer = random.Random(0x34CA4)
        seen_banks, seen_indices = set(), set()

        def snapshot():
            return (read(VEHICLE,PREFIX)+read(CONTACT,WORK)+b''.join(read(a,TABLE) for a in RESOURCES)+
                    read(root,4)+read(count_address,4)+read(CONTACT-4,4))

        def store(cpu,access,address,width,value,user):
            address &= 0x1FFFFFFF
            self.assertTrue(any(a<=address and address+width<=b for a,b in
                                ((VEHICLE & 0x1FFFFFFF,(VEHICLE & 0x1FFFFFFF)+PREFIX),
                                 (CONTACT,CONTACT+WORK),((stack & 0x1FFFFFFF)-0x200,stack & 0x1FFFFFFF))),
                            (scenario,hex(cpu.pc),hex(address),width))

        cpu = SearchCpu(memory,0,100,101,102,store)
        search_attempts = set()
        for scenario in range(256 if actual_search else 512):
            vehicle = bytearray(randomizer.randbytes(PREFIX))
            work = bytearray(randomizer.randbytes(WORK))
            for offset in (*range(0x10,0x1C,4),*range(0x88,0xC8,4)):
                struct.pack_into('<i',vehicle,offset,randomizer.randrange(-4096,4097))
            for wheel in range(4):struct.pack_into('<h',vehicle,0xF8+wheel*2,(scenario+wheel)%8)
            struct.pack_into('<I',work,0,VEHICLE)
            tables = bytearray(randomizer.randbytes(2*TABLE))
            count = (2,3,5,7,8)[scenario%5]
            chosen = (count-1 if scenario & 8 else scenario%count,
                      count-1 if scenario & 16 else (scenario//7)%count)
            if actual_search:
                count=4;chosen=(scenario%4,(scenario//4)%4)
                centers=((0,0),(0,-1000),(1000,-1000),(1000,0))
                edges=((100,-100),(100,100),(-100,100),(-100,-100))
                queries=((0,-500),(500,-1000),(1000,-500),(500,0))
                for bank in range(2):
                    for i,((x,z),(dx,dz)) in enumerate(zip(centers,edges)):
                        struct.pack_into('<4h2i',tables,bank*TABLE+i*0x3C,dx,dz,-dx,-dz,x,z)
                translation=[struct.unpack_from('<i',vehicle,offset)[0]>>2 for offset in (0x10,0x18)]
                for wheel in range(4):
                    x,z=queries[chosen[wheel//2]]
                    struct.pack_into('<i',vehicle,0x88+wheel*16,(x-translation[0])*4)
                    struct.pack_into('<i',vehicle,0x90+wheel*16,(z-translation[1])*4)
                    struct.pack_into('<h',vehicle,0xF8+wheel*2,(scenario//16+wheel)%4)
            for i,index in enumerate(chosen):seen_indices.add((i,index==count-1))
            seen_banks.add(((scenario>>1)&1,(scenario>>2)&1))
            data = bytes(vehicle+work)+tables+struct.pack('<3i',count,*chosen)
            native_input = C.create_string_buffer(data)
            output = C.create_string_buffer(STATE+4+40*(STATE+8))
            self.libraries[actual_search].run_contact(scenario,native_input,output)
            expected = output.raw
            event_count=struct.unpack_from('<I',expected,STATE)[0]
            if not actual_search:self.assertEqual(event_count,3)
            for image in (original,linked):
                write(start,image[0x800+start-base:0x800+start-base+size])
                if actual_search:write(search,image[0x800+search-base:0x800+search-base+search_size])
                write(VEHICLE-16,b'\xA6'*16+vehicle+b'\x6A'*16)
                write(CONTACT-16,b'\xA6'*12+struct.pack('<i',scenario&1)+work+b'\x6A'*16)
                for i,address in enumerate(RESOURCES):write(address-16,b'\xA6'*16+tables[i*TABLE:(i+1)*TABLE]+b'\x6A'*16)
                put32(root,RESOURCES[scenario&1]);put32(count_address,count)
                write(stack-0x210,b'\xA5'*0x220)
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[29],cpu.r[31],cpu.pc=VEHICLE,stack,stop,start
                events=0
                for instruction in range(5000):
                    nclip = actual_search and cpu.load(cpu.pc,4)==0x4B400006
                    if cpu.pc!=sine and (actual_search or cpu.pc!=search) and not nclip:
                        cpu.step();continue
                    if nclip:
                        event=(3,cpu.nclip())
                        search_attempts.add(struct.unpack('<i',read(CONTACT+0x38,4))[0])
                    elif cpu.pc==search:
                        event=(1,events*2)
                    else:
                        if not actual_search:self.assertEqual(events,2)
                        event=(2,cpu.r[4])
                    pos=STATE+4+events*(STATE+8)
                    self.assertEqual(struct.pack('<2I',*event),expected[pos:pos+8],(scenario,events))
                    self.assertEqual(snapshot(),expected[pos+8:pos+8+STATE],(scenario,events,'service boundary'))
                    if cpu.pc==sine:break
                    if nclip:
                        cpu.step();events+=1;continue
                    bank=(scenario>>(events+1))&1
                    put32(root,RESOURCES[bank])
                    write(CONTACT+0x150+4*events,struct.pack('<h',chosen[events]))
                    resume=cpu.r[31]
                    for register in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[register]=0xDEAD0000+register
                    cpu.hi,cpu.lo,cpu.pc=0x12345678,0x87654321,resume;events+=1
                self.assertEqual(cpu.pc,sine,scenario)
                self.assertEqual(events+1,event_count)
                self.assertEqual(snapshot(),expected[:STATE],(scenario,'completed copy phase'))
                self.assertEqual(read(VEHICLE-16,16),b'\xA6'*16)
                self.assertEqual(read(VEHICLE+PREFIX,16),b'\x6A'*16)
                self.assertEqual(read(CONTACT-16,12),b'\xA6'*12)
                self.assertEqual(read(CONTACT+WORK,16),b'\x6A'*16)
                for address in RESOURCES:
                    self.assertEqual(read(address-16,16),b'\xA6'*16)
                    self.assertEqual(read(address+TABLE,16),b'\x6A'*16)
                self.assertEqual(read(stack-0x210,16),b'\xA5'*16)
                self.assertEqual(read(stack,16),b'\xA5'*16)
        self.assertEqual(seen_banks,{(a,b) for a in range(2) for b in range(2)})
        self.assertEqual(seen_indices,{(i,wrapped) for i in range(2) for wrapped in (False,True)})
        if actual_search:self.assertTrue({0,1,2,3}<=search_attempts,search_attempts)


if __name__=='__main__':
    unittest.main()
