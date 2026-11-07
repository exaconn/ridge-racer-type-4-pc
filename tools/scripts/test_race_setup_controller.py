"""All signed16 race modes, mutable services and complete shared setup owners."""
from pathlib import Path
from tempfile import TemporaryDirectory
import re,subprocess,sys,unittest
ROOT=Path(__file__).resolve().parents[2]
target=ROOT/'tools/scripts/fixtures/views/func_8003CAF4.c'
class MainControllerTest(unittest.TestCase):
 def test_all_modes_and_mutable_services(self):
  code=target.read_text();_,body=code.split('void SetupRace(void)',1)
  prefix=(ROOT/'include/rr/race_setup.h').read_text()+'\n'+'\n'.join(l for l in (ROOT/'include/rr/standings.h').read_text().splitlines() if l.startswith('extern '))
  body='void SetupRace(void)'+body
  body=re.sub(r' __asm__\("\$[0-9]+"\)','',body)
  declarations=re.findall(r'^extern ([^;]+);',prefix,re.M)
  declarations=[d for d in declarations if any(n in body for n in re.findall(r'\b(?:D_[A-Fa-f0-9]+|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)\b',d))]
  gname=lambda n:'g_'+(n[2:] if n.startswith('D_') else '%08X'%__import__('c_functions').address_of(n))
  fields='\n'.join(re.sub(r'\b(?:D_[A-Fa-f0-9]+|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)\b',lambda m:gname(m[0]),d)+';' for d in declarations)
  names=[name for d in declarations for name in re.findall(r'\b(?:D_[A-Fa-f0-9]+|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)\b',d)]
  aliases='\n'.join('#define '+n+' (w->s.'+gname(n)+')' for n in names)
  mutations=[]
  for d in declarations:
   typ,decl=d.split(' ',1)
   for v in decl.split(','):
    v=v.strip();name=re.match(r'(?:D_[A-Fa-f0-9]+|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)',v)[0];bound=re.findall(r'\[(\d+)\]',v)
    if typ=='RrRaceVehicle':mutations.append('for(i=0;i<2;i++)'+name+'[i].flags1E4=(next()%4)?(u16)next():0;')
    elif typ=='RrRaceSetupPair4':mutations.append(name+'.first=(s16)next();'+name+'.second=(s16)next();')
    elif bound:mutations.append('for(i=0;i<'+bound[0]+';i++)'+name+'[i]=('+typ+')next();')
    else:mutations.append(name+'=('+typ+')next();')
  source=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
