"""Audio frame dispatch and player voices use one complete AudioState owner.

An independent Python model supplies full states at controlled service
boundaries. Actual native C, original MIPS and linked MIPS execute the same
reset/set/frame/update pipeline. No production body or ABI is rewritten.
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
AUDIO = 0xA8D0
SETTINGS, PAN, PARAMS = AUDIO, AUDIO+64, AUDIO+64+256
STATE = PARAMS+36
EVENT = 36+2*STATE
FRAMES, CALLS, MAX_EVENTS = 8, 7, 32
EVENTS_AT = 4+CALLS*24+FRAMES*STATE
STACK, STOP = 0x803F0000, 0x803FF000
NAMES = ('ResetPlayerVoices', 'SetPlayerVoice', 'SoundUpdate',
         'UpdatePlayerVoices', 'SetVoiceVolumeScaled', 'TriggerSfx')
SERVICES = {name: (number, count) for number, (name, count) in enumerate((
    ('CdAudioUpdate', 0), ('SeqFadeStep', 0), ('SoundMasterFadeStep', 0),
    ('SoundListenerUpdate', 0), ('UpdateSoundSlot', 1), ('UpdateLoopVoices', 0),
    ('UpdateMasterSoundVolumes', 0), ('SoundQueueUpdate', 0), ('SoundLoopUpdate', 0),
    ('SsUtKeyOffV', 1), ('SsUtKeyOnV', 8), ('SsUtSetVVol', 3), ('SsUtChangePitch', 7),
    ('SsUtKeyOn', 7)))}

NATIVE = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/audio_loop.h"
#include "rr/audio_pan.h"
#include "game/sound/sndslot.h"
#include "game/sound/sndown.h"
#include "game/sound/sndvol2.h"
#include "game/sound/sndchan.h"
#include "game/sound/sndmain.h"
#include "game/sound/sndcmd.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndrst2.h"
#include "game/sound/cdmain.h"
#include "game/sound/fxramp1.h"
typedef struct {
 u32 before[4];AudioState audio;u32 afterAudio[4];RuntimeSettings options;
 s16 pan[128];AudioPlayerVoiceParameters parameters[3];u32 after[4];
} World;
static World actual,initial;
static const u8 *trace;static unsigned count,eventCursor;
#define SoundState actual.audio
#define Options actual.options
#define PanGainTable actual.pan
#define PlayerVoiceParamTable actual.parameters
static u32 get32(const u8 *p){u32 v;memcpy(&v,p,4);return v;}
/* PAL trace words describe scalar fields. Native pointer cells and alignment
 * gaps remain native storage, protected by full-object comparisons. */
static void load(World *w,const u8 *p){
 AudioState *a=&w->audio;memcpy(a,p,0x2634);
 for(unsigned i=0;i<2;i++){
  memcpy(&a->streamBanks[i].sector,p+0x2634+i*8,4);
  assert(get32(p+0x2638+i*8)==0&&get32(p+0x2858+i*4)==0);
  const u8 *q=p+0x26F8+i*176;
  for(unsigned j=0;j<5;j++)assert(get32(q+j*4)==0);
  memcpy((u8*)&a->sequences[i]+offsetof(SequenceChannel,b14),q+20,156);
 }
 memcpy((u8*)a+offsetof(AudioState,reserved2644),p+0x2644,0x26F8-0x2644);
 memcpy((u8*)a+offsetof(AudioState,seqInit2860),p+0x2860,0xA8D0-0x2860);
 memcpy(&w->options,p+0xA8D0,64);memcpy(w->pan,p+0xA8D0+64,256);
 memcpy(w->parameters,p+0xA8D0+64+256,36);
}
static void check(const u8 *state){
 World wanted;memcpy(&wanted,&initial,sizeof(World));load(&wanted,state);
 assert(!memcmp(&actual,&wanted,sizeof(World)));
}
static s32 event(u32 kind,const u32 args[8]){
 assert(eventCursor<count);const u8 *e=trace+@EVENTS_AT@+eventCursor*@EVENT@;
 assert(get32(e)==kind&&!memcmp(e+4,args,32));check(e+36);
 load(&actual,e+36+@STATE@);check(e+36+@STATE@);eventCursor++;
 return (s32)(0xA5A50000U|eventCursor);
}
void CdAudioUpdate(void){const u32 a[8]={0};event(0,a);}
void SeqFadeStep(void){const u32 a[8]={0};event(1,a);}
void SoundMasterFadeStep(void){const u32 a[8]={0};event(2,a);}
void SoundListenerUpdate(void){const u32 a[8]={0};event(3,a);}
void UpdateSoundSlot(int slot){const u32 a[8]={(u32)slot};event(4,a);}
void UpdateLoopVoices(void){const u32 a[8]={0};event(5,a);}
void UpdateMasterSoundVolumes(void){const u32 a[8]={0};event(6,a);}
void SoundQueueUpdate(void){const u32 a[8]={0};event(7,a);}
void SoundLoopUpdate(void){const u32 a[8]={0};event(8,a);}
s32 SsUtKeyOn(s32 bank,s32 program,s32 mode,s32 note,s32 fine,s32 left,s32 right){
 const u32 a[8]={bank,program,mode,note,fine,left,right};return event(13,a);
}
s32 SsUtKeyOffV(s32 voice){const u32 a[8]={voice};return event(9,a);}
s32 SsUtKeyOnV(s32 voice,s32 bank,s32 program,s32 mode,s32 group,s32 unused,s32 left,s32 right){
 const u32 a[8]={voice,bank,program,mode,group,unused,left,right};return event(10,a);
}
s32 SsUtSetVVol(s32 voice,s32 left,s32 right){const u32 a[8]={voice,left,right};return event(11,a);}
s32 SsUtChangePitch(s32 voice,s32 bank,s32 program,s32 group,s32 unused,u32 coarse,u32 fine){
 const u32 a[8]={voice,bank,program,group,unused,coarse,fine};return event(12,a);
}
'''

TAIL = r'''
void run_dispatch(const u8 *input){
 trace=input;count=get32(input);eventCursor=0;assert(count<=32);
 memset(&actual,0xA5,sizeof(World));
 for(unsigned i=0;i<2;i++){
  actual.audio.streamBanks[i].table=NULL;actual.audio.seqAddr2858[i]=NULL;
  SequenceChannel *c=&actual.audio.sequences[i];c->c0=c->c4=c->c8=c->cC=c->p10=NULL;
 }
 memcpy(&initial,&actual,sizeof(World));
 const u8 *frames=input+4+7*24;load(&actual,frames);check(frames);
 assert(sizeof(void*)==8&&(uintptr_t)&actual.audio>UINT32_MAX);
 for(unsigned i=0;i<7;i++){
  const u8 *call=input+4+i*24;s32 args[5];memcpy(args,call+4,20);
  switch(get32(call)){
  case 0:ResetPlayerVoices();break;
  case 1:SetPlayerVoice(args[0],args[1],args[2],args[3]);break;
  case 2:SoundUpdate();break;
  case 3:UpdatePlayerVoices();break;
  case 4:TriggerSfx(args[0],args[1],args[2],args[3],args[4]);break;
  default:assert(!"unknown controller");
  }
  check(frames+(i+1)*@STATE@);
 }
 assert(eventCursor==count);
}
'''


def s16(value):
    return C.c_int16(value).value


def s32(value):
    return C.c_int32(value).value


def div(value, denominator):
    return (abs(value)//abs(denominator)) * (-1 if (value < 0) != (denominator < 0) else 1)


class Model:
    def __init__(self, state, number):
        self.state = bytearray(state)
        self.number = number
        self.events = []

    def word(self, at):
        return struct.unpack_from('<i', self.state, at)[0]

    def half(self, at):
        return struct.unpack_from('<h', self.state, at)[0]

    def put(self, at, value):
        struct.pack_into('<I', self.state, at, value & 0xFFFFFFFF)

    def put_half(self, at, value):
        struct.pack_into('<H', self.state, at, value & 0xFFFF)

    def effect(self, kind, ordinal):
        if self.number & 16:
            for slot in range(4):
                at = 0x38+slot*0x620
                self.put(at, self.word(at) ^ ((self.number >> (slot+6)) & 1))
            self.put(0x2874, self.number*7919+ordinal)
            self.put(0x2878, self.number*123+ordinal)
        if self.number & 32:
            self.put_half(0x18C2, ordinal & 1)
            for player in range(2):
                at = 0x195C+player*28
                self.put(at, ordinal*17-player*31)
                self.put(at+4, 32760+ordinal+player)
                self.put(at+8, (self.number+ordinal+player) % 4)
                self.put(at+16, self.number*1234567+ordinal*8191)
                self.put(at+20, (ordinal*7+player*13) % 257-128)
                self.put(at+24, (ordinal+player) % 4)
        if self.number & 64:
            self.put_half(0x18, (self.number+ordinal*127) % 257-128)
            self.put(8, self.number*2654435761 ^ ordinal*0x9E3779B9)
            self.put(4, ordinal % 3-1)
            self.state[SETTINGS+0x31] = (ordinal*37+self.number) & 255
            self.put_half(SETTINGS+0x32, ordinal % 3-1)
            for slot in range(4):
                at = 0x38+slot*0x620
                self.put_half(at+0x24, (ordinal*11+slot*31) % 128)
                self.put_half(at+0x22, (ordinal*17+slot*5) % 257-128)
            for i in range(128):
                self.put_half(PAN+i*2, (i*127+ordinal*313+kind*97) % 16130)
            for bank in (0, 1, 7):
                self.put_half(0x24+bank*2, self.half(0x24+bank*2)+321+ordinal)

    def emit(self, kind, *args):
        args = tuple(v & 0xFFFFFFFF for v in args) + (0,)*(8-len(args))
        before = bytes(self.state)
        self.effect(kind, len(self.events))
        self.events.append(struct.pack('<9I', kind, *args)+before+bytes(self.state))

    def reset(self):
        for player in range(2):
            at = 0x195C+player*28
            self.put(at, self.half(0x26))
            self.put(at+4, 12+player)
            self.put(at+8, 0)

    def set_player(self, mode, pitch, volume, player):
        at = 0x195C+player*28
        self.put(at+8, mode)
        if mode:
            self.put(at+16, pitch)
            self.put(at+20, volume)

    def players(self):
        for player in range(2):
            at = 0x195C+player*28
            mode = self.word(at+8)
            if self.word(at+12) != mode:
                if mode == 0:
                    self.emit(9, s16(self.word(at+4)))
                else:
                    self.emit(10, s16(self.word(at+4)), s16(self.word(at)),
                              s16(self.word(PARAMS+(mode-1)*12)), 0, 60, 0, 0, 0)
                self.put(at+12, self.word(at+8))
            mode = self.word(at+8)
            if not mode:
                continue
            program = self.word(PARAMS+(mode-1)*12)
            coarse, fine = self.word(at+16) >> 7, self.word(at+16) & 127
            volume = div(self.word(at+20)*self.word(PARAMS+(mode-1)*12+8), 128)
            slot = player if self.half(0x18C2) else (0 if player == 0 else self.word(0x195C+28+24))
            slot_at = 0x38+slot*0x620
            pan = self.half(slot_at+0x24)
            right = div(self.half(PAN+pan*2)*volume, 16129)
            left = div(self.half(PAN+(127-pan)*2)*volume, 16129)
            gain = self.half(slot_at+0x22)
            left, right = div(left*gain, 128), div(right*gain, 128)
            if self.half(SETTINGS+0x32):
                left = right = div(left+right, 2)
            gain = self.half(0x18)
            left, right = div(left*gain, 128), div(right*gain, 128)
            volume = self.state[SETTINGS+0x31]
            left, right = div(left*volume, 16), div(right*volume, 16)
            if self.word(4):
                left = right = 0
            scale = self.word(8)
            self.emit(11, s16(self.word(at+4)), s16(((left*scale) & 0xFFFFFFFF) >> 7),
                      s16(((right*scale) & 0xFFFFFFFF) >> 7))
            self.emit(12, s16(self.word(at+4)), s16(self.word(at)), s16(program),
                      60, 0, s16(coarse), fine)

    def sfx(self, program, level, pan, pitch, voice):
        bank_index, offset = (0, 0) if program < 30 else ((1, 30) if program < 60 else (7, 60))
        bank, program = self.half(0x24+bank_index*2), s16(program-offset)
        right = s16(div(self.half(PAN+(pan & 127)*2)*s16(level), 16129))
        left = s16(div(self.half(PAN+(127-(pan & 127))*2)*s16(level), 16129))
        if self.half(SETTINGS+0x32):
            left = right = div(left+right, 2)
        gain = self.half(0x18)
        left, right = s16(div(left*gain, 128)), s16(div(right*gain, 128))
        volume = self.state[SETTINGS+0x31]
        left, right = s16(div(left*volume, 16)), s16(div(right*volume, 16))
        raw = s32(pitch+60)
        note, fine = s16(div(raw, 128)), s16(raw-div(raw, 128)*128)
        if voice == -1:
            self.emit(13, bank, program, 0, note, fine, left, right)
            self.emit(13, bank, program, 1, note, fine, left, right)
        else:
            self.emit(10, s16(voice*2), bank, program, 0, note, fine, left, right)
            self.emit(10, s16(voice*2+1), bank, program, 1, note, fine, left, right)

    def frame(self):
        beat = self.word(0x2874)
        if beat != self.word(0x2878):
            self.put_half(0x286C, 1)
            if not ((beat-1) & 1):
                self.put_half(0x286E, 1)
            if not ((beat-1) & 3):
                self.put_half(0x2870, 1)
        else:
            for at in (0x286C, 0x286E, 0x2870):
                self.put_half(at, 0)
        self.put(0x2878, beat)
        for kind in range(4):
            self.emit(kind)
        for slot in range(4):
            if self.word(0x38+slot*0x620) & 1:
                self.emit(4, slot)
        self.players()
        for kind in range(5, 9):
            self.emit(kind)


class AudioDispatchOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[name].read_text(), name) for name in NAMES)
        assert 'asm' not in bodies, 'Actual bodies must compile without native rewrites'
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        source = Path(cls.temp.name)/'audio.c'
        library = source.with_suffix('.dylib')
        code = NATIVE+bodies+TAIL
        for name, value in {'EVENTS_AT': EVENTS_AT, 'EVENT': EVENT, 'STATE': STATE}.items():
            code = code.replace('@'+name+'@', str(value)+'U')
        source.write_text(code)
        proc = subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                               '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                               '-I', str(ROOT/'include'), str(source), '-o', str(library)], capture_output=True, text=True)
        if proc.returncode:
            raise AssertionError(proc.stderr)
        cls.native = C.CDLL(str(library))
        cls.native.run_dispatch.argtypes = [C.c_void_p]

    def test_real_owner_pipeline_with_mutable_services_on_both_images(self):
        audio, options, pan, params = map(address_of, ('SoundState', 'Options', 'PanGainTable', 'PlayerVoiceParamTable'))
        for alias, offset in [('SoundSlots', 0x38), ('PlayerEngineVoiceState', 0x195C),
                              ('VoiceVolumeScale', 8), ('SoundMasterGain', 0x18),
                              ('AudioPanCenterAngle', 0x24), ('AudioPanRearAngle', 0x26),
                              ('AudioPanFrontAngle', 0x32)]:
            self.assertEqual(address_of(alias), audio+offset)
        regions = [(audio, AUDIO), (options, 64), (pan, 256), (params, 36)]
        machines = []
        for label, path in [('original', ROOT/'assets/PAL/main.exe'), ('linked', ROOT/'build/PAL/main.exe')]:
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000)
            image = path.read_bytes()
            memory.mem_write(struct.unpack_from('<I', image, 0x18)[0] & 0x1FFFFFFF, image[0x800:])
            machines.append((label, memory))
        service_addresses = {address_of(name): descriptor for name, descriptor in SERVICES.items()}
        entries = ('ResetPlayerVoices', 'SetPlayerVoice', 'SoundUpdate', 'UpdatePlayerVoices', 'TriggerSfx')
        seen_services, seen_modes, seen_slots, seen_masks = set(), set(), set(), set()
        rng = random.Random(0x195C0038)
        for number in range(1024):
            initial = bytearray(rng.randbytes(STATE))
            for i in range(2):
                for at in (0x2638+i*8, 0x2858+i*4):
                    struct.pack_into('<I', initial, at, 0)
                initial[0x26F8+i*176:0x26F8+i*176+20] = bytes(20)
            model = Model(initial, number)
            model.put(4, 0 if number & 1 else -1)
            model.put(8, rng.getrandbits(32))
            model.put_half(0x18, (-32768, -128, 0, 1, 127, 128, 32767)[number % 7])
            model.put_half(0x18C2, (number//4) & 1)
            model.state[SETTINGS+0x31] = (0, 1, 16, 127, 255)[number % 5]
            model.put_half(SETTINGS+0x32, 0 if number & 2 else -1)
            beat = (0, 1, 2, 3, 4, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)[number % 8]
            model.put(0x2874, beat)
            model.put(0x2878, beat if number & 8 else beat ^ 1)
            for slot in range(4):
                at = 0x38+slot*0x620
                model.put(at, (model.word(at) & ~1) | ((number >> slot) & 1))
                model.put_half(at+0x24, (number+slot*31) % 128)
                model.put_half(at+0x22, (number+slot*17) % 257-128)
            for player in range(2):
                at = 0x195C+player*28
                model.put(at+12, (number//16+player) % 5-1)
                model.put(at+24, (number+player) % 4)
            for i in range(128):
                model.put_half(PAN+i*2, (i*127+number*53) % 16130)
            for i in range(3):
                model.put(PARAMS+i*12, rng.getrandbits(32))
                model.put(PARAMS+i*12+8, (number*31+i*17) % 257-128)
            pitch = s32(rng.getrandbits(32))
            calls = [(0, ()), (1, (number % 4, pitch, number % 257-128, 0)),
                     (1, ((number//4) % 4, s32(pitch ^ 0xA5A51234), 128-number % 257, 1)),
                     (2, ()), (3, ())]
            program = (-2147483648, -1, 0, 29, 30, 59, 60, 32767, 2147483647)[number % 9]
            calls += [(4, (program, s32(rng.getrandbits(32)), s32(rng.getrandbits(32)), pitch, -1)),
                      (4, (program, s32(rng.getrandbits(32)), s32(rng.getrandbits(32)), s32(pitch ^ 0xC0123456), s32(rng.getrandbits(32))))]
            frames = [bytes(model.state)]
            for identifier, args in calls:
                if identifier == 0:
                    model.reset()
                elif identifier == 1:
                    model.set_player(*args)
                    seen_modes.add(args[0])
                elif identifier == 2:
                    model.frame()
                elif identifier == 3:
                    model.players()
                else:
                    model.sfx(*args)
                frames.append(bytes(model.state))
            self.assertLessEqual(len(model.events), MAX_EVENTS)
            metadata = b''.join(struct.pack('<6I', identifier, *(v & 0xFFFFFFFF for v in args),
                                             *((0,)*(5-len(args)))) for identifier, args in calls)
            trace = struct.pack('<I', len(model.events))+metadata+b''.join(frames)+b''.join(model.events)
            self.native.run_dispatch(C.create_string_buffer(trace))
            seen_masks.add(number & 15)
            for label, memory in machines:
                read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
                write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))

                def load(state):
                    cursor = 0
                    for a, n in regions:
                        write(a, state[cursor:cursor+n])
                        cursor += n
                    self.assertEqual(cursor, STATE)

                def snapshot():
                    return b''.join(read(a, n) for a, n in regions)

                def store(cpu, access, address, length, value, user):
                    address &= 0x1FFFFFFF
                    self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n
                                        for a, n in [(audio, AUDIO), (STACK-0x400, 0x400)]),
                                    (number, label, hex(cpu.pc), hex(address), length))

                cpu = Cpu(memory, 0, 100, 101, 102, store)
                load(frames[0])
                write(audio-16, b'\xC6'*16)
                write(audio+AUDIO, b'\x6C'*16)
                event_index = 0
                for frame, (identifier, args) in enumerate(calls, 1):
                    cpu.reset()
                    cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                    cpu.r[4:4+min(4, len(args))] = [v & 0xFFFFFFFF for v in args[:4]]
                    cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(entries[identifier])
                    write(STACK-0x410, b'\xA5'*0x440)
                    for i, value in enumerate(args[4:]):
                        write(STACK+16+i*4, struct.pack('<I', value & 0xFFFFFFFF))
                    for _ in range(65536):
                        if cpu.pc == STOP:
                            break
                        if cpu.pc in service_addresses:
                            kind, argc = service_addresses[cpu.pc]
                            self.assertLess(event_index, len(model.events))
                            event = model.events[event_index]
                            event_index += 1
                            wanted = struct.unpack_from('<9I', event)
                            got = [cpu.r[4+i] if i < 4 else int.from_bytes(read(cpu.r[29]+16+(i-4)*4, 4), 'little') for i in range(argc)]
                            self.assertEqual((kind, *got, *((0,)*(8-argc))), wanted, (number, label, frame, event_index))
                            self.assertEqual(snapshot(), event[36:36+STATE], (number, label, frame, 'before service', event_index))
                            load(event[36+STATE:])
                            seen_services.add(kind)
                            if kind == 4:
                                seen_slots.add(got[0])
                            for reg in (2, 3, *range(4, 16), 24, 25):
                                cpu.r[reg] = 0xBAD00000+reg
                            cpu.r[2] = 0xA5A50000 | event_index
                            cpu.pc = cpu.r[31]
                            continue
                        cpu.step()
                    self.assertEqual((cpu.pc, cpu.r[29]), (STOP, STACK), (number, label, frame))
                    for reg in (*range(16, 24), 28, 30):
                        self.assertEqual(cpu.r[reg], 0xBEEF0000+reg)
                    self.assertEqual(snapshot(), frames[frame], (number, label, frame))
                    self.assertEqual(read(STACK-0x410, 16), b'\xA5'*16)
                    self.assertEqual(read(STACK, 16), b'\xA5'*16)
                    self.assertEqual(read(audio-16, 16), b'\xC6'*16)
                    self.assertEqual(read(audio+AUDIO, 16), b'\x6C'*16)
                self.assertEqual(event_index, len(model.events))
        self.assertEqual(seen_services, set(range(14)))
        self.assertEqual(seen_modes, {0, 1, 2, 3})
        self.assertEqual(seen_slots, {0, 1, 2, 3})
        self.assertEqual(seen_masks, set(range(16)))


if __name__ == '__main__':
    unittest.main()
