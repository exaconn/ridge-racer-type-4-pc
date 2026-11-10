"""Actual pad queue installer stores complete native callbacks in its owner."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source
ROOT=Path(__file__).resolve().parents[2]
SETUP=r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "psyq/libpad/padenab.h"
#include "psyq/libpad/padintr.h"
PadInterruptQueueRecord PadIntQueueEntry;
static int handlerCalls,testCalls;
s32 _padIntPad(void) {handlerCalls++;return -123456789;}
s32 _padIsVsync(void) {testCalls++;return 123456789;}
static s32 other(void) {assert(0);return 0;}
'''
CHECKS=r'''
int main(void) {
 unsigned seed,i;
 assert(sizeof(PadInterruptHandler)==sizeof(void *) && sizeof(void *)>4);
 for(seed=0;seed<4096;seed++) {
  PadInterruptQueueRecord expected;
  for(i=0;i<sizeof(PadIntQueueEntry);i++)((u8 *)&PadIntQueueEntry)[i]=(u8)(i*17+seed);
  PadIntQueueEntry.next00=&PadIntQueueEntry;
  PadIntQueueEntry.callbacks04[0]=other;PadIntQueueEntry.callbacks04[1]=other;
  memcpy(&expected,&PadIntQueueEntry,sizeof(expected));
  expected.next00=0;expected.callbacks04[PAD_INTERRUPT_HANDLER]=_padIntPad;
  expected.callbacks04[PAD_INTERRUPT_TEST]=_padIsVsync;expected.reserved0C=0;
  _padSetVsyncParam();assert(!memcmp(&expected,&PadIntQueueEntry,sizeof(expected)));
  assert(PadIntQueueEntry.callbacks04[PAD_INTERRUPT_HANDLER]()==-123456789);
  assert(PadIntQueueEntry.callbacks04[PAD_INTERRUPT_TEST]()==123456789);
  assert(!memcmp(&expected,&PadIntQueueEntry,sizeof(expected)));
 }
 assert(handlerCalls==4096 && testCalls==4096);
 puts("4096 complete native callback installations and both typed invocations passed");return 0;
}
'''
class ChannelInterruptQueueTest(unittest.TestCase):
 def test_real_installer_owns_full_native_callback_slots(self):
  owners=source_owners(ROOT/'src/main/PAL')
  source=SETUP+definition_source(owners['_padSetVsyncParam'].read_text(),'_padSetVsyncParam')+CHECKS
  with TemporaryDirectory() as directory:
   c=Path(directory)/'queue.c';exe=Path(directory)/'queue';c.write_text(source)
   r=subprocess.run(['cc','-std=gnu99','-O2','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',
    '-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
   self.assertIn('4096 complete native callback installations',r.stdout)
