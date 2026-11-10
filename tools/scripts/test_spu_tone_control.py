"""Typed NRPN callbacks on current C, original MIPS and linked MIPS.

The old VAB SDK corpus is inherited unchanged. New cases exercise every payload
byte, raw argument words, real tone copy/selection and complete resource guards.
ADSR 11..14 retain an original uninitialized-local defect: those cases observe
only fields initialized by the handler at the builder service boundary. The
builder itself and the fully initialized 4..10 paths also execute real bodies.
"""
import copy
import ctypes as C
from pathlib import Path
import random
import re
import struct
import unittest

from c_functions import address_of, definition_source
from unicorn import UC_HOOK_CODE
from unicorn.mips_const import *
import test_vab_sdk_interface as sdk

ROOT = Path(__file__).resolve().parents[2]
NAMES = tuple(f'_SsSetNrpnVabAttr{i}' for i in range(20))
COPY_FIELDS = tuple(range(14)) + tuple(range(16,24))
BUILD_RESULT = (0x6123, 0xB456)

NATIVE = r'''
static unsigned active_handler,active_data;
typedef void (*ToneObserver)(u32,u32,u32,const u16 *,u32);
static ToneObserver tone_observer;
void control_observer(ToneObserver value){tone_observer=value;}
static void notify_control(u32 kind,u32 a,u32 b,const u16 *fields,u32 mask){
 if(tone_observer)tone_observer(kind,a,b,fields,mask);
}
s32 SsUtSetReverbType(s32 value){notify_control(1,value,0,0,0);return value;}
void SsUtSetReverbDepth(s32 left,s32 right){notify_control(2,left,right,0,0);}
void SsUtSetReverbFeedback(s16 value){notify_control(3,(u32)(s32)value,0,0,0);}
void SsUtSetReverbDelay(s16 value){notify_control(4,(u32)(s32)value,0,0,0);}
void SsUtReverbOff(void){notify_control(5,0,0,0,0);}
void SsUtReverbOn(void){notify_control(6,0,0,0,0);}
static struct {u8 before[16];SequenceChannel row;u8 after[16];} channel;
SequenceChannel *_ss_score[32];
static u8 *control_native_pointer(u32 address){
 if(!address)return 0;
 if(address>=0x80180010U&&address<0x80180010U+sizeof(buffers[0].words))
  return (u8 *)buffers[0].words+(address-0x80180010U);
 if(address>=0x801A0010U&&address<0x801A0010U+sizeof(buffers[1].words))
  return (u8 *)buffers[1].words+(address-0x801A0010U);
 if(address>=0x801C0010U&&address<0x801C0010U+sizeof(tone_storage.words))
  return (u8 *)tone_storage.words+(address-0x801C0010U);
 __builtin_trap();
}
static u32 control_wire_pointer(const u8 *pointer){
 uintptr_t value=(uintptr_t)pointer,first;
 if(!pointer)return 0;
 first=(uintptr_t)buffers[0].words;
 if(value>=first&&value-first<sizeof(buffers[0].words))return 0x80180010U+(u32)(value-first);
 first=(uintptr_t)buffers[1].words;
 if(value>=first&&value-first<sizeof(buffers[1].words))return 0x801A0010U+(u32)(value-first);
 first=(uintptr_t)tone_storage.words;
 if(value>=first&&value-first<sizeof(tone_storage.words))return 0x801C0010U+(u32)(value-first);
 __builtin_trap();
}
void control_row_set(const u8 *input){
 u8 **pointers[5]={&channel.row.c0,&channel.row.c4,&channel.row.c8,&channel.row.cC,&channel.row.p10};
 unsigned i;memset(&channel,0xA5,sizeof(channel));memset(_ss_score,0,sizeof(_ss_score));
 for(i=0;i<5;i++){u32 value;memcpy(&value,input+i*4,4);*pointers[i]=control_native_pointer(value);}
 memcpy((u8 *)&channel.row+RR_FIELD_OFFSET(SequenceChannel,b14),input+20,156);
 _ss_score[0]=&channel.row;
}
void control_row_get(u8 *output){
 u8 *pointers[5]={channel.row.c0,channel.row.c4,channel.row.c8,channel.row.cC,channel.row.p10};
 unsigned i;for(i=0;i<5;i++){u32 value=control_wire_pointer(pointers[i]);memcpy(output+i*4,&value,4);}
 memcpy(output+20,(u8 *)&channel.row+RR_FIELD_OFFSET(SequenceChannel,b14),156);
 for(i=0;i<16;i++)if(channel.before[i]!=0xA5||channel.after[i]!=0xA5)__builtin_trap();
 for(i=RR_FIELD_OFFSET(SequenceChannel,wac)+4;i<sizeof(SequenceChannel);i++)
  if(((u8 *)&channel.row)[i]!=0xA5)__builtin_trap();
 for(i=1;i<32;i++)if(_ss_score[i])__builtin_trap();
}
void control_select(unsigned handler,u32 data){active_handler=handler;active_data=data&255;}
'''


