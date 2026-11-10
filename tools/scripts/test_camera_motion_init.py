"""Actual camera initialization, guarded native owners and a root-changing clear service."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

from overlay_units import function_extent

ROOT = Path(__file__).resolve().parents[2]


class CameraMotionInitTest(unittest.TestCase):
    def test_key_tables_root_replacement_and_full_native_pointers(self):
        source = (ROOT/'src/main/PAL/game/camera/camrun.c').read_text()
        first, last = function_extent(source, 'InitCameraMotion')
        body = re.sub(r'\s+asm\("\$2"\)', '', source[first:last])
        harness = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"PAL offset")
#include "rr/camera_motion.h"
#include "game/camera/camrun.h"
PresentationContextPrefix RaceContext;
CameraKey16 HudTachMeterSegments[4][3], HudGaugeArcVertices[4][2];
typedef struct {
    u32 before[8];
    PresentationController objects[2];
    u32 after[8];
} World;
static World actual, expected;
static unsigned replaceRoot, calls;
void PsyqBzero(void *buffer, s32 size) {
    assert(calls++ == 0);
    assert(buffer == &actual.objects[0].motion74);
    assert(size == sizeof(CameraMotion70));
    memset(buffer, 0, size);
    if (replaceRoot) RaceContext.controller3D8 = &actual.objects[1];
}
''' + body + r'''
static s32 word(u32 value) {s32 result;memcpy(&result,&value,4);return result;}
int main(void) {
    unsigned seed, mode, selection, row, key, cases = 0;
    CameraKey16 savedLinear[4][3], savedCyclic[4][2];
    const u32 edges[] = {0,1,0x7fffffffU,0x80000000U,0xffffffffU,0x12345678U,0x87654321U,4096};
    assert(sizeof(void*) == 8);
    assert((uintptr_t)&actual > UINT32_MAX && (uintptr_t)HudTachMeterSegments > UINT32_MAX);
    for (seed=0;seed<128;seed++) {
        for (row=0;row<4;row++) {
            for (key=0;key<3;key++) {
                CameraKey16 *p=&HudTachMeterSegments[row][key];
                p->x=word(edges[(seed+row+key)%8]);
                p->y=word(edges[(seed+row+key+3)%8]);
                p->z=word(edges[(seed+row+key+5)%8]);
                p->reservedC=word(0xfeed0000U+row*16+key);
                if (key<2) HudGaugeArcVertices[row][key]=*p;
            }
        }
        memcpy(savedLinear,HudTachMeterSegments,sizeof(savedLinear));
        memcpy(savedCyclic,HudGaugeArcVertices,sizeof(savedCyclic));
        for (mode=0;mode<2;mode++) for (selection=0;selection<8;selection++) {
            CameraKey16 *keys=selection<4?HudTachMeterSegments[selection]:HudGaugeArcVertices[selection-4];
            CameraKey16 initial=*keys;
            CameraMotion70 *target;
            memset(&actual,(u8)(seed*29U+0xA5U),sizeof(actual));
            memcpy(&expected,&actual,sizeof(expected));
            memset(&expected.objects[0].motion74,0,sizeof(CameraMotion70));
            target=&expected.objects[mode].motion74;
            target->mode04=selection<4?0:1;
            target->keys0C=keys;
            target->modelScale6C=720;
            target->start10[0]=target->current40[0]=initial.x;
            target->start10[1]=target->current40[1]=initial.y;
            target->start10[2]=target->current40[2]=initial.z;
            target->key08=1;
            replaceRoot=mode;calls=0;RaceContext.controller3D8=&actual.objects[0];
            InitCameraMotion(selection);
            assert(calls==1 && RaceContext.controller3D8==&actual.objects[mode]);
            assert(RaceContext.controller3D8->motion74.keys0C==keys);
            assert(!memcmp(&actual,&expected,sizeof(actual)));
            assert(!memcmp(savedLinear,HudTachMeterSegments,sizeof(savedLinear)));
            assert(!memcmp(savedCyclic,HudGaugeArcVertices,sizeof(savedCyclic)));
            cases++;
        }
    }
    printf("%u native cases: both key-table layouts, root replacement, full guarded owners\n",cases);
    return 0;
}
'''
        with TemporaryDirectory() as directory:
            path = Path(directory)
            c, exe = path/'init.c', path/'init'
            c.write_text(harness)
            subprocess.run(['cc','-std=gnu11','-O2','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                            '-I',str(ROOT/'include'),str(c),'-o',str(exe)],check=True)
            subprocess.run([str(exe)],check=True)


if __name__ == '__main__':
    unittest.main()
