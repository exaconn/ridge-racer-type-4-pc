"""Actual CD retry controller with native callback and buffer pointers."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners

Hook=C.CFUNCTYPE(C.c_int32,C.c_int32,C.c_void_p,C.c_void_p,C.c_int32)
Sync=C.CFUNCTYPE(C.c_int32,C.c_int32,C.c_void_p)


class CdCommandControlTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        headers=''
        for file in ('library.h','cd.h'):
            header=(root/'include/rr'/file).read_text()
            headers+=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)+'\n'
        owners=source_owners(root/'src/main/PAL')
        paths=dict.fromkeys(owners[name] for name in ('func_8008C140','func_8008C0D8'))
        body='\n'.join(path.read_text() for path in paths)
        assert body.count('(RrLibDataCallback *)0x800A0000U')==2
        body=body.replace('(RrLibDataCallback *)0x800A0000U','callback_page')
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
        body=re.sub(r'\s*:\s*"\$[0-9]+"(?:\s*,\s*"\$[0-9]+")*(?=\))','',body)
        code='''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8;typedef int8_t s8;typedef uint16_t u16;typedef int16_t s16;
typedef uint32_t u32;typedef int32_t s32;
#define RR_FIELD_OFFSET(type,field) offsetof(type,field)
'''+headers+'''
u32 D_800A6400[34];RrCdDriveStatus D_800A6494;
RrLibDataCallback callback_page[0x10000/sizeof(RrLibDataCallback)];
#define D_800A6488 callback_page[0x6488/sizeof(RrLibDataCallback)]
s32 (*on_call)(s32,const u8 *,u8 *,s32);
s32 (*on_sync)(s32,u8 *);
s32 func_8008CC80(s32 mode,u8 *status){return on_sync(mode,status);}
s32 called[3];
void callback_a(s32 code,u8 *data){called[0]++;}
void callback_b(s32 code,u8 *data){called[1]++;}
void callback_c(s32 code,u8 *data){called[2]++;}
s32 func_8008D1C8(s32 command,const u8 *parameters,u8 *result,s32 mode){
    return on_call(command,parameters,result,mode);
}
'''+body
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        cls.f=cls.lib.func_8008C140;cls.f.argtypes=[C.c_int32,C.c_void_p,C.c_void_p];cls.f.restype=C.c_int32
        cls.g=cls.lib.func_8008C27C;cls.g.argtypes=[C.c_int32,C.c_void_p];cls.g.restype=C.c_int32
        cls.h=cls.lib.func_8008C3B0;cls.h.argtypes=[C.c_int32,C.c_void_p,C.c_void_p];cls.h.restype=C.c_int32
        cls.wrapper=cls.lib.func_8008C0D8;cls.wrapper.argtypes=[C.c_int32,C.c_void_p];cls.wrapper.restype=C.c_int32
        cls.table=(C.c_uint32*34).in_dll(cls.lib,'D_800A6400');cls.status=C.c_uint8.in_dll(cls.lib,'D_800A6494')
        cls.slot=C.c_void_p.from_address(C.addressof((C.c_void_p*(0x10000//C.sizeof(C.c_void_p))).in_dll(cls.lib,'callback_page'))+0x6488);cls.hook=C.c_void_p.in_dll(cls.lib,'on_call')
        cls.callbacks=[C.cast(getattr(cls.lib,'callback_'+n),C.c_void_p).value for n in ('a','b','c')]
        cls.called=(C.c_int32*3).in_dll(cls.lib,'called')

    def setUp(self):
        self.table[:]=[0]*34;self.status.value=0;self.slot.value=self.callbacks[0]
        self.parameters=(C.c_uint8*8)(*[0xA0+i for i in range(8)])
        self.result=(C.c_uint8*8)(*[0xB0+i for i in range(8)])
        self.p=C.addressof(self.parameters);self.q=C.addressof(self.result)
        self.events=[];self.retained=[];self.hook_errors=[];self.called[:]=[0,0,0]
        self.sync_events=[]
        self.install_sync(lambda mode,q:2)

    def install(self,fn):
        def observe(command,p,q,mode):
            self.events.append((command,p,q,mode,self.slot.value))
            try:return fn(command,p,q,mode)
            except BaseException as error:
                self.hook_errors.append(error)
                return -1
        hook=Hook(observe);self.retained.append(hook);self.hook.value=C.cast(hook,C.c_void_p).value

    def invoke(self,*args):
        result=self.f(*args)
        if self.hook_errors:raise self.hook_errors.pop(0)
        return result

    def invoke_mode1(self,*args):
        result=self.g(*args)
        if self.hook_errors:raise self.hook_errors.pop(0)
        return result

    def install_sync(self,fn):
        def observe(mode,q):
            self.sync_events.append((mode,q,self.slot.value))
            try:return fn(mode,q)
            except BaseException as error:
                self.hook_errors.append(error)
                return -1
        hook=Sync(observe);self.retained.append(hook)
        C.c_void_p.in_dll(self.lib,'on_sync').value=C.cast(hook,C.c_void_p).value

    def invoke_wait(self,*args):
        result=self.h(*args)
        if self.hook_errors:raise self.hook_errors.pop(0)
        return result

    def test_supported_commands_high_words_and_optional_preparation(self):
        self.install(lambda *args:0)
        for command in range(34):
            for high in (0,0x12340000,0x80000000,0xFFFF0000):
                for metadata in (0,1,0x80000000):
                    for status in (0,0x10,0xFF):
                        for p in (None,self.p):
                            self.events=[];self.table[command]=metadata;self.status.value=status
                            self.slot.value=self.callbacks[0]
                            before=(bytes(self.table),bytes(self.parameters),bytes(self.result))
                            self.assertEqual(self.invoke(C.c_int32(high|command).value,p,self.q),1)
                            expected=[]
                            if command!=1 and status&0x10:expected.append((1,None,None,0,None))
                            if p and metadata:expected.append((2,p,self.q,0,None))
                            expected.append((command,p,self.q,0,self.callbacks[0]))
                            self.assertEqual(self.events,expected)
                            self.assertEqual((bytes(self.table),bytes(self.parameters),bytes(self.result)),before)
                            self.assertEqual(self.slot.value,self.callbacks[0])
        self.assertEqual(list(self.called),[0,0,0])

    def test_four_attempts_success_failure_and_status_result_ignored(self):
        for failure_stage in ('prepare','main'):
            for success_attempt in (1,2,3,4,None):
                for error in (-2147483648,-1,1,2147483647):
                    self.events=[];self.table[7]=1;self.status.value=0x10;self.slot.value=self.callbacks[0]
                    attempts=4 if success_attempt is None else success_attempt
                    plan=[]
                    for attempt in range(1,attempts+1):
                        plan.append((1,error,None))
                        succeeds=attempt==success_attempt
                        plan.append((2,0 if failure_stage=='main' or succeeds else error,None))
                        if failure_stage=='main' or succeeds:plan.append((7,0 if succeeds else error,self.callbacks[0]))
                    pending=iter(plan)
                    def dispatch(command,p,q,mode):
                        wanted,value,callback=next(pending)
                        self.assertEqual((command,self.slot.value),(wanted,callback))
                        return value
                    self.install(dispatch)
                    self.assertEqual(self.invoke(7,self.p,self.q),int(success_attempt is not None))
                    self.assertEqual([e[0] for e in self.events],[p[0] for p in plan])
                    self.assertEqual(self.slot.value,self.callbacks[0])
                    with self.assertRaises(StopIteration):next(pending)

    def test_fresh_metadata_status_and_callback_mutations(self):
        self.status.value=0x10;self.table[7]=0
        sequence=iter([1,2,2,7,7])
        main_calls=0;preparations=0
        def mutate(command,p,q,mode):
            nonlocal main_calls,preparations
            self.assertEqual(command,next(sequence))
            if command==1:
                self.table[7]=1;self.status.value=0;self.slot.value=self.callbacks[1]
                return -9
            if command==2:
                preparations+=1
                if preparations==1:self.slot.value=self.callbacks[2];return -1
                self.table[7]=0;return 0
            main_calls+=1
            self.slot.value=self.callbacks[1] if main_calls==1 else self.callbacks[2]
            C.cast(q,C.POINTER(C.c_uint8))[0]=0x70+main_calls
            return -1 if main_calls==1 else 0
        self.install(mutate)
        before=bytes(self.result)
        self.assertEqual(self.invoke(7,self.p,self.q),1)
        self.assertEqual([e[4] for e in self.events],[None,self.callbacks[1],None,self.callbacks[0],self.callbacks[0]])
        self.assertEqual(self.slot.value,self.callbacks[2]);self.assertEqual(self.result[0],0x72)
        self.assertEqual(bytes(self.result)[1:],before[1:])
        self.assertEqual((self.status.value,self.table[7]),(0,0))

    def test_reentry_retains_independent_saved_callback_and_success_mutation(self):
        nested=[];entered=False
        def reenter(command,p,q,mode):
            nonlocal entered
            if command==7 and not entered:
                entered=True;nested.append(self.invoke(1,None,None))
                self.assertEqual(self.slot.value,self.callbacks[1])
            elif command==1:
                self.slot.value=self.callbacks[1]
            return 0
        self.install(reenter)
        self.assertEqual(self.invoke(7,None,None),1)
        self.assertEqual(nested,[1]);self.assertEqual([e[0] for e in self.events],[7,1])
        self.assertEqual([e[4] for e in self.events],[self.callbacks[0],self.callbacks[0]])
        self.assertEqual(self.slot.value,self.callbacks[1])
        DataCallback=C.CFUNCTYPE(None,C.c_int32,C.c_void_p)
        DataCallback(self.slot.value)(123,self.q)
        self.assertEqual(list(self.called),[0,1,0])

    def test_mode1_commands_high_words_optional_preparation_and_null_results(self):
        self.install(lambda *args:0)
        for command in range(34):
            for high in (0,0x12340000,0x80000000,0xFFFF0000):
                for metadata in (0,1,0x80000000):
                    for status in (0,0x10,0xFF):
                        for p in (None,self.p):
                            self.events=[];self.table[command]=metadata;self.status.value=status
                            self.slot.value=self.callbacks[0]
                            before=(bytes(self.table),bytes(self.parameters),bytes(self.result))
                            self.assertEqual(self.invoke_mode1(C.c_int32(high|command).value,p),1)
                            expected=[]
                            if command!=1 and status&0x10:expected.append((1,None,None,0,None))
                            if p and metadata:expected.append((2,p,None,0,None))
                            expected.append((command,p,None,1,self.callbacks[0]))
                            self.assertEqual(self.events,expected)
                            self.assertEqual((bytes(self.table),bytes(self.parameters),bytes(self.result)),before)
                            self.assertEqual(self.slot.value,self.callbacks[0])
        self.assertEqual(list(self.called),[0,0,0])

    def test_mode1_four_attempts_and_each_failure_stage(self):
        for failure_stage in ('prepare','main'):
            for success_attempt in (1,2,3,4,None):
                for error in (-2147483648,-1,1,2147483647):
                    self.events=[];self.table[7]=1;self.status.value=0x10;self.slot.value=self.callbacks[0]
                    attempts=4 if success_attempt is None else success_attempt
                    plan=[]
                    for attempt in range(1,attempts+1):
                        plan.append((1,error,None,0))
                        succeeds=attempt==success_attempt
                        plan.append((2,0 if failure_stage=='main' or succeeds else error,None,0))
                        if failure_stage=='main' or succeeds:plan.append((7,0 if succeeds else error,self.callbacks[0],1))
                    pending=iter(plan)
                    def dispatch(command,p,q,mode):
                        wanted,value,callback,wanted_mode=next(pending)
                        self.assertEqual((command,self.slot.value,q,mode),(wanted,callback,None,wanted_mode))
                        return value
                    self.install(dispatch)
                    self.assertEqual(self.invoke_mode1(7,self.p),int(success_attempt is not None))
                    self.assertEqual([e[0] for e in self.events],[item[0] for item in plan])
                    self.assertEqual(self.slot.value,self.callbacks[0])
                    with self.assertRaises(StopIteration):next(pending)

    def test_mode1_fresh_metadata_status_and_success_callback_mutation(self):
        self.status.value=0x10;self.table[7]=0
        expected=iter([1,2,2,7,7]);main_calls=0;preparations=0
        def mutate(command,p,q,mode):
            nonlocal main_calls,preparations
            self.assertEqual(command,next(expected));self.assertIsNone(q)
            if command==1:
                self.table[7]=1;self.status.value=0;self.slot.value=self.callbacks[1]
                return -9
            if command==2:
                preparations+=1
                if preparations==1:self.slot.value=self.callbacks[2];return -1
                self.table[7]=0;return 0
            self.assertEqual(mode,1);self.assertEqual(p,self.p)
            main_calls+=1
            self.slot.value=self.callbacks[1] if main_calls==1 else self.callbacks[2]
            self.parameters[0]=0x60+main_calls
            return -1 if main_calls==1 else 0
        self.install(mutate)
        self.assertEqual(self.invoke_mode1(7,self.p),1)
        self.assertEqual([e[4] for e in self.events],[None,self.callbacks[1],None,self.callbacks[0],self.callbacks[0]])
        self.assertEqual(self.slot.value,self.callbacks[2])

        self.assertEqual(self.parameters[0],0x62)
        self.assertEqual((self.status.value,self.table[7]),(0,0))

    def test_wait_variant_commands_arguments_and_exact_completion_call(self):
        self.install(lambda *args:0)
        for command in range(34):
            for high in (0,0x12340000,0x80000000,0xFFFF0000):
                for metadata in (0,1,0x80000000):
                    for status in (0,0x10,0xFF):
                        for p in (None,self.p):
                            for q in (None,self.q):
                                self.events=[];self.sync_events=[]
                                self.table[command]=metadata;self.status.value=status;self.slot.value=self.callbacks[0]
                                before=(bytes(self.table),bytes(self.parameters),bytes(self.result))
                                self.assertEqual(self.invoke_wait(C.c_int32(high|command).value,p,q),1)
                                expected=[]
                                if command!=1 and status&0x10:expected.append((1,None,None,0,None))
                                if p and metadata:expected.append((2,p,q,0,None))
                                expected.append((command,p,q,0,self.callbacks[0]))
                                self.assertEqual(self.events,expected)
                                self.assertEqual(self.sync_events,[(0,q,self.callbacks[0])])
                                self.assertEqual((bytes(self.table),bytes(self.parameters),bytes(self.result)),before)

    def test_wait_variant_all_attempts_and_no_completion_after_exhaustion(self):
        for failure_stage in ('prepare','main'):
            for success_attempt in (1,2,3,4,None):
                for error in (-2147483648,-1,1,2147483647):
                    self.events=[];self.sync_events=[]
                    self.table[7]=1;self.status.value=0x10;self.slot.value=self.callbacks[0]
                    attempts=4 if success_attempt is None else success_attempt
                    plan=[]
                    for attempt in range(1,attempts+1):
                        plan.append((1,error,None,None,None))
                        succeeds=attempt==success_attempt
                        plan.append((2,0 if failure_stage=='main' or succeeds else error,None,self.p,self.q))
                        if failure_stage=='main' or succeeds:
                            plan.append((7,0 if succeeds else error,self.callbacks[0],self.p,self.q))
                    pending=iter(plan)
                    def dispatch(command,p,q,mode):
                        wanted,value,callback,wanted_p,wanted_q=next(pending)
                        self.assertEqual((command,p,q,mode,self.slot.value),(wanted,wanted_p,wanted_q,0,callback))
                        return value
                    self.install(dispatch)
                    self.assertEqual(self.invoke_wait(7,self.p,self.q),int(success_attempt is not None))
                    self.assertEqual(self.sync_events,[] if success_attempt is None else [(0,self.q,self.callbacks[0])])
                    self.assertEqual(self.slot.value,self.callbacks[0])
                    with self.assertRaises(StopIteration):next(pending)

    def test_wait_completion_result_two_only_and_callback_mutations_survive(self):
        for value in (-2147483648,-1,0,1,2,3,5,2147483647):
            self.events=[];self.sync_events=[];self.slot.value=self.callbacks[0]
            def dispatch(command,p,q,mode):
                self.slot.value=self.callbacks[1]
                C.c_uint8.from_address(q).value=0xCC
                return 0
            def complete(mode,q):
                self.assertEqual((mode,q,self.slot.value),(0,self.q,self.callbacks[1]))
                self.assertEqual(C.c_uint8.from_address(q).value,0xCC)
                self.slot.value=self.callbacks[2]
                C.c_uint8.from_address(q).value=0xDD
                return value
            self.install(dispatch);self.install_sync(complete)
            self.assertEqual(self.invoke_wait(7,None,self.q),int(value==2))
            self.assertEqual(len(self.events),1)
            self.assertEqual(len(self.sync_events),1)
            self.assertEqual((self.slot.value,self.result[0]),(self.callbacks[2],0xDD))

    def test_completion_wrapper_forwards_signed_result_mode_and_full_pointer(self):
        for mode in (-2147483648,-1,0,1,2147483647):
            for value in (-2147483648,-1,0,1,2,2147483647):
                for q in (None,self.q):
                    self.sync_events=[]
                    self.install_sync(lambda observed_mode,observed_q:value)
                    self.assertEqual(self.wrapper(mode,q),value)
                    if self.hook_errors:raise self.hook_errors.pop(0)
                    self.assertEqual(self.sync_events,[(mode,q,self.slot.value)])

    def test_wait_variant_fresh_metadata_and_nested_mode1_during_preparation(self):
        self.status.value=0x10;self.table[7]=0
        preparations=0;nested=[]
        def dispatch(command,p,q,mode):
            nonlocal preparations
            if mode==1:
                self.assertEqual((command,p,q,self.slot.value),(1,None,None,self.callbacks[1]))
                self.slot.value=self.callbacks[2]
                return 0
            if command==1:
                self.assertEqual((p,q,self.slot.value),(None,None,None))
                self.table[7]=1;self.status.value=0;self.slot.value=self.callbacks[1]
                return -123
            if command==2:
                self.assertEqual((p,q),(self.p,self.q))
                preparations+=1
                if preparations==1:
                    nested.append(self.invoke_mode1(1,None))
                    self.assertEqual(self.slot.value,self.callbacks[2])
                    return -1
                self.assertIsNone(self.slot.value)
                self.table[7]=0
                return 0
            self.assertEqual((command,p,q,self.slot.value),(7,self.p,self.q,self.callbacks[0]))
            self.slot.value=self.callbacks[1]
            return 0
        self.install(dispatch)
        self.assertEqual(self.invoke_wait(7,self.p,self.q),1)
        self.assertEqual(nested,[1])
        self.assertEqual([e[0] for e in self.events],[1,2,1,2,7])
        self.assertEqual(self.sync_events,[(0,self.q,self.callbacks[1])])
        self.assertEqual(self.slot.value,self.callbacks[1])

    def test_mode1_reentry_into_other_variant_keeps_independent_saved_callback(self):
        nested=[];main_calls=0
        def reenter(command,p,q,mode):
            nonlocal main_calls
            if mode==1:
                self.assertEqual((command,q,self.slot.value),(7,None,self.callbacks[0]))
                main_calls+=1
                if main_calls==1:
                    nested.append(self.invoke(1,None,self.q))
                    self.assertEqual(self.slot.value,self.callbacks[1])
                    return -1
                self.slot.value=self.callbacks[2]
            else:
                self.assertEqual((command,q),(1,self.q))
                self.slot.value=self.callbacks[1]
            return 0
        self.install(reenter)
        self.assertEqual(self.invoke_mode1(7,None),1)
        self.assertEqual(nested,[1])
        self.assertEqual([e[0] for e in self.events],[7,1,7])
        self.assertEqual(self.slot.value,self.callbacks[2])
