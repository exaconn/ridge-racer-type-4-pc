"""Race sound startup and stop use complete scene/audio owners.

An independent controller model checks whole native objects at every controlled
SDK/audio boundary. The identical trace drives original and linked MIPS; the
three scene setters and reverb shutdown execute their real implementations.
"""
import ctypes as C
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]
STATE = 0x57C + 0xA8D0 + 4
EVENT = 20 + 2 * STATE
MAX_EVENTS = 32
OUTPUT = 4 + 5 * STATE + MAX_EVENTS * EVENT
STACK, STOP = 0x803F0000, 0x803FF000
NAMES = ('SoundInitForRace', 'SoundStopAll', 'InitSceneOverlayState',
         'SetSceneOverlayColor', 'SetSceneOverlayEnabled', 'SoundReverbOffAll',
         'SoundPause', 'SoundResume')
SERVICES = {'SsUtAllKeyOff': (0, 1), 'SsSetReservedVoice': (1, 1),
            'SoundReset': (2, 2), 'SlotsStartForRace': (3, 0),
            'SlotKeyOffAll': (4, 1), 'SetPlayerVoice': (5, 4),
            'SetLoopVoiceVolumes': (6, 3), 'SetSoundChannelVolLR': (7, 3),
            'SoundLoopSet': (8, 4), 'CdAudioMute': (9, 0),
            'SsUtReverbOff': (10, 0), 'SsUtSetReverbType': (11, 1),
            'SsUtSetReverbDepth': (12, 2), 'DisableEngineSoundUpdates': (13, 0),
            'CdAudioRequestPause': (14, 0), 'SlotSetPause': (15, 2)}

PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/scene_camera_state.h"
#include "rr/audio_state_types.h"
#include "game/gfx/scene1.h"
#include "game/sound/sndcmd.h"
#include "game/sound/sndctrl.h"
#include "game/sound/sndown.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndslot.h"
#include "game/sound/sndrst2.h"
#include "game/sound/sndloop2.h"
#include "game/sound/cdstart.h"
#include "psyq/libsnd/vmvinit.h"
#include "psyq/libsnd/utrev3.h"
#include "rr/channels.h"
#include "rr/library.h"
typedef struct {
 u32 guard0[4];SceneRaceContext scene;u32 guard1[4];
 AudioState audio;u32 guard2[4];s32 mask;u32 guard3[4];
} World;
static World actual,expected;
static World before[32],after[32];
static s32 arguments[32][4];static u32 kinds[32];
static unsigned scenario,modelCount,eventCount;static u8 *events;
#define RaceContext actual.scene
#define SoundState actual.audio
#define MaskBitSetting actual.mask
static void put32(u8 *p,u32 value){memcpy(p,&value,4);}
static void wire(const World *w,u8 *out){
 const SceneRaceContext *s=&w->scene;const AudioState *a=&w->audio;
 assert(!s->standings&&!s->cameraEC.tracks138&&!s->camera24C);
 assert(!s->cameraNodes550&&!s->workspace554&&!s->lightPresets558&&!s->workspace55C);
 assert(!s->soundPayload564&&!s->lookupTables);
 memcpy(out,s,0xE8);put32(out+0xE8,0);
 memcpy(out+0xEC,&s->cameraEC,0x138);put32(out+0x224,0);
 memcpy(out+0x228,(const u8*)&s->cameraEC+offsetof(SceneCameraState160,mode13C),0x24);put32(out+0x24C,0);
 memcpy(out+0x250,(const u8*)s+offsetof(SceneRaceContext,reserved250),0x300);memset(out+0x550,0,16);
 memcpy(out+0x560,&s->model560,4);memset(out+0x564,0,8);
 memcpy(out+0x56C,&s->sound,16);out+=0x57C;
 memcpy(out,a,0x2634);
 for(unsigned i=0;i<2;i++){
  assert(!a->streamBanks[i].table&&!a->seqAddr2858[i]);
  memcpy(out+0x2634+i*8,&a->streamBanks[i].sector,4);put32(out+0x2638+i*8,0);
  const SequenceChannel *c=&a->sequences[i];assert(!c->c0&&!c->c4&&!c->c8&&!c->cC&&!c->p10);
  memset(out+0x26F8+i*176,0,20);
  memcpy(out+0x26F8+i*176+20,(const u8*)c+offsetof(SequenceChannel,b14),156);
  put32(out+0x2858+i*4,0);
 }
 memcpy(out+0x2644,(const u8*)a+offsetof(AudioState,reserved2644),0x26F8-0x2644);
 memcpy(out+0x2860,(const u8*)a+offsetof(AudioState,seqInit2860),0xA8D0-0x2860);out+=0xA8D0;
 memcpy(out,&w->mask,4);
}
/* Service effects deliberately alter later-read fields, including future
 * slots. These are environment behavior, not implementations of controllers. */
