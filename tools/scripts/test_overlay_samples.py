"""Native behavior of the nine overlay samples; actual C and full host pointers."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT = Path(__file__).resolve().parents[2]


class OverlaySamplesTest(unittest.TestCase):
    def test_native_samples(self):
        bodies = []
        from verify_overlay_samples import SAMPLES
        for index, address, _ in SAMPLES:
            source = ROOT / f'src/overlays/PAL/{index}/func_{address:08X}.c'
            body = source.read_text()
            if source.parent.name == '663':
                literal = '(s32 *)0x1F800000'
                self.assertEqual(body.count(literal), 1)
                body = body.replace(literal, 'test_scratch')
            bodies.append(body)
        self.assertEqual(len(bodies), 9)
        harness = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "types.h"
#include <stddef.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"target size")
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"target offset")
#include "rr/overlay662_garage.h"
s16 D_800F4C6A, D_800F3A26, D_800F4D8A, D_800AD50A;
s32 D_800F2DE4, D_800ABEF8[0x320/4];
u8 *D_800ACC14, display_buffer[256];
s16 first[8], second[8], *D_800F3AFC;
u8 D_800ACC68;
RrGarageWorkingStorage ovl662_D_80127668;
s32 ovl667_D_80118EE0;
u32 *ovl668_D_80116E54;
s32 test_scratch[64];
static int scenario, step;
void func_80036068(void) { assert(scenario == 666 && step++ == 0); }
void func_8003CAB4(void) {
    assert((scenario == 659 && step == 0) || (scenario == 666 && step == 1));
    step++;
    D_800F4C6A = -8; D_800F3A26 = -9; D_800F4D8A = -10;
    D_800ACC14 = display_buffer;
}
void func_800207D4(void *a, s32 b, s32 c) {
    assert(scenario == 666 && step++ == 2);
    assert(a == display_buffer + 112 && b == 255 && c == 69);
    assert(D_800F4C6A == 1 && D_800F3A26 == 82 && D_800F4D8A == 0);
}
void func_80013890(s32 a) {
    assert(scenario == 663 && step++ == 0 && a == 5 && D_800F2DE4 == 0);
    memset(test_scratch, 0xA5, sizeof(test_scratch));
}
void func_8002E4F4(void *p) {
    assert(scenario == 663 && step++ == 1 && p == D_800ABEF8);
    for (int i = 0; i < 64; i++) {
        s32 expected = (s32)0xA5A5A5A5U;
        if(i == 6) expected = 120;
        if(i == 2) expected = -16;
        if(i == 3) expected = -248;
        if(i == 7 || i == 8) expected = 0;
        if(i == 4) expected = -96;
        assert(test_scratch[i] == expected);
    }
    memset(p, 0x5A, sizeof(D_800ABEF8));
}
void func_80037110(void) {
    assert(scenario == 663 && step++ == 2);
    for (int i = 0; i < 0x320/4; i++) {
        s32 expected = 0x5A5A5A5A;
        if(i == 10) expected = 308;
        if(i == 9) expected = 10;
        if(i == 21) expected = 2560;
        assert(D_800ABEF8[i] == expected);
    }
    D_800F3AFC = second;
}
void func_8002A604(void) {
    assert(scenario == 665 && step++ == 0);
    D_800ACC68 = 7;
}
void func_8003D228(s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g) {
    assert(scenario == 665 && step++ == 1 && D_800ACC68 == 2);
    assert(a == 0 && b == 12 && c == 304 && d == 296 && e == 88 && f == 0 && g == 0);
    D_800F3AFC = second;
}
void func_8002AF8C(s32 a,s32 b,s32 c) {
    assert(scenario == 665 && step++ == 2);
    assert(a == 1 && b == 1 && c == 0 && second[1] == 1);
}
''' + '\n'.join(bodies) + r'''
int main(void) {
    scenario = 659; step = 0;
    ovl659_func_801148CC();
    assert(step == 1 && D_800F4C6A == 1 && D_800F3A26 == 47 && D_800F4D8A == 0);
    ovl660_func_80114910(); assert(D_800AD50A == 0x131);
    ovl661_func_80117340(); assert(D_800AD50A == 0x137);
    for (unsigned i = 0; i < 65536; i++) {
        memset(&ovl662_D_80127668, 0xA5, sizeof(ovl662_D_80127668));
        s32 args[4] = {(s32)(0x12340000U|i), (s32)(0xFFFF0000U|i),
                       (s32)(0x80000000U|(i^0xFFFF)), (s32)(0x7FFF0000U|i)};
        ovl662_func_8011E7C0(args[0],args[1],args[2],args[3]);
        for (int j = 0; j < 32; j++) {
            u16 expected = j >= 24 && j < 28 ? (u16)args[j-24] : 0xA5A5;
            assert((u16)ovl662_D_80127668.halves[j] == expected);
        }
        u32 bits = (i << 16) | (i ^ 0xFFFF);
        ovl667_D_80118EE0 = (s32)bits;
        assert((u32)ovl667_func_80115048() == bits);
        u32 values[3] = {bits, ~bits, i};
        for (int j = 0; j < 3; j++) {
            ovl668_D_80116E54 = &values[j];
            assert(ovl668_func_8011688C() == values[j]);
        }
    }
    for (int module = 663; module <= 665; module += 2) {
        scenario = module; step = 0;
        memset(first, 0xA5, sizeof(first)); memset(second, 0x5A, sizeof(second));
        D_800F3AFC = first; D_800F2DE4 = 99;
        if (module == 663) ovl663_func_80115BA0(); else ovl665_func_80115C8C();
        assert(step == 3);
        for (int j = 0; j < 8; j++) {
            assert((u16)first[j] == 0xA5A5);
            assert((u16)second[j] == (j == 1 ? 1 : 0x5A5A));
        }
    }
    scenario = 666; step = 0;
    ovl666_func_801166D0(); assert(step == 3);
    return 0;
}
'''
        with TemporaryDirectory() as temporary:
            directory = Path(temporary)
            source = directory / 'samples.c'
            executable = directory / 'samples'
            source.write_text(harness)
            result = subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra',
                                     '-Werror', '-fsanitize=undefined',
                                     '-I', str(ROOT / 'include'), str(source),
                                     '-o', str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, '')


if __name__ == '__main__':
    unittest.main()
