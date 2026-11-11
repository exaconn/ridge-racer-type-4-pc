from selection_fixture_transport import transport as transport_selection
"""Menu choice: mutable helpers, repeated byte records and native buffer publication."""
from menu_glyph_fixture_transport import transport
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import definition_source

ROOT=Path(__file__).resolve().parents[2]


class MenuChoiceTest(unittest.TestCase):
    def test_actual_body_and_complete_mutable_owners(self):
        s=definition_source((ROOT/'src/main/PAL/game/menu/mnchoice.c').read_text(),'StepResultsMenuChoice')
        # This adapter transports only the actual field this caller accesses.
        # Keep the legacy World, PRNG stream and reference model unchanged.
        self.assertEqual(['b4'],re.findall(r'Dialogue->(\w+)',s))
        prefix=r'''
#include <assert.h>
#include "rr/save_slot_types.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "rr/layout.h"
#include "rr/menu_choice.h"
_Static_assert(sizeof(UiRow24)==36,"UI row");
_Static_assert(offsetof(UiRow24,cells)==12,"cells");
_Static_assert(offsetof(UiRow24,byte14)==20,"accept byte");
_Static_assert(offsetof(UiRow24,byte16)==22,"selection byte");
_Static_assert(sizeof(MenuGlyphRow1C)==28,"glyph stride");
_Static_assert(offsetof(MenuGlyphRow1C,flags00)==0,"flags");
_Static_assert(offsetof(MenuGlyphRow1C,control02)==2,"control");
_Static_assert(offsetof(MenuGlyphRow1C,phase18)==24,"phase");
_Static_assert(sizeof(unsigned long)>=sizeof(void*),"full pointer arithmetic");
_Static_assert(sizeof(MenuChoice4)==4,"packed choice");
_Static_assert(_Alignof(MenuChoice4)==1,"byte alignment");
_Static_assert(sizeof(SavedMenuSettings)==44,"settings owner");
_Static_assert(offsetof(SavedMenuSettings,fields.mode1A)==26,"mode");
_Static_assert(offsetof(SavedMenuSettings,fields.copies)==28,"records");
_Static_assert(sizeof(BadgeRow1C)==28,"control row");
_Static_assert(offsetof(BadgeRow1C,state10)==16,"state");
typedef union {uint64_t align;u8 bytes[288];} UiOwner;
typedef struct {MenuChoice4 choice;SavedMenuSettings settings;BadgeRow1C rows[2];UiOwner ui[2];MenuGlyphRow1C slots[4];u8 highlight[2][8];s16 selected;u16 buttons;s32 event,accepted;int rowbank,uibank,hintbank;void *resource,*current;u8 canary[24];} World;
typedef struct {int kind;s32 args[3];World snapshot;} Entry;
static World actual,expected,*w;
static Entry entries[8],actual_entries[8];
static int count,mutation;
static unsigned seed;
static uint64_t arena[32][8];

/* The original eight-byte fixture prefix remains in World so its complete
 * deterministic input stream and reference snapshots are unchanged. Actual C
 * receives two complete MenuTextState objects. Only PAL byte4 maps to b4. */
static MenuTextState dialogue_owners[2],dialogue_before[2];
static u16 dialogue_words[2][8];
static MenuTextState *native_dialogue(World *owner){
 assert(owner==&actual);return &dialogue_owners[owner->hintbank];
}
static void sync_dialogue(void){
 if(w==&actual)for(unsigned i=0;i<2;i++)actual.highlight[i][4]=(u8)dialogue_owners[i].b4;
}
static void reset_dialogue(void){
 for(unsigned i=0;i<2;i++){
  memset(&dialogue_owners[i],0xA7,sizeof(MenuTextState));
  dialogue_owners[i].p=dialogue_words[i];dialogue_owners[i].images20=(u8*)arena[i];
  dialogue_owners[i].b4=(s8)actual.highlight[i][4];
  memcpy(&dialogue_before[i],&dialogue_owners[i],sizeof(MenuTextState));
  assert((uintptr_t)&dialogue_owners[i]>UINT32_MAX);
 }
}
static void verify_dialogue(void){
 for(unsigned i=0;i<2;i++){
  dialogue_before[i].b4=(s8)actual.highlight[i][4];
  assert(!memcmp(&dialogue_before[i],&dialogue_owners[i],sizeof(MenuTextState)));
 }
}
#define ActiveSelectionRowValue (w->choice.bytes[1])
#define SelectionRows (w->choice)
#define SelectedMakerIndex (w->selected)
#define MenuSelectedCarSlot (w->buttons)
#define MenuStepDirection (w->event)
#define MenuReturnSubmodeIndex (w->accepted)
#define SelectionState (w->settings)
#define BadgeRows (&w->rows[w->rowbank])
#define UiRows ((UiRow24 *)w->ui[w->uibank].bytes)
#define GlyphRows (w->slots)
#define Dialogue native_dialogue(w)
#define LoadCursor (w->current)
#define MenuArenaBase (w->resource)
'''
        reference=r'''
static void emit(int kind,s32 a,s32 b,s32 c){
 sync_dialogue();
 assert(count<8);Entry *e=&entries[count++];memset(e,0,sizeof(*e));e->kind=kind;e->args[0]=a;e->args[1]=b;e->args[2]=c;e->snapshot=*w;
 if(!mutation)return;
 if(kind==0){
  w->selected=(s16)(u16)(seed*109U);w->rowbank^=1;w->uibank^=1;w->hintbank^=1;
  w->accepted=(seed&8)?0:1;
  w->rows[w->rowbank].state10=(seed&4)?3:2;
  w->buttons^=65535;
 }
 if(kind==1){
  w->rowbank^=1;w->uibank^=1;
  for(int i=0;i<4;i++)w->choice.bytes[i]=(u8)((seed>>(i*8))^0xA5);
  w->rows[w->rowbank].state10=-1;
 }
 if(kind==2)w->resource=arena[(seed+3)%32];
 if(kind==3)w->resource=arena[(seed+7)%32];
}
void PlaySfx(s32 mode){assert(mode==0||mode==1);emit(mode,mode,0,0);}
void UpdateUnlockedCarMask(void){emit(2,0,0,0);}
void ResetScreenCounters(void){emit(3,0,0,0);}
void SetMenuFrameOptions(s32 a,s32 b,s32 c){emit(4,a,b,c);}
static void put16(u8 *p,u16 v){memcpy(p,&v,2);}
static void reference(void){
 int previous=w->choice.bytes[1];w->selected=previous;
 w->ui[w->uibank].bytes[12+previous]=0;
 int changed=0;
 if((w->buttons&0x2000) && w->selected<3){w->selected++;w->event=2;changed=1;}
 else if((w->buttons&0x8000) && w->selected>0){w->selected--;w->event=1;changed=1;}
 else w->event=0;
 if(changed){
  emit(0,0,0,0);
  if(w->selected<0)w->selected=0;else if(w->selected>=4)w->selected=3;
  w->highlight[w->hintbank][4]=(u8)(w->selected+34);
  w->choice.bytes[1]=(u8)w->selected;
  ((u8*)&w->slots[w->selected])[2]=250;((u8*)&w->slots[w->selected])[0]=0;
  w->ui[w->uibank].bytes[22]=(u8)(w->selected+4);
 }
 w->ui[w->uibank].bytes[12+w->selected]=2;
 if(w->accepted && w->rows[w->rowbank].state10==3){
  int captured=3;emit(1,1,0,0);
  for(int i=0;i<4;i++){u8 copy[4];memcpy(copy,w->choice.bytes,4);memcpy((u8*)&w->settings+28+i*4,copy,4);}
  w->rows[w->rowbank].state10=4;
  w->ui[w->uibank].bytes[20]=(u8)captured;
  put16((u8*)&w->settings+26,2);
  ((u8*)&w->settings)[29]=w->choice.bytes[1];
  emit(2,0,0,0);emit(3,0,0,0);w->current=w->resource;emit(4,0,1,0);
 }
}
static uint32_t state=0x71C81925;
static uint32_t next(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
static void bind(World *owner){w=owner;count=0;memset(entries,0,sizeof(entries));}
int main(void){
 assert(sizeof(void*)>4 && (uintptr_t)arena>UINT32_MAX);
 for(unsigned k=0;k<131072;k++){
  seed=k*2654435761U;mutation=k>=65536;
  for(size_t i=0;i<sizeof(actual);i++)((u8*)&actual)[i]=(u8)next();
  actual.rowbank=actual.uibank=actual.hintbank=0;
  actual.choice.bytes[1]=(u8)(k*17U);actual.buttons=(u16)k;actual.accepted=k&4;
  actual.rows[0].state10=k%5;actual.rows[1].state10=3;
  actual.resource=arena[k%32];actual.current=arena[(k+1)%32];expected=actual;reset_dialogue();
  bind(&actual);StepResultsMenuChoice();sync_dialogue();verify_dialogue();int actual_count=count;memcpy(actual_entries,entries,sizeof(entries));
  bind(&expected);reference();
  if(count!=actual_count || memcmp(entries,actual_entries,sizeof(entries)) || memcmp(&actual,&expected,sizeof(actual))){fprintf(stderr,"case%u mutation%d failed actual calls%d expected%d\n",k,mutation,actual_count,count);return 1;}
 }
 puts("131072 native choice cases: OK (UBSan)");return 0;
}
'''
        with TemporaryDirectory() as temporary:
            p=Path(temporary)
            source=p/'choice.c'
            source.write_text(transport_selection(transport(prefix+s+reference,s,'StepResultsMenuChoice','&actual.slots[0]','4',[('emit', 'w==&actual')]),s,'StepResultsMenuChoice','&actual.choice',4,[('emit','w==&actual')]))
            executable=p/'choice'
            subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined',
                            '-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                            str(source),'-o',str(executable)],check=True,capture_output=True,text=True)
            result=subprocess.run([str(executable)],check=True,capture_output=True,text=True)
            self.assertIn('131072 native choice cases: OK (UBSan)',result.stdout)


if __name__=='__main__':
    unittest.main()
