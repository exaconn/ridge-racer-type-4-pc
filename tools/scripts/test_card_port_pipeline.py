"""Actual card-port controller, event helpers, sync and clear on C/original/linked MIPS.

Only BIOS services and the command-start boundary are controlled. No C body
rewrites, low mappings, synthetic pointer values or substituted MemCardSync.
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
STACK,STOP=0x803F0000,0x803FF000
PARTS=(('before',16),('states',8),('timers',8),('last',8),('results',8),('loaded',8),
       ('raw',48),('hw',16),('sw',16),('command',4),('status',4),('tx',4),('modeCell',4),('after',16))
OFFSETS={};SIZE=0
for name,length in PARTS:OFFSETS[name]=SIZE;SIZE+=length
NAMES=('PollHwCardEvents','ClearHwCardEvents','ClearSwCardEvents','WaitSwCardEvent',
       'MemCardSync','_card_clear','CardUpdate')

NATIVE=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "game/data/cardini.h"
#include "psyq/libcard/cardsts.h"
#include "psyq/libcard/cardc162.h"
#include "psyq/libgpu/gpuctl.h"
#include "rr/bios_devices.h"
#include "rr/bios_events.h"
#include "rr/commands.h"
typedef struct {
 u32 before[4];s32 states[2],timers[2],last[2],results[2],loaded[2];
 u8 raw[RR_CARD_RAW_SCRATCH_BYTES];u32 hw[4],sw[4];
 s32 command;volatile s32 status,tx;
 u8 modeCell[4];u32 after[4];
} World;
typedef struct {s32 port,slot,begin;u32 mutate,seed;s32 replies[32];} Plan;
static World actual;static Plan plan;static const u8 *expected;
static unsigned eventIndex,replyIndex;
#define CardPortState actual.states
#define CardPortTimer actual.timers
#define CardPortLastResult actual.last
#define CardPortResult actual.results
#define CardPortLoaded actual.loaded
#define CardRawSectorWorkBuf actual.raw
#define HwCardEvIoe actual.hw[0]
#define HwCardEvError actual.hw[1]
#define HwCardEvTimeout actual.hw[2]
#define HwCardEvNew actual.hw[3]
#define SwCardEvIoe actual.sw[0]
#define SwCardEvError actual.sw[1]
#define SwCardEvTimeout actual.sw[2]
#define SwCardEvNew actual.sw[3]
#define CardCurrentCommand actual.command
#define CardCommandStatus actual.status
#define CardTxCount actual.tx
#define D_80114741 actual.modeCell[0]
static u32 get(const u8 *p){u32 v;memcpy(&v,p,4);return v;}
static s32 word(u32 v){s32 r;memcpy(&r,&v,4);return r;}
static const u8 *start(unsigned kind,s32 a,s32 b,s32 c){
 assert(eventIndex<get(expected));
 const u8 *event=expected+12+sizeof(World)+eventIndex*(16+2*sizeof(World));
 if(get(event)!=kind||get(event+4)!=(u32)a||get(event+8)!=(u32)b||get(event+12)!=(u32)c||memcmp(&actual,event+16,sizeof(actual))){
  fprintf(stderr,"port=%d slot=%d phase=%d event=%u kind=%u seed=%u\n",plan.port,plan.slot,actual.states[plan.port],eventIndex,kind,plan.seed);
  assert(0);
 }
 return event;
}
static void change(unsigned kind){
 if(!plan.mutate)return;
 u32 seed=plan.seed+(eventIndex+1)*0x9E3779B9U+kind*0x10203U;
 unsigned port=(unsigned)plan.port,other=1-port;
 actual.timers[port]=(s32)(seed%124)-2;
 actual.loaded[port]=(s32)((seed>>8)%3)-1;
 actual.results[port]=(s32)((seed>>16)%9)-4;
 actual.states[other]=(s32)((seed>>20)%9)-1;
 actual.last[port]=word((u32)actual.last[port]^seed);
 actual.raw[47]^=(u8)seed;
 actual.status=(s32)((seed>>4)&63);actual.tx=(s32)(seed&7);
 actual.command=(s32)((seed>>12)&15);actual.modeCell[0]^=0x5A;
}
static void finish(const u8 *event){assert(!memcmp(&actual,event+16+sizeof(World),sizeof(actual)));eventIndex++;}
s32 TestEvent(BiosEventHandle handle){
 unsigned index;
 for(index=0;index<8;index++)if(handle==(index<4?actual.hw[index]:actual.sw[index-4]))break;
 assert(index<8&&replyIndex<32);const u8 *event=start(1,(s32)index,0,0);
 s32 result=plan.replies[replyIndex++];change(1);finish(event);return result;
}
s32 _card_info(s32 channel){const u8 *event=start(2,channel,0,0);change(2);finish(event);return word(plan.seed^0xDEADBEEFU);}
s32 _card_load(s32 channel){const u8 *event=start(3,channel,0,0);change(3);finish(event);return word(plan.seed^0x8F010203U);}
s32 _cardCmd5ABegin(s32 channel,u8 *buffer){
 assert(buffer==actual.raw+plan.port*RR_CARD_RAW_REPLY_BYTES);
 assert(sizeof(void*)==8&&(uintptr_t)buffer>UINT32_MAX);
 const u8 *event=start(4,channel,(s32)(buffer-actual.raw),0);change(4);
 if(plan.begin){
  for(unsigned i=0;i<RR_CARD_RAW_REPLY_BYTES;i++)buffer[i]^=(u8)(plan.seed+i*19);
  actual.command=12;actual.status=2;
 }
 finish(event);return plan.begin;
}
void _new_card(void){const u8 *event=start(5,0,0,0);change(5);finish(event);}
s32 _card_write(s32 channel,s32 sector,const u8 *buffer){
 assert(buffer==NULL);const u8 *event=start(6,channel,sector,0);change(6);finish(event);return -123;
}
'''

TAIL=r'''
void run_driver(const u8 *input,const u8 *script,const u8 *trace){
 memcpy(&actual,input,sizeof(actual));memcpy(&plan,script,sizeof(plan));expected=trace;eventIndex=replyIndex=0;
 assert(plan.port>=0&&plan.port<2&&plan.slot>=0&&plan.slot<256);
 assert((uintptr_t)actual.raw>UINT32_MAX);
 s32 result=CardUpdate(plan.port,plan.slot);
 assert((u32)result==get(trace+4)&&replyIndex==get(trace+8)&&eventIndex==get(trace));
 assert(!memcmp(&actual,trace+12,sizeof(actual)));
}
'''


def signed(value):
    value&=0xFFFFFFFF
    return value-(1<<32) if value&(1<<31) else value


def service_effect(current,plan,event_index,kind):
    """Apply scripted I/O to actual MIPS state, independently of expected traces."""
    state=bytearray(current);port,slot,begin,mutate,seed=plan[:5]
    def get(name,index=0):return struct.unpack_from('<I',state,OFFSETS[name]+4*index)[0]
    def put(name,value,index=0):struct.pack_into('<I',state,OFFSETS[name]+4*index,value&0xFFFFFFFF)
    if mutate:
        value=(seed+(event_index+1)*0x9E3779B9+kind*0x10203)&0xFFFFFFFF
        put('timers',value%124-2,port);put('loaded',(value>>8)%3-1,port)
        put('results',(value>>16)%9-4,port);put('states',(value>>20)%9-1,1-port)
        put('last',get('last',port)^value,port);state[OFFSETS['raw']+47]^=value&255
        put('status',(value>>4)&63);put('tx',value&7);put('command',(value>>12)&15)
        state[OFFSETS['modeCell']]^=0x5A
    if kind==4 and begin:
        for i in range(17):state[OFFSETS['raw']+port*17+i]^=(seed+i*19)&255
        put('command',12);put('status',2)
    return bytes(state)


class Model:
    def __init__(self,state,plan):
        self.state=bytearray(state);self.port,self.slot,self.begin,self.mutate,self.seed,*self.replies=plan
        self.events=[];self.replyIndex=0;self.coverage=set()
    def get(self,name,index=0):return struct.unpack_from('<i',self.state,OFFSETS[name]+index*4)[0]
    def put(self,name,value,index=0):struct.pack_into('<I',self.state,OFFSETS[name]+index*4,value&0xFFFFFFFF)
    def field(self,name):return self.get(name,self.port)
    def set(self,name,value):self.put(name,value,self.port)
    def service(self,kind,a=0,b=0,c=0):
        before=bytes(self.state);result=0
        if kind==1:
            result=self.replies[self.replyIndex];self.replyIndex+=1
        if self.mutate:
            seed=(self.seed+(len(self.events)+1)*0x9E3779B9+kind*0x10203)&0xFFFFFFFF
            self.set('timers',seed%124-2);self.set('loaded',(seed>>8)%3-1);self.set('results',(seed>>16)%9-4)
            self.put('states',(seed>>20)%9-1,1-self.port)
            self.set('last',self.field('last')^seed);self.state[OFFSETS['raw']+47]^=seed&255
            self.put('status',(seed>>4)&63);self.put('tx',seed&7);self.put('command',(seed>>12)&15)
            self.state[OFFSETS['modeCell']]^=0x5A
        if kind==4:
            result=self.begin
            if result:
                for i in range(17):self.state[OFFSETS['raw']+self.port*17+i]^=(self.seed+i*19)&255
                self.put('command',12);self.put('status',2)
        self.events.append(struct.pack('<4i',kind,a,b,c)+before+bytes(self.state))
        return result
    def poll(self):
        responses=[self.service(1,i) for i in range(4)]
        # BIOS event priority is NEW > TIMEOUT > ERROR > IOE.
        event=next((i+1 for i in range(3,-1,-1) if responses[i]==1),0)
        timer=self.field('timers');self.set('timers',timer+1)
        self.coverage.add(('poll_timeout',timer>=120))
        return 2 if timer>=120 else event
    def clear(self,channel):
        for i in range(4):self.service(1,i+4)
        self.service(5);self.service(6,channel,63)
        while True:
            for i in range(4):
                if self.service(1,i+4)==1:return
    def sync(self):
        command,status=self.get('command'),self.get('status')
        if command==0 or status==1:return -1,0
        if status in (2,4):return 0,0
        result=(1 if self.get('tx')==1 else 2) if status==17 else 2 if status==33 else 3 if self.state[OFFSETS['modeCell']]==8 else 0
        self.put('command',0);return 1,result
    def run(self):
        phase=self.field('states');channel=self.port*16+self.slot;self.coverage.add(('phase',phase))
        if phase==0:
            self.service(2,channel);self.set('states',1);self.set('timers',0);self.set('last',0)
        elif phase==1:
            event=self.poll();self.coverage.add(('info_event',event))
            if event:
                result,nextPhase,loaded={1:(1,2,None),3:(-1,6,0),4:(2,2,None)}.get(event,(-4,6,0))
                self.set('results',result);self.set('states',nextPhase)
                if loaded is None:self.set('timers',0)
                else:self.set('loaded',loaded)
        elif phase==2:
            success=self.service(4,channel,self.port*17);self.coverage.add(('begin',success))
            if success:self.set('states',3);self.set('timers',0)
            else:
                timer=self.field('timers');self.set('timers',timer+1);self.coverage.add(('begin_timeout',timer>=120))
                if timer>=120:self.set('results',-4);self.set('states',6);self.set('loaded',0)
        elif phase==3:
            completion,result=self.sync();self.coverage.add(('sync',completion))
            if completion<0:self.set('results',-4);self.set('states',6);self.set('loaded',0)
            elif completion==0:
                timer=self.field('timers');self.set('timers',timer+1);self.coverage.add(('sync_timeout',timer>=120))
                if timer>=120:self.set('results',-4);self.set('states',6);self.set('loaded',0)
            else:
                self.coverage.add(('sync_result',result))
                if result==1:self.set('results',-1);self.set('states',6);self.set('loaded',0)
                elif result==0 or (result==2 and self.field('results')==1):
                    if result==0:self.set('results',3)
                    self.set('states',6 if self.field('loaded')==1 else 4)
                else:
                    self.set('results',2 if result==2 else 4);self.clear(channel);self.set('states',4);self.set('loaded',0)
        elif phase==4:
            for i in range(4):self.service(1,i)
            self.service(3,channel);self.set('states',5);self.set('timers',0)
        elif phase==5:
            event=self.poll();self.coverage.add(('load_event',event))
            if event:
                self.set('states',6)
                if event==1:self.set('loaded',1)
                else:
                    prior=self.field('results')
                    result=-1 if event==3 else -2 if event==4 and prior in (1,2) else -3 if event==4 and prior in (3,4) else prior if event==4 else -4
                    self.set('results',result);self.set('loaded',0)
        elif phase==6:self.set('last',self.field('results'));self.set('states',0)
        return struct.pack('<IiI',len(self.events),self.field('last'),self.replyIndex)+bytes(self.state)+b''.join(self.events)


class CardPortPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        p=Path(cls.temp.name)/'driver.c';library=p.with_suffix('.dylib');owners=source_owners(ROOT/'src/main/PAL')
        assertions='\n'.join('_Static_assert(offsetof(World,%s)==%d,"wire %s");'%(name,OFFSETS[name],name) for name,_ in PARTS)
        assertions+=f'\n_Static_assert(sizeof(World)=={SIZE},"complete world");\n_Static_assert(sizeof(Plan)==148,"complete plan");\n'
        bodies='\n'.join(definition_source(owners[name].read_text(),name) for name in NAMES)
        p.write_text(NATIVE+assertions+bodies+TAIL)
        result=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-funsigned-char','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p),'-o',str(library)],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_driver.argtypes=[C.c_void_p]*3

    def test_numeric_channels_actual_sync_all_port_phases_and_service_traces(self):
        bindings=[]
        for symbol,name,length in (('CardPortState','states',8),('CardPortTimer','timers',8),('CardPortLastResult','last',8),
                                   ('CardPortResult','results',8),('CardPortLoaded','loaded',8),('CardRawSectorWorkBuf','raw',48),
                                   ('CardCurrentCommand','command',4),('CardCommandStatus','status',4),('CardTxCount','tx',4),('D_80114741','modeCell',4)):
            bindings.append((address_of(symbol),OFFSETS[name],length))
        for group,labels in (('hw',('HwCardEvIoe','HwCardEvError','HwCardEvTimeout','HwCardEvNew')),
                             ('sw',('SwCardEvIoe','SwCardEvError','SwCardEvTimeout','SwCardEvNew'))):
            bindings.extend((address_of(name),OFFSETS[group]+i*4,4) for i,name in enumerate(labels))
        services={address_of(name):kind for name,kind in (('TestEvent',1),('_card_info',2),('_card_load',3),('_cardCmd5ABegin',4),('_new_card',5),('_card_write',6))}
        entry=address_of('CardUpdate');raw=address_of('CardRawSectorWorkBuf');machines=[]
        for label,path in (('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')):
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
        rng=random.Random(0x43415244);coverage=set();cases=0
        for phase in (-1,0,1,2,3,4,5,6,7):
            for port in range(2):
                for profile in range(256):
                    initial=bytearray(rng.randbytes(SIZE));m=Model(initial,[port,profile,(profile>>2)&1,(profile>>7)&1,rng.getrandbits(32)]+[0]*32)
                    for i in range(2):
                        for name in ('states','timers','last','results','loaded'):m.put(name,rng.randrange(-4,8),i)
                    m.set('states',phase);m.set('timers',(-2147483648,-2,-1,0,118,119,120,121,2147483646)[profile%9])
                    m.set('results',(profile//11)%9-4);m.set('loaded',(profile//7)%3-1)
                    m.put('command',(0,1,12)[profile%3]);m.put('status',(0,1,2,4,5,17,33,65)[(profile//3)%8]);m.put('tx',(0,1,2,7)[(profile//5)%4])
                    m.state[OFFSETS['modeCell']]=8 if (profile//13)&1 else 0
                    for i in range(8):m.put('hw' if i<4 else 'sw',0xA0010200+i*17,i%4)
                    m.replies=[rng.choice((-1,0,1,2)) for _ in range(28)]+[1]*4
                    for i in range(4):m.replies[i]=1 if profile&(1<<i) else (0,2,-1)[(profile//16)%3]
                    initial=bytes(m.state);plan=[port,profile,m.begin,m.mutate,m.seed]+m.replies
                    script=struct.pack('<3i2I32i',*plan);trace=m.run();coverage|=m.coverage;cases+=1
                    self.native.run_driver(C.create_string_buffer(initial),C.create_string_buffer(script),C.create_string_buffer(trace))
                    for label,memory in machines:
                        read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                        write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                        for address,offset,length in bindings:write(address,initial[offset:offset+length])
                        write(STACK-0x410,b'\xA5'*0x450)
                        allowed=[(p,n) for p,off,n in bindings if off in [OFFSETS[k] for k in ('states','timers','last','results','loaded','command')]]+[(STACK-0x400,0x400)]
                        def store(cpu,access,address,length,value,user):
                            self.assertTrue(any((p&0x1FFFFFFF)<=(address&0x1FFFFFFF) and (address&0x1FFFFFFF)+length<=(p&0x1FFFFFFF)+n for p,n in allowed),(cases,label,hex(cpu.pc),hex(address)))
                        cpu=Cpu(memory,0,100,101,102,store);cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                        cpu.r[4],cpu.r[5],cpu.r[29],cpu.r[31],cpu.pc=port,profile,STACK,STOP,entry;eventIndex=replyIndex=0
                        def check(wanted):
                            for address,offset,length in bindings:self.assertEqual(read(address,length),wanted[offset:offset+length],(cases,label,phase,profile,hex(address)))
                        for _ in range(20000):
                            if cpu.pc==STOP:break
                            if cpu.pc not in services:cpu.step();continue
                            kind=services[cpu.pc];event=m.events[eventIndex];a,b,c=cpu.r[4:7];result=0
                            if kind==1:
                                handles=[struct.unpack_from('<I',initial,OFFSETS['hw' if i<4 else 'sw']+(i%4)*4)[0] for i in range(8)]
                                a=handles.index(a);b=c=0;result=m.replies[replyIndex];replyIndex+=1
                            elif kind in (2,3):a=signed(a);b=c=0;result=signed(m.seed^(0xDEADBEEF if kind==2 else 0x8F010203))
                            elif kind==4:
                                self.assertEqual(b,raw+port*17);a=signed(a);b-=raw;c=0;result=m.begin
                            elif kind==5:a=b=c=0
                            else:a=signed(a);b=signed(b);self.assertEqual(c,0);result=-123
                            self.assertEqual(struct.unpack_from('<4i',event),(kind,a,b,c),(cases,label,eventIndex))
                            check(event[16:16+SIZE])
                            current=bytearray(initial)
                            for address,offset,length in bindings:current[offset:offset+length]=read(address,length)
                            after=service_effect(current,plan,eventIndex,kind)
                            for address,offset,length in bindings:
                                if after[offset:offset+length]!=current[offset:offset+length]:write(address,after[offset:offset+length])
                            check(event[16+SIZE:]);eventIndex+=1
                            for register in (*range(2,16),24,25):cpu.r[register]=(0xD00D0000+eventIndex*32+register)&0xFFFFFFFF
                            cpu.r[2]=result&0xFFFFFFFF;cpu.pc=cpu.r[31]
                        self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(cases,label,phase,profile))
                        self.assertEqual((eventIndex,replyIndex),(len(m.events),m.replyIndex));self.assertEqual(cpu.r[2],struct.unpack_from('<I',trace,4)[0]);check(trace[12:12+SIZE])
                        for register in (*range(16,24),28,30):self.assertEqual(cpu.r[register],0xBEEF0000+register)
                        self.assertEqual(read(STACK-0x410,16),b'\xA5'*16);self.assertEqual(read(STACK+0x10,16),b'\xA5'*16)
        self.assertEqual(cases,4608)
        for category,values in (('phase',(-1,0,1,2,3,4,5,6,7)),('info_event',range(5)),('load_event',range(5)),('sync',(-1,0,1)),('sync_result',range(4)),('begin',(0,1)),('poll_timeout',(False,True)),('begin_timeout',(False,True)),('sync_timeout',(False,True))):
            self.assertTrue({(category,value) for value in values}<=coverage,(category,coverage))


if __name__=='__main__':unittest.main()
