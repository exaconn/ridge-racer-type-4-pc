"""Overlay659 results controller: original/compiled MIPS vs guarded native owners.

Services are stateful mocks. All argument lists and complete owner snapshots
are checked before each service. RR_COMPILED_ELF substitutes rebuilt MIPS.
"""
from overlay_units import read_c_source, symbol_addresses, resolve_c_source_path
SYMBOL_ADDRESSES = symbol_addresses()
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2];os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR','build/tests/overlay659_results_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/659/func_80115224.c')
body=read_c_source(source);body=body[body.index('void ovl659_update_results'):]
global_names=sorted(set(re.findall(r'\b(?:(?:D_[0-9A-F]{8}|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|CD_read_dma_mode|CdFileSyncResult|MenuTextRowCount|ModelTexturePage|MotionController|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|HwCardEvTimeout|MenuGridRecords|RubberBandCurve|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|ModelBankTable|Rect16RowCount|SavedRaceState|SceneLoadState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|GlyphRowCount|HwCardEvError|MenuItemCount|ModeFuncTable|RaceStandings|SwCardEvError|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FrameCounter|MenuTextRows|PanGainTable|ReplayInputs|ReplayLength|VBLANK_MINUS|_snd_ev_flag|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|HwCardEvIoe|HwCardEvNew|RaceContext|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|ViewOffsetX|ViewOffsetY|ViewScreenH|_svm_damper|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_printf|LoadCursor|MenuCursor|NumPlayers|PlayerCars|Rect16Rows|SoundSlots|SoundState|TrackNodes|UiRowCount|_spu_EVdma|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|FarColorB|FarColorG|FarColorR|GlyphRows|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|LapTimes|PadPorts|SinTable|_spu_RXX|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount)|ovl659_[a-z][a-z0-9_]*)\b',body)))
global_names=[n for n in global_names if not re.search(r"\b"+n+r"\s*\(",body)]
functions=sorted(set(re.findall(r'\b(?:(?:ovl659_)?func_[0-9A-F]{8}|InterpolateObjectKeyframe|RelocateTrackLookupTables|UpdateCarTrackProjection|ClearScreenOverlayState|GetSmoothedTrackHeading|SpuSetTransferStartAddr|UpdateComCarTargetPoint|ClearTrackObjectStates|FindTrackNodeUnbounded|FormatMillisecondsTime|LookupTrackEventTable0|LookupTrackEventTable1|LookupTrackEventTable2|LookupTrackEventTable3|SpuClearReverbWorkArea|SpuIsTransferCompleted|UpdateComCarLaneChange|ValidateRecordPassword|_SsVabOpenHeadWithMode|AddSpriteWithDrawMode|CdAudioFindStreamFile|GetTrackTargetHeading|LookupTrackEventPair2|ProjectToTrackSegment|SpuSetReverbModeParam|SsUtSetReverbFeedback|_SpuIsInAllocateArea_|_SsVmSelectToneAndVag|AllocMenuGridRecords|BuildRotMatrixAngles|CalcDistanceToPlayer|CalcRubberBandFactor|CdAudioReadyCallback|ComputeQuadMidpoints|ComputeStartDistance|DecodeRecordPassword|EncodeRecordPassword|EnterCriticalSection|FindTrackNodeScratch|FramesToMilliseconds|LookupTrackEventPair|PlanComCarCornerTurn|RelocateRelativeFile|ResolveCarCollisions|SetVoiceVolumeScaled|SoundSetMasterVolume|TransformCourseModel|UpdateComCarOvertake|_SpuIsInAllocateArea|ApplyRubberBandSpec|BuildCameraMatrices|CD_set_test_parmnum|CdAudioRequestPause|CdAudioSetVolumeRaw|ComputeWheelOffsets|ConstrainCarToTrack|CopyStringClearTail|CopyStringPadSpaces|DrawLoadingTriangle|EmitSemiTransG3List|EmitSemiTransG4List|ExitCriticalSection|FindNearestCarAhead|FindTrackEventIndex|FindWheelTrackNodes|FormatDecimalDigits|FormatFrameTimeMMSS|GetTrackNodeHeading|InitCdFileDirectory|InterpTrackPosition|RelocateOffsetTable|ResetScreenCounters|ReturnFromException|SetModelTexturePage|SetTrackLaneOffsets|SoundMasterFadeStep|SsVabOpenHeadSticky|SsVabTransCompleted|SteerCarTowardTrack|UnrelocateModelBank|UpdateRaceStandings|_SsSetControlChange|_SsSetNrpnVabAttr10|_SsSetNrpnVabAttr11|_SsSetNrpnVabAttr12|_SsSetNrpnVabAttr13|_SsSetNrpnVabAttr14|_SsSetNrpnVabAttr15|_SsSetNrpnVabAttr16|_SsSetNrpnVabAttr17|_SsSetNrpnVabAttr18|_SsSetNrpnVabAttr19|_SsSetProgramChange|_copy_memcard_patch|data_ready_callback|CardSystemShutdown|CdAudioSelectTrack|CheckFinalLapEvent|EncodePasswordCode|FormatCountOfTotal|GameQueueTileTrans|GetTrackPointAhead|IntegrateCarMotion|PadSetRumbleParams|SetReverbDepthBoth|SpuSetTransferMode|SsSetReservedVoice|SsUtSetReverbDelay|SsUtSetReverbDepth|SubmitCourseModel2|UpdateBadgeRowAnim|UpdateCameraMotion|UpdateCarTrackNode|UpdateTrackObjects|_SsSetNrpnVabAttr0|_SsSetNrpnVabAttr1|_SsSetNrpnVabAttr2|_SsSetNrpnVabAttr3|_SsSetNrpnVabAttr4|_SsSetNrpnVabAttr5|_SsSetNrpnVabAttr6|_SsSetNrpnVabAttr7|_SsSetNrpnVabAttr8|_SsSetNrpnVabAttr9|_padRecvAtLoadInfo|_padSendAtLoadInfo|_spu_getInTransfer|_spu_setInTransfer|_spu_setReverbAttr|AddOrSubtractByte|BuildObjectMatrix|CdAudioFilterStep|CdAudioResumeStep|CdFileBytesLoaded|ClearHwCardEvents|ClearSwCardEvents|CountChoiceGlyphs|DecodeCourseIndex|EmitTexturedPanel|GetTrackHeadingAt|InterpCurveLinear|InterruptCallback|LoadTrackResource|PadSetRumbleFlags|PlayTrackEventSfx|RegisterModelBank|SetCarImpactState|SetIdentityMatrix|SetTrackStartNode|SetTrackWorkspace|SetupViewLighting|SoundQueueEnqueue|SpawnParticlePair|SpuSetReverbVoice|SsSetMarkCallback|SsUtGetReverbType|SsUtSetReverbType|SubmitCourseModel|UnpackChoiceGlyph|UpdateCarSteering|UpdateComCarDrive|UpdateComCarSpeed|UpdateMotionTrack|UpdateRaceRanking|UpdateRumbleStrip|UpdateScrollTrack|_SsContBankChange|_SsContExpression|_SsReadDeltaValue|_padSetVsyncParam|_remove_ChgclrPAD|BuildLightMatrix|CdAudioBeginPlay|CdAudioPauseStep|CdAudioSetFilter|CdAudioSetTarget|CdAudioSetVolume|DrawSyncCallback|DrawTrackObjects|EmitPresetSprite|FindCurveSegment|FindFreeParticle|InitMenuTextRows|InitViewLighting|LoadTrackObjects|LookupTrackEvent|PadRangeCheckSet|PollHwCardEvents|RequestSceneLoad|SaveCarSnapshots|SetTrackNodeData|ShuffleSlotOrder|SoundQueueUpdate|SplashLogoUpdate|SpuSetCommonAttr|SpuSetNoiseClock|SpuSetNoiseVoice|SubmitModelFaces|TriggerWheelBump|UpdateCarHeading|UpdateScreenFade|UpdateWheelBumps|_SpuDataCallback|_SsContDataEntry|_SsUtResolveADSR|init_ring_status|AddDrawAreaPrim|AddDrawModePrim|AllocRect16Rows|AngleDiffSigned|BuildRotMatrixX|BuildRotMatrixY|BuildRotMatrixZ|ByteToCodeChars|CarLocalToWorld|CardWriteFileAt|CdAudioFadeStep|CdAudioLoopStep|CdAudioPlayStep|CdAudioStopStep|CdFileReadRange|CdFileReadStart|CdReadyCallback|ChangeClearRCnt|CheckRaceFinish|CodeCharsToByte|GameQueueSprite|InterpCurveEase|LoadR4Directory|NormalColorCol3|NormalColorDpq3|PackChoiceGlyph|PadUpdateRumble|ReadInitPadFlag|RestartCallback|SelectModelBank|SetCarSpecTable|Snd_SetPlayMode|SoundQueueClear|SoundSystemInit|SpuGetKeyStatus|SpuSetVoiceAttr|SsPitchFromNote|SsSeqCalledTbyT|SsSetSerialAttr|SsUtChangePitch|TitleMenuUpdate|UpdateEngineRpm|UpdateLapTiming|UpdatePlayerCar|UpdateSoundSlot|WaitSwCardEvent|_SsContExternal|_SsContResetAll|_SsInitSoundSeq|_SsSetPitchBend|_SsVmDoAllocate|_SsVmGetProgPan|_SsVmGetProgVol|_SsVmGetSeqLVol|_SsVmGetSeqRVol|_SsVmSetProgPan|_SsVmSetProgVol|_padCmdParaMode|_padInitSioMode|_padLoadActInfo|_padSetActAlign|_padSetMainMode|_spu_FsetDelayR|_spu_FsetDelayW|_spu_FwriteByIO|_spu_note2pitch|_spu_pitch2note|AddTextSprites|AddTileGrid2x2|AllocBadgeRows|AllocGlyphRows|AverageCarSpec|BuildRateCurve|CalcSteerLimit|CardFreeBlocks|CardReadFileAt|CardSystemInit|CdDataCallback|CdReadCallback|CdSyncCallback|CountCardFiles|DecayCarOffset|EmitFadeSprite|EmitPolyFT4Fog|EmitPolyFT4Raw|EmitSpriteGrid|FormatRaceTime|GetHwCardEvent|GetR4FileIndex|InitMenuArenas|NormalColorCol|NormalColorDpq|OuterProduct12|PadRumbleStart|PadSetActAlign|PadSetMainMode|PointInQuadPtr|RemovePatchPad|ScanCurveAhead|SetColorMatrix|SetInitPadFlag|SetLightMatrix|SetMenuTextRow|SetTransMatrix|SpuSetAnyVoice|SsSetSerialVol|SsSetTableSize|SsUtGetProgAtr|SsVabTransBody|StepCarPhysics|TitleModeEnter|TransformPoint|UpdateCarShake|VSyncCallbacks|_SsContMainVol|_SsUtBuildADSR|_SsVmDamperOff|_SsVmGetSeqVol|_SsVmKeyOffNow|_SsVmPitchBend|_SsVmSeqKeyOff|_SsVmSetSeqVol|_padGetActSize|_padInitDirSeq|startIntrVSync|ApplyMatrixLV|ApplyMatrixSV|CD_getsector2|CardWriteFile|CdAudioUpdate|CdGetDiskType|CheckCallback|ChecksumBytes|DpqColorLight|DrawHudSprite|EmitPolyF4Fog|EmitTiledRect|EvalRateCurve|FindTrackNode|GetGraphDebug|InitParticles|InitPlayerCar|InitRect16Row|MulRotMatrix0|OuterProduct0|PadInitDirect|PadRumbleStop|PlaySfxVolPan|ResetCallback|ResetCarState|ResetEntryInt|SceneLoadStep|SetDefDispEnv|SetDefDrawEnv|SetDrawOffset|SetFogNearFar|SetGeomOffset|SetGeomScreen|SetGraphDebug|SetGraphQueue|SetupRenderer|SetupViewport|SlotKeyOffAll|SpuInitMalloc|SsSetTickMode|SsUtAllKeyOff|SsUtGetVagAtr|SsUtReverbOff|SsUtSetVagAtr|SsVabFakeHead|StCdInterrupt|StepMenuValue|StepRaceFrame|TitleMenuInit|UpdateComCars|VSyncCallback|_SsContDamper|_SsContPanpot|_SsGetSeqData|_SsSndNextSep|_SsVmDamperOn|_SsVmKeyOnNow|_SsVmSeKeyOff|_dbl_shift_us|_spu_FgetRXXa|_spu_FsetRXXa|trapIntrVSync|AngleDiffAbs|BlendCarSpec|CD_getsector|CardReadFile|CdFileFinish|CdGetSector2|CdSearchFile|DS_cachefile|DS_searchdir|DeliverEvent|DrawBadgeRow|DrawHudDigit|GetVideoMode|HookEntryInt|InitBadgeRow|InitGlyphRow|InitMenuGrid|InitRaceCars|NormalColor3|PadEnableCom|QueueDrawStp|ReadCarInput|RotMatrixYXZ|RotMatrixZYX|RotTransPers|SetBackColor|SetCarBounds|SetCdMixMode|SetRotMatrix|SetSemiTrans|SetTexWindow|SetVertexTri|SetVideoMode|SlotSetPause|SoundStopAll|SpuSetReverb|SsUtReverbOn|StGetBackloc|StopCallback|SubmitModel2|VSync_memclr|_SsContNrpn1|_SsContNrpn2|_SsSndReplay|_SsSndSetVol|_SsVmPBVoice|_SsVmSeKeyOn|_dirFailAuto|_padChkVsync|_padStartCom|_patch_card2|_sio_control|_spu_FsetPCR|_spu_FsetRXX|setIntrVSync|startIntrDMA|AddTilePrim|AllocUiRows|ApplyMatrix|CD_datasync|CD_initintr|CardHasFile|CdAudioFade|CdAudioInit|CdAudioMute|CdDiskReady|CdGetSector|ClearImage2|DMACallback|DS_newmedia|DrawOTagEnv|EmitPolyFT3|EnableEvent|GetIntrMask|InitComCars|LerpAngle12|NormalColor|PadChkVsync|PadGetState|PadInfoComb|PadInfoMode|PadPollPort|PadStartCom|PointInQuad|ReverbReset|RotAverage3|RotAverage4|ScaleMatrix|SetDispMask|SetDrawArea|SetDrawMode|SetDrawMove|SetFarColor|SetIntrMask|SetPriority|SetSXSYfifo|SetShadeTex|SoundResume|SoundUpdate|SquareRoot0|SsSepSetVol|SsSeqGetVol|SsSeqSetVol|SsSetStereo|SsUtKeyOffV|SsUtSetVVol|StClearRing|StSetStream|StUnSetRing|StoreImage2|SysDeqIntRP|SysEnqIntRP|YawToOctant|_SsContRpn1|_SsContRpn2|_SsSndPause|_SsSndTempo|_SsVmKeyOff|_SsVmSetVol|_SsVmVSetUp|__floatsidf|_add_mant_d|_card_clear|_mul_mant_d|_padStopCom|_patch_card|dma_execute|note2pitch2|restartIntr|trapIntrDMA|Atan2Table|CD_initvol|CardUpdate|CdControlB|CdControlF|CdDataSync|CdFileStep|CdIntToPos|CdPosToInt|CdReadMode|CdReadSync|CdSetDebug|ClearImage|ClearOTagR|CloseEvent|DMA_memclr|DisablePAD|EmitPolyF4|FlushCache|GPU_memset|GetDispEnv|GetDrawEnv|KeyOnCheck|LightColor|LoadImage2|LocalLight|MoveImage2|MulMatrix0|MulMatrix2|NormalClip|PadChkMtap|PadInfoAct|PadStopCom|PutDispEnv|PutDrawEnv|ResetGraph|RotMatrixX|RotMatrixY|RotMatrixZ|SetDrawEnv|SetDrawStp|SetFogNear|SetRGBfifo|SetSZfifo3|SetSZfifo4|SetVertex0|SetVertex1|SetVertex2|SoundPause|SoundReset|SsSepClose|SsSeqClose|SsUtKeyOff|SsUtKeyOnV|SsVabClose|StFreeRing|StoreImage|SystemInit|TriggerSfx|XorBytes37|_SsSeqPlay|_SsSndPlay|_SsSndStop|_SsVmAlloc|_SsVmFlush|_SsVmKeyOn|_comp_mant|_dbl_shift|_padSetAct|_padSetCmd|_patch_gte|_patch_pad|_spu_FiDMA|_spu_Fw1ts|_spu_gcSPU|chkRC2wait|note2pitch|setIntrDMA|setRC2wait|vmNoiseOff|AverageZ3|AverageZ4|CdControl|CdLastCom|CdLastPos|ClearOTag|DpqColor3|DrawOTag2|DrawUiRow|EnablePAD|FileClose|FileWrite|InitUiRow|LoadClut2|LoadImage|LoadTPage|MoveImage|MulMatrix|OpenEvent|PAD_init2|PadSetAct|PadUpdate|ResetRCnt|RotMatrix|SetData32|SetMAC123|SetPolyF3|SetReverb|SetupRace|SpuSetKey|SsSepPlay|SsSepStop|SsSeqOpen|SsSeqPlay|SsSeqStop|SsSetMVol|SsSetMono|SsUtKeyOn|StGetNext|StSetMask|StSetRing|StartCARD|StartPAD2|StartRCnt|StopCARD2|TestEvent|VsGetAddr|WaitEvent|_ExitCard|_SsNoteOn|_SsVmInit|__fixdfsi|_err_math|_padSioRW|_spu_init|checkRECT|firstfile|get_alarm|set_alarm|startIntr|AddPrims|AlignUp4|CD_flush|CD_ready|CdComstr|CdIntstr|CdStatus|ColorCol|ColorDpq|DpqColor|DrawCars|DrawOTag|DrawPrim|DrawRing|DrawSync|FileOpen|FileRead|FileSeek|GetTPage|HsvToRgb|InitCARD|InitGeom|InitHeap|InitPAD2|LoadClut|MainLoop|PAD_init|RotTrans|SetIR123|SpuStart|SpuWrite|Square12|SsStart2|StartPAD|StopCARD|StopPAD2|StopRCnt|_SpuInit|_SsClose|_SsStart|__adddf3|__divdf3|__muldf3|__subdf3|_addque2|_mainasu|_spu_Fr_|get_mode|stopIntr|trapIntr|AddPrim|CD_init|CD_sync|CatPrim|CdFlush|CdRead2|CdReady|CdReset|GetClut|GetRCnt|InitPAD|LoadVab|PlaySfx|SetRCnt|SpuFree|SpuInit|Square0|SsStart|StopPAD|_addque|_exeque|_getctl|_spu_Fr|_spu_Fw|_status|get_ofs|getintr|mem2mem|setIntr|AddSIO|CD_vol|CdInit|CdMode|CdRead|CdSync|DelSIO|GetODE|SetDQA|SetDQB|SetIR0|SetMem|SetRii|_param|_reset|_spu_t|get_ce|get_cs|get_tw|memclr|ratan2|v_wait|CD_cw|CdMix|Intpl|VSync|_sync|sin_1|_clr|_ctl|_cwb|_cwc|_drs|_dws|_otc|rcos|rsin|Lzc)(?=\()',body))-{'ovl659_update_results'})
head='''#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==(n),"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==(n),"PAL offset")
#include "rr/frame_storage_types.h"
#include "rr/overlay659_results_controller.h"
'''
(P/'header.c').write_text(head)
pre=subprocess.check_output(['cc','-E','-Iinclude',str(P/'header.c')],text=True)
protos={};code=head+'\n'
for name in global_names:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
code+='''
static struct {u32 guard[4];RrFrameStorage data[2];u32 tail[4];} frames;
static struct {u32 guard[4];RrMenuTextRow60 data[2][44];u32 tail[4];} texts;
_Static_assert(sizeof(RrFrameStorage)==0x22778,"complete frame owner");
_Static_assert(sizeof(RrMenuTextRow60)==60,"real row stride");
_Static_assert(offsetof(RrMenuTextRow60,state.d34)==0x34,"row state halfword");
_Static_assert(sizeof(RrRaceVehicle)==0x320,"complete vehicle");
_Static_assert(offsetof(RrRaceVehicle,steering288.finish20)==0x2A8,"best-lap field");
_Static_assert(sizeof(RrMenuSettings2C)==0x2C,"complete settings");
_Static_assert(offsetof(RrMenuSettings2C,retries08)==8,"retry bank");
typedef uintptr_t (*Callback)(unsigned,uintptr_t*);
static Callback cb;void set_callback(Callback c){cb=c;}
'''
for i,name in enumerate(functions):
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1].removeprefix('extern ');params=m[2];types=[] if params.strip() in ['', 'void'] else [p.strip() for p in params.split(',')]
 clean=[]
 for t in types:
  match=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if match:t=match[1]
  elif '*' not in t and len(t.split())>1:t=t.rsplit(' ',1)[0]
  clean.append(t)
 types=clean
 protos[name]=(ret,types);args=','.join('(uintptr_t)a'+str(j) for j in range(len(types))) or '0'
 decl=','.join(t+' a'+str(j) for j,t in enumerate(types)) or 'void'
 code+=f'{ret} {name}({decl}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\nvoid *object(unsigned i){switch(i){\n'