static void effect(World *w,unsigned kind,unsigned ordinal){
 if(scenario&16){
  w->scene.overlayToggle3DC=(s32)((scenario+ordinal+kind)%5)-2;
  w->scene.cameraNodeChanged1C=100+(s32)ordinal;
  w->scene.cameraNodeIndex4C=200+(s32)ordinal;
  w->scene.cameraNodeFrames48=300+(s32)ordinal;
  w->audio.split10=(s32)((ordinal+kind)%3)-1;
  w->audio.active0C=400+(s32)ordinal;w->audio.muted04=500+(s32)ordinal;
  w->audio.muted04=600+(s32)ordinal;w->mask=700+(s32)ordinal;
 }
 if(scenario&32)for(unsigned i=0;i<4;i++){
  w->audio.slots38[i].flags00^=(scenario>>(i+6))&1;
  w->audio.slots38[i].levels20.halves.level=(s16)(ordinal*9+i);
  w->audio.slots38[i].levels20.halves.gain=(s16)(ordinal*7+i);
 }
}
static void model_event(unsigned kind,s32 a,s32 b,s32 c,s32 d){
 unsigned n=modelCount++;assert(n<32);kinds[n]=kind;
 const s32 args[4]={a,b,c,d};memcpy(arguments[n],args,16);
 memcpy(&before[n],&expected,sizeof(World));effect(&expected,kind,n);
 memcpy(&after[n],&expected,sizeof(World));
}
static void event(unsigned kind,s32 a,s32 b,s32 c,s32 d){
 unsigned n=eventCount++;assert(n<modelCount);const s32 args[4]={a,b,c,d};
 assert(kinds[n]==kind&&!memcmp(arguments[n],args,16));
 assert(!memcmp(&actual,&before[n],sizeof(World)));
 u8 *p=events+n*@EVENT@;put32(p,kind);memcpy(p+4,args,16);wire(&actual,p+20);
 effect(&actual,kind,n);wire(&actual,p+20+@STATE@);
 assert(!memcmp(&actual,&after[n],sizeof(World)));
}
void SsUtAllKeyOff(s32 a){event(0,a,0,0,0);}
s32 SsSetReservedVoice(u32 a){event(1,(s32)a,0,0,0);return -17;}
void SoundReset(int a,int b){event(2,a,b,0,0);}
void SlotsStartForRace(void){event(3,0,0,0,0);}
void SlotKeyOffAll(int a){event(4,a,0,0,0);}
void SetPlayerVoice(int a,int b,int c,int d){event(5,a,b,c,d);}
void SetLoopVoiceVolumes(int a,int b,int c){event(6,a,b,c,0);}
void SetSoundChannelVolLR(int a,s16 b,s16 c){event(7,a,b,c,0);}
void SoundLoopSet(int a,int b,int c,int d){event(8,a,b,c,d);}
void CdAudioMute(void){event(9,0,0,0,0);}
void SsUtReverbOff(void){event(10,0,0,0,0);}
s32 SsUtSetReverbType(s32 a){event(11,a,0,0,0);return -17;}
void SsUtSetReverbDepth(s32 a,s32 b){event(12,a,b,0,0);}
void DisableEngineSoundUpdates(void){event(13,0,0,0,0);}
void CdAudioRequestPause(void){event(14,0,0,0,0);}
void SlotSetPause(int a,int b){event(15,a,b,0,0);}
'''

TAIL = r'''
static void model_start(void){
 model_event(0,0,0,0,0);model_event(1,4,0,0,0);
 expected.audio.active0C=1;expected.audio.muted04=0;
 model_event(2,expected.audio.split10,1,0,0);
 expected.scene.overlayBlendE0=1;expected.scene.overlayEnabledD4=0;
 expected.scene.overlayStyleD8=0;expected.scene.overlayModeDC=0;
 for(unsigned i=0;i<3;i++)expected.scene.overlayColorE4[i]=0x90;
 expected.mask=1;
 expected.scene.overlayEnabledD4=expected.scene.overlayToggle3DC;
 model_event(3,0,0,0,0);
 for(unsigned i=0;i<4;i++)expected.audio.slots38[i].levels20.halves.level=i<2?0x52:0x4E;
 expected.scene.cameraNodeIndex4C=-1;expected.scene.cameraNodeChanged1C=1;
 expected.scene.cameraNodeFrames48=0;
}
static void model_stop(void){
 for(unsigned i=0;i<4;i++)if(expected.audio.slots38[i].flags00&1)model_event(4,(s32)i,0,0,0);
 model_event(5,0,0,0,0);model_event(5,0,0,0,1);
 model_event(6,0,0,0,0);model_event(6,1,0,0,0);
 model_event(7,0,0,0,0);model_event(8,5,0x1E00,0,0x40);
 model_event(9,0,0,0,0);model_event(0,0,0,0,0);
 model_event(10,0,0,0,0);model_event(11,0,0,0,0);model_event(12,0,0,0,0);
 model_event(13,0,0,0,0);expected.audio.muted04=1;
}
void run_sound_scene(unsigned number,u8 *out){
 scenario=number;modelCount=eventCount=0;memset(&actual,0xA5,sizeof(World));
 u32 seed=number+1;
 for(unsigned i=0;i<sizeof(actual.scene);i++){seed=seed*1664525U+1013904223U;((u8*)&actual.scene)[i]=(u8)(seed>>24);}
 for(unsigned i=0;i<sizeof(actual.audio);i++){seed=seed*1664525U+1013904223U;((u8*)&actual.audio)[i]=(u8)(seed>>24);}
 SceneRaceContext *s=&actual.scene;s->standings=NULL;s->cameraEC.tracks138=NULL;s->camera24C=NULL;
 s->cameraNodes550=NULL;s->workspace554=NULL;s->lightPresets558=NULL;s->workspace55C=NULL;
 s->soundPayload564=NULL;s->lookupTables=NULL;
 for(unsigned i=0;i<2;i++){
  actual.audio.streamBanks[i].table=NULL;actual.audio.seqAddr2858[i]=NULL;
  SequenceChannel *c=&actual.audio.sequences[i];c->c0=c->c4=c->c8=c->cC=c->p10=NULL;
 }
 for(unsigned i=0;i<4;i++)actual.audio.slots38[i].flags00=(actual.audio.slots38[i].flags00&~1U)|((number>>i)&1);
 actual.audio.split10=(s32)(number%3)-1;actual.audio.muted04=-7;actual.mask=-9;
 assert(sizeof(void*)==8&&(uintptr_t)&actual.scene>UINT32_MAX&&(uintptr_t)&actual.audio>UINT32_MAX);
 memcpy(&expected,&actual,sizeof(World));wire(&actual,out+4);
 events=out+4+5*@STATE@;model_start();SoundInitForRace();
 assert(!memcmp(&actual,&expected,sizeof(World)));wire(&actual,out+4+@STATE@);
 model_stop();SoundStopAll();
 assert(eventCount==modelCount&&!memcmp(&actual,&expected,sizeof(World)));
 wire(&actual,out+4+2*@STATE@);
 for(s32 pause=1;pause>=0;pause--){
  expected.audio.muted04=pause;model_event(14,0,0,0,0);
  for(s32 i=0;i<4;i++)model_event(15,i,pause,0,0);
  if(pause)SoundPause();else SoundResume();
  assert(eventCount==modelCount&&!memcmp(&actual,&expected,sizeof(World)));
  wire(&actual,out+4+(4U-(unsigned)pause)*@STATE@);
 }
 put32(out,eventCount);
 for(unsigned i=0;i<4;i++)assert(actual.guard0[i]==0xA5A5A5A5U&&actual.guard1[i]==0xA5A5A5A5U&&actual.guard2[i]==0xA5A5A5A5U&&actual.guard3[i]==0xA5A5A5A5U);
}
'''


class SoundSceneOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = '\n'.join(definition_source(owners[n].read_text(), n) for n in NAMES)
        assert 'asm' not in bodies, 'Controllers must compile without native body rewrites'
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        source = Path(cls.temp.name)/'sound.c'
        library = source.with_suffix('.dylib')
        source.write_text((PREFIX + bodies + TAIL).replace('@STATE@', str(STATE)+'U').replace('@EVENT@', str(EVENT)+'U'))
        proc = subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                               '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                               '-I', str(ROOT/'include'), str(source), '-o', str(library)], capture_output=True, text=True)
        if proc.returncode:
            raise AssertionError(proc.stderr)
        cls.native = C.CDLL(str(library))
        cls.native.run_sound_scene.argtypes = [C.c_uint, C.c_void_p]

    def test_live_scene_startup_and_slot_shutdown_on_both_images(self):
        scene, audio, paused, mask = map(address_of, ('RaceContext', 'SoundState', 'SoundPaused', 'MaskBitSetting'))
        self.assertEqual(paused, audio+4)
        regions = [(scene, 0x57C), (audio, 0xA8D0), (mask, 4)]
        machines = []
        for label, path in [('original', ROOT/'assets/PAL/main.exe'), ('linked', ROOT/'build/PAL/main.exe')]:
            memory = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
            memory.mem_map(0, 0x400000)
            image = path.read_bytes()
            memory.mem_write(struct.unpack_from('<I', image, 0x18)[0] & 0x1FFFFFFF, image[0x800:])
            machines.append((label, memory))
        service_addresses = {address_of(n): v for n, v in SERVICES.items()}
        seen = set()
        slot_patterns = set()
        for number in range(1024):
            output = C.create_string_buffer(OUTPUT)
            self.native.run_sound_scene(number, output)
            raw = output.raw
            count = struct.unpack_from('<I', raw)[0]
            self.assertLessEqual(count, MAX_EVENTS)
            frames = [raw[4+i*STATE:4+(i+1)*STATE] for i in range(5)]
            events = [raw[4+5*STATE+i*EVENT:4+5*STATE+(i+1)*EVENT] for i in range(count)]
            slot_patterns.add(tuple(struct.unpack_from('<I', frames[0], 0x57C+0x38+i*0x620)[0] & 1 for i in range(4)))
            for label, memory in machines:
                read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
                write = lambda a, data: memory.mem_write(a & 0x1FFFFFFF, bytes(data))

                def load(data):
                    p = 0
                    for a, n in regions:
                        write(a, data[p:p+n])
                        p += n
                    self.assertEqual(p, STATE)

                def snapshot():
                    return b''.join(read(a, n) for a, n in regions)

                def store(cpu, access, address, length, value, user):
                    address &= 0x1FFFFFFF
                    writable = regions + [(STACK-0x200, 0x200)]
                    self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n for a, n in writable),
                                    (number, label, hex(cpu.pc), hex(address), length))

                cpu = Cpu(memory, 0, 100, 101, 102, store)
                load(frames[0])
                for a, n in regions[:2]:
                    write(a-16, b'\xC6'*16)
                    write(a+n, b'\x6C'*16)
                event_index = 0
                for frame, name in enumerate(('SoundInitForRace', 'SoundStopAll', 'SoundPause', 'SoundResume'), 1):
                    cpu.reset()
                    cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                    cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(name)
                    write(STACK-0x210, b'\xA5'*0x220)
                    for _ in range(16384):
                        if cpu.pc == STOP:
                            break
                        if cpu.pc in service_addresses:
                            kind, argc = service_addresses[cpu.pc]
                            self.assertLess(event_index, count)
                            event = events[event_index]
                            event_index += 1
                            wanted = struct.unpack_from('<5I', event)
                            args = cpu.r[4:4+argc] + [0]*(4-argc)
                            self.assertEqual((kind, *args), wanted, (number, label, name, event_index))
                            self.assertEqual(snapshot(), event[20:20+STATE], (number, label, name, 'before service', event_index))
                            load(event[20+STATE:])
                            seen.add(kind)
                            cpu.r[2] = 0xFFFFFFEF
                            cpu.pc = cpu.r[31]
                            continue
                        cpu.step()
                    self.assertEqual((cpu.pc, cpu.r[29]), (STOP, STACK), (number, label, name))
                    for reg in (*range(16, 24), 28, 30):
                        self.assertEqual(cpu.r[reg], 0xBEEF0000+reg)
                    self.assertEqual(snapshot(), frames[frame], (number, label, name))
                    self.assertEqual(read(STACK-0x210, 16), b'\xA5'*16)
                    self.assertEqual(read(STACK, 16), b'\xA5'*16)
                    for a, n in regions[:2]:
                        self.assertEqual(read(a-16, 16), b'\xC6'*16)
                        self.assertEqual(read(a+n, 16), b'\x6C'*16)
                self.assertEqual(event_index, count)
        self.assertEqual(seen, set(range(16)))
        self.assertEqual(len(slot_patterns), 16)


if __name__ == '__main__':
    unittest.main()