class SpuToneControlTest(sdk.VabSdkInterfaceTest):
    @classmethod
    def native_compile_flags(cls):
        # The unused words belong to the recovered callback ABI. Two original
        # handlers pass an uninitialized ADSR object; their warning remains
        # visible and their tests use the explicit partial-builder boundary.
        return ['-Wno-unused-parameter', '-Wno-error=uninitialized-const-pointer', '-Wno-error=uninitialized']

    @classmethod
    def native_extension(cls, owners):
        headers = {'psyq/libsnd/cc_6.h', 'psyq/libsnd/cc_nrpn3.h',
                   'psyq/libsnd/utrev2.h', 'psyq/libsnd/utrtype.h', 'psyq/libsnd/utrev3.h'}
        for name in NAMES:
            headers.add(str(owners[name].relative_to(ROOT/'src/main/PAL').with_suffix('.h')))
        code = '#include "rr/library.h"\n'+''.join(f'#include "{h}"\n' for h in sorted(headers))+NATIVE
        code += definition_source(owners['_SsUtResolveADSR'].read_text(), '_SsUtResolveADSR')
        body = definition_source(owners['_SsUtBuildADSR'].read_text(), '_SsUtBuildADSR')
        # These two pre-existing target register bindings remain explicit debt.
        assert len(re.findall(r'__asm__\("\$\d+"\)', body)) == 2
        body = re.sub(r' __asm__\("\$\d+"\)', '', body)
        code += body.replace('void _SsUtBuildADSR(', 'void native_build_adsr(')
        code += r'''
void _SsUtBuildADSR(const SpuAdsrParameters *p,u16 *first,u16 *second){
 u16 fields[9]={0};u32 mask=0;
 if(active_handler==11){fields[4]=p->releaseRate;fields[7]=p->releaseExponential;mask=(1U<<4)|(1U<<7);}
 else if(active_handler==12){if(active_data>=1&&active_data<=127){fields[8]=p->sustainDecrease;mask=1U<<8;}}
 else if(active_handler!=13&&active_handler!=14){memcpy(fields,p,sizeof(fields));mask=511;}
 notify_control(0,0,0,fields,mask);
 if(active_handler>=11&&active_handler<=14){*first=0x6123;*second=0xB456;}
 else native_build_adsr(p,first,second);
}
'''
        for name in NAMES:
            code += definition_source(owners[name].read_text(), name)
        code += 'SvmToneControl SsFCALLCcEntry[20]={'+','.join(NAMES)+'};\n'
        code += r'''
void control_invoke(unsigned index,s32 bank,s32 program,s32 tone,const VagAtr *input,s32 attribute,s32 data){
 control_select(index,(u32)data);
 SsFCALLCcEntry[index]((s16)bank,(s16)program,(s16)tone,*input,attribute,(u8)data);
}
'''
        code += definition_source(owners['_SsReadDeltaValue'].read_text(), '_SsReadDeltaValue')
        code += definition_source(owners['_SsContDataEntry'].read_text(), '_SsContDataEntry')
        return code

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        if cls.native_compile_warnings:
            print(cls.native_compile_warnings)
        cls.lib.control_invoke.argtypes = [C.c_uint, C.c_int32, C.c_int32, C.c_int32,
                                           C.c_void_p, C.c_int32, C.c_int32]
        cls.lib.control_invoke.restype = None
        cls.lib.control_select.argtypes = [C.c_uint, C.c_uint32]
        cls.lib.control_row_set.argtypes = [C.c_void_p]
        cls.lib.control_row_get.argtypes = [C.c_void_p]
        cls.lib._SsContDataEntry.argtypes = [C.c_int16, C.c_int16, C.c_uint8]
        cls.lib._SsContDataEntry.restype = None
        cls.lib._SsUtResolveADSR.argtypes = [C.c_int32, C.c_uint32, C.c_void_p]
        cls.lib._SsUtResolveADSR.restype = None
        cls.lib.native_build_adsr.argtypes = [C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.native_build_adsr.restype = None

    def setup_controls(self):
        self.with_tones = True
        self.addCleanup(setattr, self, 'with_tones', False)
        self.tone_events = []
        self.control_errors = []
        callback_type = C.CFUNCTYPE(None, C.c_uint32, C.c_uint32, C.c_uint32, C.c_void_p, C.c_uint32)
        def observer(kind,a,b,fields,mask):
            try:
                values = (C.c_uint16*9).from_address(fields) if fields else ()
                self.tone_events.append((kind,a,b,tuple((i,int(values[i])) for i in range(9) if mask&(1<<i))))
            except BaseException as error:
                self.control_errors.append(error)
        self.control_observer = callback_type(observer)
        self.lib.control_observer(self.control_observer)
        self.addCleanup(self.lib.control_observer, callback_type())
        table = (C.c_void_p*20).in_dll(self.lib,'SsFCALLCcEntry')
        self.native_table = tuple(table)
        self.assertEqual(self.native_table,tuple(C.cast(getattr(self.lib,n),C.c_void_p).value for n in NAMES))
        self.assertTrue(all(p>0xFFFFFFFF for p in self.native_table))
        self.pool = b'\xA5'*16+bytes((i*73+(i>>7)+19)&255 for i in range(sdk.TONE_SIZE))+b'\xA5'*16

    def control_initial(self, bank, program, group, flag):
        initial = self.initial(4)
        initial[1].append(bytearray(self.pool))
        v = initial[0]
        center = sdk.TONE_POOL+sdk.TONE_SIZE//2
        v[0] = [1]*16
        v[0][bank] = flag
        v[4] = [sdk.BUFFERS[0]]*16
        v[5] = [sdk.BUFFERS[0]+64]*16
        v[6] = [center]*16
        v[12][0],v[13][0],v[14][0] = sdk.BUFFERS[1]+64,sdk.BUFFERS[1],center
        v[15][7] = group
        if program<64:
            data,at=self.buffer_cell(initial,v[5][bank]+program*16+8)
            data[at]=group
        return initial

    @staticmethod
    def decode_adsr(first,second):
        return [(first>>8)&127,(first>>4)&15,first&15,(second>>6)&127,second&31,
                first&32768,second&32768,second&32,second&16384]

    @staticmethod
    def encode_adsr(p):
        return ((32768 if p[5] else 0)|((p[0]&127)<<8)|((p[1]&15)<<4)|(p[2]&15),
                (32768 if p[6] else 0)|(16384 if p[8] else 0)|(32 if p[7] else 0)|((p[3]&127)<<6)|(p[4]&31))

    def oracle_handler(self, model, handler, raw_bank, raw_program, raw_tone, parameters, raw_data):
        bank,program,tone = (C.c_int16(x).value for x in (raw_bank,raw_program,raw_tone))
        value = raw_data&255
        if handler>=15:
            kind={15:1,16:2,17:3,18:4,19:4}[handler]
            return [(kind,value,value if handler==16 else 0,())]
        record=bytearray(parameters)
        if model[0][0][bank]==1:
            self.oracle_select(model,bank,program)
            group=C.c_int8(model[0][15][7]).value
            index=C.c_int16(tone+group*16).value
            data,at=self.buffer_cell(model,model[0][14][0]+index*32)
            for field in COPY_FIELDS:record[field]=data[at+field]
        events=[]
        if handler in (0,1,2,3):
            record[(0,1,6,7)[handler]]=value
        else:
            if handler<=10:
                p=self.decode_adsr(*struct.unpack_from('<HH',record,16))
                rate={4:0,5:0,6:1,7:2,8:3,9:3,10:4}[handler]
                p[rate]=value
                if handler in (4,5):p[5]=handler-4
                if handler in (8,9):p[6]=handler-8
                if handler==10:p[7]=0
                fields=tuple(enumerate(p))
                words=self.encode_adsr(p)
            else:
                if handler==11:fields=((4,value),(7,1))
                elif handler==12:fields=((8,int(value>=64)),) if 1<=value<=127 else ()
                else:fields=()
                if handler in (13,14):record[handler-4]=value
                words=BUILD_RESULT
            events.append((0,0,0,fields))
            struct.pack_into('<HH',record,16,*words)
        if model[0][0][bank]==1:
            self.oracle_select(model,bank,program)
            group=C.c_int8(model[0][15][7]).value
            index=C.c_int16(tone+group*16).value
            data,at=self.buffer_cell(model,model[0][14][0]+index*32)
            for field in COPY_FIELDS:data[at+field]=record[field]
        if handler==1 and value in (0,4):events.append((5 if value==0 else 6,0,0,()))
        return events

    def install_control_hooks(self):
        entries=[address_of(n) for n in NAMES]
        self.write(address_of('SsFCALLCcEntry'),struct.pack('<20I',*entries))
        target={address_of(n):kind for n,kind in [('SsUtSetReverbType',1),('SsUtSetReverbDepth',2),
                ('SsUtSetReverbFeedback',3),('SsUtSetReverbDelay',4),('SsUtReverbOff',5),('SsUtReverbOn',6)]}
        builder_entry=address_of('_SsUtBuildADSR')
        def hook(cpu,pc,size,opaque):
            partial=False
            if pc==builder_entry:
                pointer=cpu.reg_read(UC_MIPS_REG_A0)
                if self.handler<=10 or self.handler>14:indices=range(9)
                elif self.handler==11:indices=(4,7)
                elif self.handler==12:indices=(8,) if 1<=(self.payload&255)<=127 else ()
                else:indices=()
                fields=tuple((i,struct.unpack('<H',self.read(pointer+2*i,2))[0]) for i in indices)
                self.tone_events.append((0,0,0,fields))
                partial=11<=self.handler<=14
                if not partial:return
                self.write(cpu.reg_read(UC_MIPS_REG_A1),struct.pack('<H',BUILD_RESULT[0]))
                self.write(cpu.reg_read(UC_MIPS_REG_A2),struct.pack('<H',BUILD_RESULT[1]))
            elif pc in target:
                kind=target[pc]
                a=cpu.reg_read(UC_MIPS_REG_A0) if kind<5 else 0
                b=cpu.reg_read(UC_MIPS_REG_A1) if kind==2 else 0
                self.tone_events.append((kind,a,b,()))
            else:return
            ra=cpu.reg_read(UC_MIPS_REG_RA)
            for reg in (UC_MIPS_REG_AT,UC_MIPS_REG_V0,UC_MIPS_REG_V1,
                        *range(UC_MIPS_REG_A0,UC_MIPS_REG_T7+1),UC_MIPS_REG_T8,UC_MIPS_REG_T9):
                cpu.reg_write(reg,sdk.word(0xDEAD0000+reg))
            cpu.reg_write(UC_MIPS_REG_V0,0xA55AA55A)
            cpu.reg_write(UC_MIPS_REG_HI,0x12345678);cpu.reg_write(UC_MIPS_REG_LO,0x87654321)
            cpu.reg_write(UC_MIPS_REG_PC,ra)
        self.cpu.hook_add(UC_HOOK_CODE,hook)

    def invoke_handler(self, handler, bank, program, tone, parameters, attribute, data):
        self.write(sdk.STACK-0x2000,b'\x5A'*0x2040)
        for reg in range(UC_MIPS_REG_1,UC_MIPS_REG_31+1):self.cpu.reg_write(reg,sdk.word(0xA5A50000+reg))
        for reg,value in zip((UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2),(bank,program,tone)):
            self.cpu.reg_write(reg,sdk.word(value))
        self.cpu.reg_write(UC_MIPS_REG_A3,struct.unpack_from('<I',parameters)[0])
        self.write(sdk.STACK+16,parameters[4:]+struct.pack('<II',sdk.word(attribute),sdk.word(data)))
        self.cpu.reg_write(UC_MIPS_REG_SP,sdk.STACK);self.cpu.reg_write(UC_MIPS_REG_RA,sdk.STOP)
        registers=(*range(UC_MIPS_REG_S0,UC_MIPS_REG_S7+1),UC_MIPS_REG_GP,UC_MIPS_REG_FP,UC_MIPS_REG_SP,UC_MIPS_REG_RA)
        preserved={r:self.cpu.reg_read(r) for r in registers}
        entry=struct.unpack('<I',self.read(address_of('SsFCALLCcEntry')+4*handler,4))[0]
        self.cpu.emu_start(entry,sdk.STOP,count=50000)
        self.assertEqual(self.cpu.reg_read(UC_MIPS_REG_PC),sdk.STOP)
        self.assertEqual({r:self.cpu.reg_read(r) for r in registers},preserved)
        self.assertEqual(self.read(sdk.STACK+52,12),b'\x5A'*12)
        self.assertEqual(self.read(sdk.STACK-0x2000,32),b'\x5A'*32)

    def test_all_handlers_payload_bytes_and_raw_stack_words(self):
        self.setup_controls()
        selected_programs=(-1,0,1,63,64,32767)
        for linked,self.cpu in self.machines():
            self.install_control_hooks()
            for self.handler in range(20):
                for self.payload in range(256):
                    self.case=self.handler*256+self.payload
                    bank=(0,7,15)[self.payload%3]
                    program=selected_programs[self.payload%6]
                    tone=(-32768,-1,0,32767)[self.payload%4]
                    flag=(1,1,1,0,2,255)[(self.payload//6)%6]
                    initial=self.control_initial(bank,program,self.payload,flag)
                    parameters=bytes((self.handler*17+i*29+self.payload*7)&255 for i in range(32))
                    raw_bank=sdk.signed(0xABCD0000|bank)
                    raw_program=sdk.signed(0x12340000|(program&65535))
                    raw_tone=sdk.signed(0xA5A50000|(tone&65535))
                    attribute=sdk.signed(0xDEAD0000|((self.handler+11)&255))
                    data=sdk.signed(((0,0x80000000,0xFFFFFF00,0x12345600)[self.payload%4])|self.payload)
                    model=copy.deepcopy(initial)
                    events=self.oracle_handler(model,self.handler,raw_bank,raw_program,raw_tone,parameters,data)
                    expected=self.freeze(model)
                    self.reset(initial,False);self.tone_events=[]
                    self.invoke_handler(self.handler,raw_bank,raw_program,raw_tone,parameters,attribute,data)
                    self.assertEqual(self.tone_events,events,(linked,self.case,'MIPS events'))
                    self.assertEqual(self.snapshot(False),expected,(linked,self.case,'MIPS state'))
                    self.reset(initial,True);self.tone_events=[]
                    storage=(C.c_uint16*32)();C.memset(C.addressof(storage),0xA5,C.sizeof(storage))
                    C.memmove(C.addressof(storage)+16,parameters,32);before=bytes(storage)
                    self.lib.control_invoke(self.handler,raw_bank,raw_program,raw_tone,C.addressof(storage)+16,attribute,data)
                    if self.control_errors:raise self.control_errors[0]
                    self.assertEqual(self.tone_events,events,(linked,self.case,'native events'))
                    self.assertEqual(self.snapshot(True),expected,(linked,self.case,'native state'))
                    self.assertEqual(bytes(storage),before)
            self.assertEqual(self.read(address_of('SsFCALLCcEntry'),80),struct.pack('<20I',*(address_of(n) for n in NAMES)))
        self.assertEqual(tuple((C.c_void_p*20).in_dll(self.lib,'SsFCALLCcEntry')),self.native_table)


    def test_adsr_codec_real_helpers_and_aliases(self):
        self.setup_controls()
        decoded=(C.c_uint16*25)()
        pair=(C.c_uint16*2)()
        for first in range(65536):
            second=(first*40503+17)&65535
            C.memset(C.addressof(decoded),0xA5,C.sizeof(decoded))
            self.lib._SsUtResolveADSR(sdk.signed(0x80000000|first),0xA55A0000|second,C.addressof(decoded)+16)
            expected=struct.pack('<9H',*self.decode_adsr(first,second))
            self.assertEqual(bytes(decoded),b'\xA5'*16+expected+b'\xA5'*16)
            self.lib.native_build_adsr(C.addressof(decoded)+16,C.addressof(pair),C.addressof(pair)+2)
            self.assertEqual(tuple(pair),(first,second&~0x2000))
        generator=random.Random(0x4E52504E)
        cases=[tuple(generator.randrange(65536) for _ in range(9)) for _ in range(1024)]
        region=sdk.BUFFERS[1]+0x11000
        aliases=((0,2),(16,16),(18,20),(20,0),(8,10),(-8,-6))
        initial=self.control_initial(0,0,0,1)
        for linked,self.cpu in self.machines():
            for native in (False,True):
                self.reset(initial,native)
                model=copy.deepcopy(initial)
                for index,fields in enumerate(cases):
                    self.case=index
                    shadow=bytearray(b'\xA5'*16+struct.pack('<9H',*fields)+b'\xA5'*30)
                    first,second=self.encode_adsr(fields)
                    a,b=aliases[index%len(aliases)]
                    wanted=bytearray(shadow)
                    struct.pack_into('<H',wanted,16+a,first)
                    struct.pack_into('<H',wanted,16+b,second)
                    if native:
                        base=self.native_pointer(region)
                        C.memmove(base,bytes(shadow),len(shadow))
                        self.lib.native_build_adsr(base+16,base+16+a,base+16+b)
                        actual=C.string_at(base,len(shadow))
                    else:
                        self.write(region,shadow)
                        self.invoke('_SsUtBuildADSR',(region+16,region+16+a,region+16+b))
                        actual=self.read(region,len(shadow))
                    self.assertEqual(actual,bytes(wanted),(linked,native,index,'build aliases'))
                    data,at=self.buffer_cell(model,region)
                    data[at:at+len(wanted)]=wanted
                self.assertEqual(self.snapshot(native),self.freeze(model),(linked,native,'complete resources'))
            # A distinct MIPS sample checks raw-word decode, including the
            # ignored upper halfwords, against the independent field oracle.
            for index,fields in enumerate(cases):
                first=(index*40503)&65535;second=(index*173+0x8000)&65535
                shadow=b'\xA5'*50
                self.write(region,shadow);self.case=index
                self.invoke('_SsUtResolveADSR',(0xBEEF0000|first,0xA55A0000|second,region+16))
                expected=b'\xA5'*16+struct.pack('<9H',*self.decode_adsr(first,second))+b'\xA5'*16
                self.assertEqual(self.read(region,50),expected,(linked,index,'decode'))

    def test_real_nrpn_dispatch_and_delta_on_complete_sequence_owner(self):
        self.setup_controls()
        row_address=0x803D0010
        generator=random.Random(0x43433600)
        for linked,self.cpu in self.machines():
            self.install_control_hooks()
            for self.handler in range(20):
                for self.payload in (0,1,4,63,64,127,128,255):
                    self.case=self.handler*256+self.payload
                    bank=(0,7,15)[self.case%3]
                    program=(0,1,63,64)[self.case%4]
                    group=(self.case*73+128)&255
                    initial=self.control_initial(bank,program,group,1)
                    row=bytearray(generator.randbytes(176))
                    pointers=[sdk.BUFFERS[1]+0x100+i*16 for i in range(5)]
                    struct.pack_into('<5I',row,0,*pointers)
                    channel=self.handler%16
                    row[0x17]=channel;row[0x26]=bank;row[0x37+channel]=program
                    row[0x1C]=0;row[0x1E]=0;row[0x1F]=2;row[0x1A]=self.handler
                    row[0x1B]=16 if self.case&1 else self.handler%16
                    struct.pack_into('<I',row,0x88,0xFFFFFFFE)
                    stream=(b'\0',b'\x7F',b'\x81\0',b'\xFF\x81\1')[self.case%4]
                    data,at=self.buffer_cell(initial,pointers[0]);data[at:at+len(stream)]=stream
                    model=copy.deepcopy(initial)
                    expected_row=bytearray(row)
                    # GetProgAtr runs before the NRPN branch and publishes the
                    # selected cursors even though its local copy is unused.
                    self.oracle_select(model,bank,program)
                    tone=0 if row[0x1B]==16 else row[0x1B]
                    events=self.oracle_handler(model,self.handler,bank,program,tone,bytes(32),self.payload)
                    value=0
                    for byte in stream:
                        value=(value<<7)+(byte&127)
                        if not byte&128:break
                    delta=sdk.word(value*10)
                    struct.pack_into('<I',expected_row,0,pointers[0]+len(stream))
                    struct.pack_into('<I',expected_row,0x88,sdk.word(0xFFFFFFFE+delta))
                    struct.pack_into('<I',expected_row,0x90,delta)
                    expected_row[0x1F]=0
                    expected=self.freeze(model)
                    for native in (False,True):
                        self.reset(initial,native);self.tone_events=[]
                        if native:
                            input_row=(C.c_uint8*176).from_buffer_copy(row)
                            self.lib.control_row_set(input_row)
                            self.lib.control_select(self.handler,self.payload)
                            self.lib._SsContDataEntry(0,0,self.payload)
                            result=(C.c_uint8*176)();self.lib.control_row_get(result)
                            actual_row=bytes(result)
                        else:
                            self.write(row_address-16,b'\xA5'*16+bytes(row)+b'\xA5'*16)
                            self.write(address_of('_ss_score'),struct.pack('<32I',row_address,*([0]*31)))
                            self.invoke('_SsContDataEntry',(0,0,self.payload))
                            actual_row=self.read(row_address,176)
                            self.assertEqual(self.read(row_address-16,16),b'\xA5'*16)
                            self.assertEqual(self.read(row_address+176,16),b'\xA5'*16)
                            self.assertEqual(self.read(address_of('_ss_score'),128),struct.pack('<32I',row_address,*([0]*31)))
                        if self.control_errors:raise self.control_errors[0]
                        self.assertEqual(self.tone_events,events,(linked,self.case,native,'dispatch events'))
                        self.assertEqual(actual_row,bytes(expected_row),(linked,self.case,native,'complete row'))
                        self.assertEqual(self.snapshot(native),expected,(linked,self.case,native,'complete resources'))
        self.assertEqual(tuple((C.c_void_p*20).in_dll(self.lib,'SsFCALLCcEntry')),self.native_table)


if __name__=="__main__":
    unittest.main()