for i,name in enumerate(global_names):code+=f'case {i}:return &n_{name};\n'
G=len(global_names)
code+=f'case {G}:return &frames;case {G+1}:return &texts;default:abort();}}}}\n'
code+='size_t object_size(unsigned i){switch(i){\n'
for i,name in enumerate(global_names):code+=f'case {i}:return sizeof(n_{name});\n'
code+=f'case {G}:return sizeof(frames);case {G+1}:return sizeof(texts);default:abort();}}}}\n'
code+='void roots(unsigned f,unsigned t){D_800ACC14=(void*)&frames.data[f];MenuTextRows=texts.data[t];}\n'
native=P/'native.c';native.write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Iinclude','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(native),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
N={n:(lib.object(i),lib.object_size(i)) for i,n in enumerate(global_names)}
frame=lib.object(G);frame_size=lib.object_size(G);text=lib.object(G+1);text_size=lib.object_size(G+1)
assert frame>0xffffffff and text>0xffffffff and frame_size==32+2*0x22778 and text_size==32+2*44*60
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+659*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  elf=ELFFile(f)
  for secname in ['.text','.rodata']:
   sec=elf.get_section_by_name(secname)
   if sec:u.mem_write(sec['sh_addr']&0x1fffffff,sec.data())
read=lambda a,n:bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
addr=lambda n:SYMBOL_ADDRESSES[n] if n in SYMBOL_ADDRESSES else __import__('c_functions').address_of(n)
regions=[(n,p,addr(n),s) for n,(p,s) in N.items() if n not in ['D_800ACC14','MenuTextRows']]
regions += [('frames',frame,0x800ADAE0,frame_size),('texts',text,0x8018FFF0,text_size)]
mappings=[(p,a,s) for n,p,a,s in regions]
def palpointer(value):
 for p,a,s in mappings:
  if p<=value<p+s:return a+value-p
 raise AssertionError(('unowned pointer',hex(value)))
