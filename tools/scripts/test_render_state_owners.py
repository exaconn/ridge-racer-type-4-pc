"""Actual render-state APIs, light-preset caller and complete slot owners.

Native pointers remain above 4 GiB. A byte oracle checks the entire scene and
16 native slot records; no production body is rewritten for host compilation.
"""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from c_functions import definition_source, source_owners

ROOT = Path(__file__).resolve().parents[2]
PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/scene_camera_state.h"
#include "scene_context_wire.h"
#include "game/gfx/scene1.h"
#include "game/gfx/dispenv.h"
#include "game/data/cdfile2.h"
static struct World {
    u32 before[4]; SceneRaceContext scene; u32 guardScene[4];
    CarSlotBufferC8 slots[16]; u32 guardSlots[4];
    TrackLightPreset8 values[4]; u32 after[4];
} actual, expected;
typedef struct {
    u32 before[4]; FrozenNativeSceneInput578 scene; u32 guardScene[4];
    CarSlotBufferC8 slots[16]; u32 guardSlots[4];
    TrackLightPreset8 values[4]; TrackLightPreset8 *palette;
    s32 enabled; u32 after[4];
} FrozenWorldInput;
#define RaceContext actual.scene
#define CarSlotBuffers actual.slots
'''
TAIL = r'''
static u32 seed=0x1A6E517U;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static s32 signed_word(u32 bits){s32 n;memcpy(&n,&bits,4);return n;}
static void put32(void *owner,unsigned offset,u32 bits){memcpy((u8 *)owner+offset,&bits,4);}
static void color(u32 mode,u32 red,u32 green,u32 blue){
    put32(&expected.scene,0xDC,mode);
    u8 *p=(u8 *)&expected.scene;p[0xE4]=red;p[0xE5]=green;p[0xE6]=blue;
}
static void compare(void){assert(!memcmp(&actual,&expected,sizeof(actual)));rr_scene_resources_unchanged();}
int main(void){
    const u32 edges[]={0,1,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU,127,128,255,256,65535,65536};
    _Static_assert(offsetof(SceneRaceContext,overlayModeDC)==0xDC,"scene mode prefix");
    _Static_assert(offsetof(SceneRaceContext,overlayColorE4)==0xE4,"scene color prefix");
    _Static_assert(offsetof(CarSlotBufferC8,mainCount0C)==0x10,"native main count");
    _Static_assert(offsetof(CarSlotBufferC8,secondCount34)==0x58,"native second count");
    _Static_assert(offsetof(CarSlotBufferC8,phase5C)==0xA0,"native phase");
    _Static_assert(offsetof(CarSlotBufferC8,historyCount68)==0xAC,"native history count");
    _Static_assert(offsetof(CarSlotBufferC8,overlay70)==0xB4,"native overlay flag");
    unsigned seen[256]={0};
    for(unsigned scenario=0;scenario<8192;scenario++){
        FrozenWorldInput frozen;u8 *bytes=(u8 *)&frozen;
        for(unsigned i=0;i<sizeof(frozen);i++)bytes[i]=(u8)next();
        memset(&actual,0xA5,sizeof(actual));memcpy(actual.before,frozen.before,16);
        rr_scene_context_load_native578(&actual.scene,&frozen.scene);
        memcpy(actual.guardScene,frozen.guardScene,16);memcpy(actual.slots,frozen.slots,sizeof(actual.slots));
        memcpy(actual.guardSlots,frozen.guardSlots,16);memcpy(actual.values,frozen.values,sizeof(actual.values));
        memcpy(actual.after,frozen.after,16);
        actual.scene.lightPresets558=actual.values;
        for(unsigned i=0;i<4;i++){
            actual.values[i].mode=signed_word(next());actual.values[i].r=next();
            actual.values[i].g=next();actual.values[i].b=next();
        }
        assert(sizeof(void *)==8&&(uintptr_t)&actual.scene>UINT32_MAX);
        assert((uintptr_t)actual.slots>UINT32_MAX&&(uintptr_t)actual.scene.lightPresets558>UINT32_MAX);
        memcpy(&expected,&actual,sizeof(actual));
        InitSceneOverlayState();
        put32(&expected.scene,0xD4,0);put32(&expected.scene,0xD8,0);
        put32(&expected.scene,0xDC,0);put32(&expected.scene,0xE0,1);
        color(0,144,144,144);compare();
        u32 mode=edges[scenario%11],r=next(),g=next(),b=next();
        void (*set_color)(s32,s32,s32,s32)=SetSceneOverlayColor;
        set_color(signed_word(mode),signed_word(r),signed_word(g),signed_word(b));
        color(mode,r,g,b);compare();
        u32 enabled=edges[(scenario/11)%11];
        SetSceneOverlayEnabled(signed_word(enabled));expected.scene.overlayEnabledD4=signed_word(enabled);compare();
        ResetCarSlotBuffers();
        for(unsigned i=0;i<16;i++){
            put32(&expected.slots[i],0x10,0);put32(&expected.slots[i],0x58,0);
            put32(&expected.slots[i],0xA0,0);put32(&expected.slots[i],0xAC,0);
        }
        compare();
        unsigned selector=scenario%256;seen[selector]++;
        u32 raw=(next()&0xFFFFF00FU)|(selector<<4);
        ApplyTrackLightPreset(signed_word(raw));
        if(selector>=1&&selector<8){
            const TrackLightPreset8 *entry=&expected.values[(selector-1)%4];
            expected.scene.overlayEnabledD4=1;color((u32)entry->mode,entry->r,entry->g,entry->b);
        }else expected.scene.overlayEnabledD4=0;
        for(unsigned i=0;i<8;i++)put32(&expected.slots[i],0xB4,selector>=5);
        compare();
    }
    for(unsigned i=0;i<256;i++)assert(seen[i]==32);
    return 0;
}
'''


class RenderStateOwnersTest(unittest.TestCase):
    def test_raw_words_all_light_selectors_and_full_native_owners(self):
        owners=source_owners(ROOT/'src/main/PAL')
        names=('InitSceneOverlayState','SetSceneOverlayColor','SetSceneOverlayEnabled',
               'ResetCarSlotBuffers','ApplyTrackLightPreset')
        bodies=[definition_source(owners[n].read_text(),n) for n in names]
        for body in bodies:
            self.assertIsNone(re.search(r'\b(?:asm|__asm__)\s*\(',body))
        with TemporaryDirectory() as temporary:
            directory=Path(temporary);source=directory/'state.c';binary=directory/'state'
            source.write_text(PREFIX+'\n'.join(bodies)+TAIL)
            result=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror',
                                   '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                   '-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),str(source),'-o',str(binary)],
                                  capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            subprocess.run([str(binary)],check=True)


if __name__=='__main__':
    unittest.main()
