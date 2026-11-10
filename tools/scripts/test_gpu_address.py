"""Real native DMA address translation, mirrors, bounds and invalid maps."""
from pathlib import Path
import resource
import signal
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/gpu_address.h"
static struct {u8 before[16],first[256],gap[32],second[256],after[16];} storage,initial;
static GpuDmaRegion regions[2],bad[2];
int main(int argc,char **argv){
 assert(sizeof(void*)==8&&(uintptr_t)&storage>UINT32_MAX);
 if(argc>1){
  assert(GpuDmaSetRegions(0,0));
  if(argv[1][0]=='r')GpuDmaReadPayloadByte(0x80180000U);
  else GpuDmaEncodeAddress(storage.first);
  return 99;
 }
 assert(GpuDmaSetRegions(0,0));u32 output=0xDEADBEEFU;
 assert(!GpuDmaResolveAddress(0,1)&&!GpuDmaAddressOf(storage.first,&output)&&output==0xDEADBEEFU);
 regions[0]=(GpuDmaRegion){storage.first,0x80180000U,256};
 regions[1]=(GpuDmaRegion){storage.second,0xA0190000U,256};
 for(unsigned sample=0;sample<256;sample++){
  for(unsigned i=0;i<sizeof(storage);i++)((u8*)&storage)[i]=(u8)(sample*19+i*29);
  memcpy(&initial,&storage,sizeof(storage));assert(GpuDmaSetRegions(regions,2));
  for(unsigned region=0;region<2;region++){
   const u8 *base=region?storage.second:storage.first;u32 cpu=region?0xA0190000U:0x80180000U;
   for(unsigned i=0;i<256;i++){
    assert(GpuDmaAddressOf(base+i,&output)&&output==cpu+i);
    assert(GpuDmaEncodeAddress(base+i)==cpu+i);
    assert(GpuDmaResolveAddress(cpu+i,1)==base+i);
    assert(GpuDmaResolveAddress((cpu&0x1FFFFFFFU)+i,256-i)==base+i);
    assert(GpuDmaResolveAddress(0x80000000U|(cpu&0x1FFFFFFFU)|i,1)==base+i);
    assert(GpuDmaResolveAddress(0xA0000000U|(cpu&0x1FFFFFFFU)|i,1)==base+i);
    if(i<=251)assert(GpuDmaReadPayloadByte(cpu+i)==base[i+4]);
   }
   assert(!GpuDmaResolveAddress(cpu,0));
   assert(!GpuDmaResolveAddress(cpu+255,2));
   assert(!GpuDmaResolveAddress(cpu+256,1));
   assert(!GpuDmaResolveAddress(cpu-1,1));
   assert(!GpuDmaResolveAddress(0x20000000U|(cpu&0x1FFFFFFFU),1));
   assert(!GpuDmaResolveAddress(0xE0000000U|(cpu&0x1FFFFFFFU),1));
   output=0x12345678;assert(!GpuDmaAddressOf(base+256,&output)&&output==0x12345678);
  }
  assert(!memcmp(&storage,&initial,sizeof(storage)));
 }
 /* Low-byte payload markers and high-byte GPU opcodes are distinct. */
 storage.first[4]=0xE6;storage.first[7]=0x3C;
 assert(GpuDmaReadPayloadByte(0x180000)==0xE6);
 storage.first[4]=0x17;storage.first[7]=0xE6;
 assert(GpuDmaReadPayloadByte(0xA0180000)==0x17);
 for(unsigned mode=0;mode<8;mode++){
  memcpy(bad,regions,sizeof(bad));
  if(mode==0)bad[1].address=0xA0180080U;
  if(mode==1)bad[1].native=storage.first+128;
  if(mode==2)bad[1].size=0;
  if(mode==3)bad[1].native=0;
  if(mode==4)bad[1].address=0x80FFFFFFU;
  if(mode==5)bad[1].address=0x20190000U;
  if(mode==6)bad[1].native=(const void*)(UINTPTR_MAX-3);
  if(mode==7)bad[1].address=0x81190000U;
  assert(!GpuDmaSetRegions(bad,2));
  assert(GpuDmaEncodeAddress(storage.second)==0xA0190000U);
 }
 assert(!GpuDmaSetRegions(0,1));
 assert(GpuDmaEncodeAddress(storage.first)==0x80180000U);
 assert(!GpuDmaAddressOf(storage.first,0));
 /* Adjacent bus spans stay separate native objects; a read cannot cross them. */
 regions[1].address=0x80180100U;assert(GpuDmaSetRegions(regions,2));
 assert(GpuDmaResolveAddress(0x180100,1)==storage.second);
 assert(!GpuDmaResolveAddress(0x1800FF,2));
 regions[0]=(GpuDmaRegion){storage.first,0x80FFFF00U,256};
 assert(GpuDmaSetRegions(regions,1));
 assert(GpuDmaResolveAddress(0xFFFFFF,1)==storage.first+255);
 assert(!GpuDmaResolveAddress(0x1000000,1));
 assert(GpuDmaSetRegions(0,0)&&!GpuDmaResolveAddress(0xFFFFFF,1));
 return 0;
}
'''


class GpuAddressTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        source = Path(cls.temporary.name) / 'address.c'
        source.write_text(SOURCE)
        cls.binary = source.with_suffix('')
        subprocess.run(['cc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                        '-I', str(ROOT / 'include'), str(source),
                        str(ROOT / 'src/native/gpu_address.c'), '-o', str(cls.binary)], check=True)
        # Invalid-boundary tests must not leave core dumps in the workspace.
        _, hard = resource.getrlimit(resource.RLIMIT_CORE)
        resource.setrlimit(resource.RLIMIT_CORE, (0, hard))

    def test_real_regions_mirrors_and_bounds(self):
        subprocess.run([str(self.binary)], check=True)

    def test_unmapped_encoded_address_aborts(self):
        result = subprocess.run([str(self.binary), 'read'], capture_output=True)
        self.assertEqual(result.returncode, -signal.SIGABRT)

    def test_unregistered_native_pointer_aborts(self):
        result = subprocess.run([str(self.binary), 'encode'], capture_output=True)
        self.assertEqual(result.returncode, -signal.SIGABRT)