def scalar(name,value,native):
 p,size=N[name];data=(value&((1<<(8*size))-1)).to_bytes(size,'little')
 if native:C.memmove(p,data,size)
 else:write(addr(name),data)
def roots(f,t,native):
 if native:lib.roots(f,t)
 else:
  write(0x800ACC14,struct.pack('<I',0x800ADAF0+f*0x22778));write(0x800FFC14,struct.pack('<I',0x80190000+t*44*60))
def textbyte(bank,row,off,value,native):
 offset=16+bank*44*60+row*60+off;data=bytes([value&255])
 if native:C.memmove(text+offset,data,1)
 else:write(0x8018FFF0+offset,data)
def snapshot(native):
 parts=[C.string_at(p,s) if native else read(a,s) for n,p,a,s in regions]
 for name in ['D_800ACC14','MenuTextRows']:
  value=palpointer(C.c_void_p.from_address(N[name][0]).value) if native else struct.unpack('<I',read(addr(name),4))[0]
  parts.append(struct.pack('<I',value))
 return b''.join(parts)
phase_values=[0,3,4,5,6,7,8];signed_bytes=[-128,-1,0,1,2,3,7,8,11,12,20,21,127];timers=[-32768,-1,0,1,2,3,63,64,65,32767]
case=0;events=[];seen=0;errors=[];coverage=set();statecoverage=set();groupcoverage=set();returns=set();mutations=set()
def field(name,offset,data,native):
 if native:C.memmove(N[name][0]+offset,data,len(data))
 else:write(addr(name)+offset,data)