'''+prefix+r'''
typedef struct {u32 guard[4],rng;
'''+fields+r'''
u32 tail[4];} State;
typedef struct {s32 kind,args[4];State snapshot;} Event;
typedef struct {State s;Event events[24];unsigned count;} World;
static World actual,reference,*w;
static State initial;
static unsigned scenario,mutation,seen[32],modeSeen[65536],courseSeen[8],groupSeen[2],budgetSeen[9],callbackSeen[18];
static const s16 modes[10]={0,1,2,3,4,5,6,-1,INT16_MIN,INT16_MAX};
static const s32 budgets[9]={INT32_MIN,INT32_MAX,-3,-2,-1,0,1,2,3};
'''+aliases+r'''
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
static s16 half(u16 n){s16 v;memcpy(&v,&n,2);return v;}
static u32 next(void){u32 n=w->s.rng;n^=n<<13;n^=n>>17;n^=n<<5;return w->s.rng=n;}
static void fill(void){unsigned i;
'''+ '\n'.join(mutations)+r'''
 D_800F4D44=modes[(scenario+w->count)%10];D_800F3914=next()%8;ReverseCourse=next()%2;D_80107178=next()%4;
 TrackLength=budgets[((scenario>>4)+w->count)%9];
}
static Event *event(s32 kind){Event *e;assert(w->count<24);e=&w->events[w->count++];memset(e,0,sizeof(*e));e->kind=kind;e->snapshot=w->s;callbackSeen[kind]++;return e;}
static void change(void){if(mutation)fill();}
s32 func_80114988(void){event(1);change();return -123;}
void func_80114AF8(unsigned long n){assert(n==0x7E900UL);event(2)->args[0]=(s32)n;change();}
void DrawSync(s32 n){event(3)->args[0]=n;change();}
void func_8004A678(void){event(4);change();}
void InitRaceCars(s32 n){event(5)->args[0]=n;change();}
void PadRumbleStart(s32 n){assert(n==0||n==1);event(6)->args[0]=n;change();}
void func_8006E794(void){event(7);change();}
void SetupRenderer(s32 n){event(8)->args[0]=n;change();}
void func_80045830(void){event(9);change();}
void func_80070440(void){event(10);change();}
void BuildCameraMatrices(void){event(11);change();}
void InitRaceHudPackets(s32 n){event(12)->args[0]=n;change();}
void InitHudNumberTargets(s32 n){event(13)->args[0]=n;change();}
void ClearTrackObjectStates(s32 n){event(14)->args[0]=n;change();}
void func_8004F7E0(void){event(15);change();}
void func_80059168(void){event(16);change();}
void SetModelTexturePage(s32 n){event(17)->args[0]=n;change();}
'''+body+r'''
static void expected(void){unsigned i,index,course,group;int enabled;int64_t third;static const u8 special[8]={0,0,1,1,1,0,0,1};
 if(D_800F4D44==4){seen[0]++;func_80114988();func_80114AF8(0x7E900UL);}else seen[1]++;
 DrawSync(0);func_8004A678();InitRaceCars(D_800F4D44);
 enabled=D_800F4D44<2||D_800F4D44==6||(D_800F4D44==4&&D_80107178==2);D_800FF6F8=enabled;seen[2+enabled]++;
 if(D_800F4D44==4)seen[20+(D_80107178==2)]++;
 if(D_800F4D44!=5){seen[4]++;for(i=0;i<2;i++){
  if(PlayerCars[i].flags1E4){seen[6+i]++;PadRumbleStart(i);PlayerCars[i].flags1E4^=256;}
  else seen[8+i]++;
 }}else seen[5]++;
 func_8006E794();SetupRenderer(5);
 D_8010C13C=0;D_8010C12C=2;D_8010C120[0]=D_8010C120[1]=0;D_8010C120[2]=-1;
 third=(int64_t)TrackLength/3;D_8010C140[0]=(s32)third;D_8010C140[1]=(s32)(2*third);D_8010C140[2]=TrackLength;
 for(i=0;i<9;i++)if(TrackLength==budgets[i])budgetSeen[i]++;
 course=(u16)D_800F3914;group=(u16)ReverseCourse;assert(course<8&&group<2);courseSeen[course]++;groupSeen[group]++;
 index=(group*8+course)*3;for(i=0;i<3;i++)D_8010C0F8[i]=D_800F3838[index+i];RaceState=SavedRaceState;
 if(D_800F4D44==0){BestLapTime=CourseBestLap[group*8+course];seen[10]++;}
 else if(D_800F4D44==1){BestLapTime=D_800F3B00[course];seen[11]++;}else seen[12]++;
 D_8010C134=-1;
 if(D_800F4D44<2||D_800F4D44==6){NumLaps=CourseLapCounts[course+8*(D_800F4D44==1)];seen[13]++;}else seen[14]++;
 for(i=0;i<10;i++){LapFrameCounts[i]=0;LapTimes[i]=-1;}TotalRaceTime=-1;
 D_800ABEF0=special[course];func_80045830();
 if(D_800F4D44==0||D_800F4D44==5){D_800AD50C=0;D_800AC538=0;D_800ACBF8=0;seen[15]++;}
 else{func_80070440();D_800AD50C=1;D_800AC538=0;D_800ACBF8=1;seen[16]++;}
 BuildCameraMatrices();InitRaceHudPackets(D_800F4D44);InitHudNumberTargets(D_800F4D44);
 ScreenFrame=0;ModeFrame=0;D_800AC5D0=0;RacePaused=0;PauseCursor=0;D_80107190=128;
 enabled=D_800F4D44==1||D_800F4D44==6;D_800AC5A4=enabled?15000:45000;seen[17+enabled]++;VSyncThreshold=384;
 ClearTrackObjectStates(0);D_800F4C70.first=D_800F4C70.second=0;D_800F2AB4.first=D_800F2AB4.second=0;RaceVoiceFlags=0;
 func_8004F7E0();func_80059168();SetModelTexturePage(10);
}
int main(void){unsigned i;uint64_t cases=0;_Static_assert(sizeof(RrRaceVehicle)==0x320,"whole vehicle");
 assert((uintptr_t)&actual.s>UINT32_MAX);
 for(scenario=0;scenario<65536;scenario++)for(mutation=0;mutation<2;mutation++){
  memset(&actual.s,0,sizeof(actual.s));actual.count=0;w=&actual;w->s.rng=0x3CAF4U+scenario*73U;fill();
  D_800F4D44=half((u16)scenario);D_800F3914=scenario%8;ReverseCourse=(scenario>>3)&1;D_80107178=scenario%3;
  PlayerCars[0].flags1E4=(u16)scenario;PlayerCars[1].flags1E4=(u16)(scenario^65535U);
  for(i=0;i<4;i++){w->s.guard[i]=next();w->s.tail[i]=next();}
  initial=w->s;modeSeen[(u16)D_800F4D44]++;reference.s=initial;reference.count=0;
  w=&actual;SetupRace();w=&reference;expected();
  assert(!memcmp(&actual.s,&reference.s,sizeof(State)));assert(actual.count==reference.count);assert(!memcmp(actual.events,reference.events,actual.count*sizeof(Event)));cases++;
 }
 assert(cases==131072);for(i=0;i<19;i++)assert(seen[i]);assert(seen[20]&&seen[21]);for(i=1;i<18;i++)assert(callbackSeen[i]);
 for(i=0;i<65536;i++)assert(modeSeen[i]);for(i=0;i<8;i++)assert(courseSeen[i]);for(i=0;i<2;i++)assert(groupSeen[i]);for(i=0;i<9;i++)assert(budgetSeen[i]);return 0;
}
'''
  with TemporaryDirectory() as tmp:
   p=Path(tmp);(p/'test.c').write_text(source)
   r=subprocess.run(['cc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(r.returncode,0,r.stderr)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