def configure_settings(n,serial,native):
 field('D_800AD4D8',0x17,bytes([((n//11)+serial)%8]),native)
 field('D_800AD4D8',0x1A,struct.pack('<H',[0,4,7,0x8000,0x7fff][(n+serial)%5]),native)
 field('D_800AD4D8',8,bytes(v&255 for v in ([-128,-1,0,1,2,127][(n+serial+j)%6] for j in range(8))),native)
def service(event,args,native):
 global seen
 name=functions[event];normalized=[palpointer(a) if native and '*' in t else a&0xffffffff for a,t in zip(args,protos[name][1])]
 before=snapshot(native)
 if native:
  assert seen<len(events),(case,name,'unexpected service')
  expected=events[seen];assert (event,normalized)==expected[:2],(case,seen,name,normalized,expected[:2])
  assert before==expected[2],(case,seen,name,'full owner state before service')
  serial=seen;seen+=1
 else:
  serial=len(events);events.append((event,normalized,before));coverage.add(name)
 if name=='func_80040858':groupcoverage.add(normalized[0]//33)
 if name in ['FormatDecimalDigits','FormatRaceTime']:
  data=bytes([(case+serial)&255,(case*17+serial)&255])
  if native:C.memmove(args[0],data,2)
  else:write(args[0],data)
 if case&64:
  value=(case*0x9e3779b9+event*97+serial*31)&0xffffffff
  changes={'D_800F2DE4':value,'D_800AC5D0':timers[(case+serial)%len(timers)],'D_800F3A60':(case+serial)%3-1,
   'MenuCursor':[-32768,-1,0,1,32767][(case+serial)%5], 'MenuItemCount':value,
   'D_800F4D8A':[0,3,4,5,6,7,8,65535][(case//7+serial)%8],'D_800F4C6A':value,'D_800F3A26':value>>8,'D_800F4BE8':value>>16,
   'D_800F3914':[0,7,8,-32768,32767][(case+serial)%5], 'NumLaps':[-1,0,1,2,3,0x7fffffff][(case+serial)%6],
   'ovl659_preview_timer':[-2,-1,0,1,64,0x7fffffff,0x80000000][(case//7+serial)%7]}
  for n,v in changes.items():scalar(n,v,native)
  configure_settings(case,serial,native)
  field('PlayerCars',0x2A8,struct.pack('<H',[-32768,-1,0,1,2,3,4,5,32767][(case+serial)%9]&65535),native)
  for bank in range(2):
   for row,off in [(0,2),(3,3),(38,3)]:textbyte(bank,row,off,signed_bytes[(case+serial+row)%len(signed_bytes)],native)
  # All ten standings slots are live; only entries0..5 are consumed here.
  field('LapTimes',0,struct.pack('<10I',*[(value+i*1234567)&0xffffffff for i in range(10)]),native)
  mutations.add(name)
 if case&128:roots((case+serial)&1,((case//2)+serial)&1,native)
 result=0
 if name=='rsin':result=[0xffffffff,0,1,0x80000000,0x7fffffff][(case//7)%5]
 if name=='func_8003B12C':result=[0,1,2,0xffffffff,0x10001,0x80000000][(case//7)%6]
 if name in ['rsin','func_8003B12C']:returns.add((name,result))
 return result
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:errors.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
argregs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
volatile_regs=[UC_MIPS_REG_V1]+argregs+[globals()[f'UC_MIPS_REG_T{i}'] for i in range(10)]
service_map={__import__('c_functions').address_of(n):i for i,n in enumerate(functions)}
def hook(uc,address,size,data):
 event=service_map[address];argc=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in argregs[:min(argc,4)]]
 if argc>4:args+=list(struct.unpack('<'+'I'*(argc-4),read(uc.reg_read(UC_MIPS_REG_SP)+16,4*(argc-4))))
 result=service(event,args,False);ra=uc.reg_read(UC_MIPS_REG_RA)
 for r in volatile_regs:uc.reg_write(r,0xA5A55A5A)
 uc.reg_write(UC_MIPS_REG_HI,0x11223344);uc.reg_write(UC_MIPS_REG_LO,0x55667788)
 uc.reg_write(UC_MIPS_REG_V0,result);uc.reg_write(UC_MIPS_REG_PC,ra)
for a in service_map:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
framefill=b'\xA5'*frame_size

def initialize(n,native):
 for name,p,a,size in regions:
  data=framefill if name=='frames' else bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 roots(n&1,(n//2)&1,native)
 values={'D_800F2DE4':[0x7fffffff,0x80000000,0xffffffff,0,17][n%5], 'D_800F4D8A':phase_values[n%7],
 'D_800F3A60':[0,1,-1][(n//7)%3], 'D_800AC5D0':timers[(n//7)%len(timers)],
 'MenuCursor':[-32768,-1,0,1,32767][(n//7)%5], 'MenuItemCount':[-1,0,1,2,32767][(n//35)%5],
 'D_800F3914':[0,7,8][(n//7)%3], 'NumLaps':[-1,0,1,2,3,0x7fffffff][(n//21)%6],
 'ovl659_preview_timer':[-2,-1,0,1,63,64,0x7fffffff,0x80000000][(n//7)%8]}
 for name,value in values.items():scalar(name,value,native)
 configure_settings(n,0,native)
 field('PlayerCars',0x2A8,struct.pack('<H',[-32768,-1,0,1,2,3,4,5,32767][(n//7)%9]&65535),native)
 for bank in range(2):
  for row,off in [(0,2),(3,3),(38,3)]:textbyte(bank,row,off,signed_bytes[(n//7+row+bank)%len(signed_bytes)],native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 4096
for case in range(count):
 events=[];seen=0;initialize(case,False);statecoverage.add(phase_values[case%7])
 for r in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(r,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800);u.reg_write(UC_MIPS_REG_HI,0x12345678);u.reg_write(UC_MIPS_REG_LO,0x87654321)
 u.emu_start(0x80115224,0x803FF800,count=30000);assert u.reg_read(UC_MIPS_REG_PC)==0x803FF800
 expected=snapshot(False);initialize(case,True);lib.ovl659_update_results()
 if errors:raise errors[0]
 assert seen==len(events),(case,'missing services');assert expected==snapshot(True),(case,'final full owners')
 if case and case%512==0:print(case,'cases',flush=True)
assert coverage==set(functions),(coverage,set(functions));assert mutations==set(functions),(mutations,set(functions));assert statecoverage==set(phase_values) and groupcoverage=={0,1}
print('PASS',count,('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS'),'services',len(coverage),'states',sorted(statecoverage),'groups',sorted(groupcoverage),flush=True)
if os.getenv('RR_AUDIT_JSON'):
 sha=lambda f:hashlib.sha256(resolve_c_source_path(f).read_bytes()).hexdigest()
 deps=subprocess.check_output(['cc','-MM','-Iinclude',str(native)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 audit=dict(source=str(resolve_c_source_path(source)),source_sha256=sha(source),test_sha256=sha(__file__),dependencies={f:sha(f) for f in deps},cases=count,engine='compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS',services=sorted(coverage),phases=sorted(statecoverage),groups=sorted(groupcoverage),returns=sorted(returns),mutating_services=sorted(mutations),full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
