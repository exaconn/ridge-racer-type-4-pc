"""Full-owner original/compiled MIPS against native C, overlay662 controller154C8."""
from pathlib import Path
import ctypes as C, re,struct,sys,os,subprocess,random,json,hashlib
from elftools.elf.elffile import ELFFile
from unicorn import *
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2]
source=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else ROOT/'src/overlays/PAL/662/func_801154C8.c'
count=int(sys.argv[2]) if len(sys.argv)>2 else 4096
os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR',str(ROOT/'build/tests/overlay662_course_controller'))).resolve();P.mkdir(parents=True,exist_ok=True)
body=source.read_text().split('\n',1)[1];names=sorted(set(re.findall(r'\b(?:ovl662_)?(?:D_[0-9A-F]{8}|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|CD_read_dma_mode|CdFileSyncResult|MenuTextRowCount|ModelTexturePage|MotionController|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|HwCardEvTimeout|MenuGridRecords|RubberBandCurve|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|ModelBankTable|Rect16RowCount|SavedRaceState|SceneLoadState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|GlyphRowCount|HwCardEvError|MenuItemCount|ModeFuncTable|RaceStandings|SwCardEvError|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FrameCounter|MenuTextRows|PanGainTable|ReplayInputs|ReplayLength|VBLANK_MINUS|_snd_ev_flag|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|HwCardEvIoe|HwCardEvNew|RaceContext|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|ViewOffsetX|ViewOffsetY|ViewScreenH|_svm_damper|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_printf|LoadCursor|MenuCursor|NumPlayers|PlayerCars|Rect16Rows|SoundSlots|SoundState|TrackNodes|UiRowCount|_spu_EVdma|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|FarColorB|FarColorG|FarColorR|GlyphRows|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|LapTimes|PadPorts|SinTable|_spu_RXX|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount)\b',body)))
funcs=sorted(set(re.findall(r'\b(?:(?:ovl662_)?func_[0-9A-F]{8}|InterpolateObjectKeyframe|RelocateTrackLookupTables|UpdateCarTrackProjection|ClearScreenOverlayState|GetSmoothedTrackHeading|SpuSetTransferStartAddr|UpdateComCarTargetPoint|ClearTrackObjectStates|FindTrackNodeUnbounded|FormatMillisecondsTime|LookupTrackEventTable0|LookupTrackEventTable1|LookupTrackEventTable2|LookupTrackEventTable3|SpuClearReverbWorkArea|SpuIsTransferCompleted|UpdateComCarLaneChange|ValidateRecordPassword|_SsVabOpenHeadWithMode|AddSpriteWithDrawMode|CdAudioFindStreamFile|GetTrackTargetHeading|LookupTrackEventPair2|ProjectToTrackSegment|SpuSetReverbModeParam|SsUtSetReverbFeedback|_SpuIsInAllocateArea_|_SsVmSelectToneAndVag|AllocMenuGridRecords|BuildRotMatrixAngles|CalcDistanceToPlayer|CalcRubberBandFactor|CdAudioReadyCallback|ComputeQuadMidpoints|ComputeStartDistance|DecodeRecordPassword|EncodeRecordPassword|EnterCriticalSection|FindTrackNodeScratch|FramesToMilliseconds|LookupTrackEventPair|PlanComCarCornerTurn|RelocateRelativeFile|ResolveCarCollisions|SetVoiceVolumeScaled|SoundSetMasterVolume|TransformCourseModel|UpdateComCarOvertake|_SpuIsInAllocateArea|ApplyRubberBandSpec|BuildCameraMatrices|CD_set_test_parmnum|CdAudioRequestPause|CdAudioSetVolumeRaw|ComputeWheelOffsets|ConstrainCarToTrack|CopyStringClearTail|CopyStringPadSpaces|DrawLoadingTriangle|EmitSemiTransG3List|EmitSemiTransG4List|ExitCriticalSection|FindNearestCarAhead|FindTrackEventIndex|FindWheelTrackNodes|FormatDecimalDigits|FormatFrameTimeMMSS|GetTrackNodeHeading|InitCdFileDirectory|InterpTrackPosition|RelocateOffsetTable|ResetScreenCounters|ReturnFromException|SetModelTexturePage|SetTrackLaneOffsets|SoundMasterFadeStep|SsVabOpenHeadSticky|SsVabTransCompleted|SteerCarTowardTrack|UnrelocateModelBank|UpdateRaceStandings|_SsSetControlChange|_SsSetNrpnVabAttr10|_SsSetNrpnVabAttr11|_SsSetNrpnVabAttr12|_SsSetNrpnVabAttr13|_SsSetNrpnVabAttr14|_SsSetNrpnVabAttr15|_SsSetNrpnVabAttr16|_SsSetNrpnVabAttr17|_SsSetNrpnVabAttr18|_SsSetNrpnVabAttr19|_SsSetProgramChange|_copy_memcard_patch|data_ready_callback|CardSystemShutdown|CdAudioSelectTrack|CheckFinalLapEvent|EncodePasswordCode|FormatCountOfTotal|GameQueueTileTrans|GetTrackPointAhead|IntegrateCarMotion|PadSetRumbleParams|SetReverbDepthBoth|SpuSetTransferMode|SsSetReservedVoice|SsUtSetReverbDelay|SsUtSetReverbDepth|SubmitCourseModel2|UpdateBadgeRowAnim|UpdateCameraMotion|UpdateCarTrackNode|UpdateTrackObjects|_SsSetNrpnVabAttr0|_SsSetNrpnVabAttr1|_SsSetNrpnVabAttr2|_SsSetNrpnVabAttr3|_SsSetNrpnVabAttr4|_SsSetNrpnVabAttr5|_SsSetNrpnVabAttr6|_SsSetNrpnVabAttr7|_SsSetNrpnVabAttr8|_SsSetNrpnVabAttr9|_padRecvAtLoadInfo|_padSendAtLoadInfo|_spu_getInTransfer|_spu_setInTransfer|_spu_setReverbAttr|AddOrSubtractByte|BuildObjectMatrix|CdAudioFilterStep|CdAudioResumeStep|CdFileBytesLoaded|ClearHwCardEvents|ClearSwCardEvents|CountChoiceGlyphs|DecodeCourseIndex|EmitTexturedPanel|GetTrackHeadingAt|InterpCurveLinear|InterruptCallback|LoadTrackResource|PadSetRumbleFlags|PlayTrackEventSfx|RegisterModelBank|SetCarImpactState|SetIdentityMatrix|SetTrackStartNode|SetTrackWorkspace|SetupViewLighting|SoundQueueEnqueue|SpawnParticlePair|SpuSetReverbVoice|SsSetMarkCallback|SsUtGetReverbType|SsUtSetReverbType|SubmitCourseModel|UnpackChoiceGlyph|UpdateCarSteering|UpdateComCarDrive|UpdateComCarSpeed|UpdateMotionTrack|UpdateRaceRanking|UpdateRumbleStrip|UpdateScrollTrack|_SsContBankChange|_SsContExpression|_SsReadDeltaValue|_padSetVsyncParam|_remove_ChgclrPAD|BuildLightMatrix|CdAudioBeginPlay|CdAudioPauseStep|CdAudioSetFilter|CdAudioSetTarget|CdAudioSetVolume|DrawSyncCallback|DrawTrackObjects|EmitPresetSprite|FindCurveSegment|FindFreeParticle|InitMenuTextRows|InitViewLighting|LoadTrackObjects|LookupTrackEvent|PadRangeCheckSet|PollHwCardEvents|RequestSceneLoad|SaveCarSnapshots|SetTrackNodeData|ShuffleSlotOrder|SoundQueueUpdate|SplashLogoUpdate|SpuSetCommonAttr|SpuSetNoiseClock|SpuSetNoiseVoice|SubmitModelFaces|TriggerWheelBump|UpdateCarHeading|UpdateScreenFade|UpdateWheelBumps|_SpuDataCallback|_SsContDataEntry|_SsUtResolveADSR|init_ring_status|AddDrawAreaPrim|AddDrawModePrim|AllocRect16Rows|AngleDiffSigned|BuildRotMatrixX|BuildRotMatrixY|BuildRotMatrixZ|ByteToCodeChars|CarLocalToWorld|CardWriteFileAt|CdAudioFadeStep|CdAudioLoopStep|CdAudioPlayStep|CdAudioStopStep|CdFileReadRange|CdFileReadStart|CdReadyCallback|ChangeClearRCnt|CheckRaceFinish|CodeCharsToByte|GameQueueSprite|InterpCurveEase|LoadR4Directory|NormalColorCol3|NormalColorDpq3|PackChoiceGlyph|PadUpdateRumble|ReadInitPadFlag|RestartCallback|SelectModelBank|SetCarSpecTable|Snd_SetPlayMode|SoundQueueClear|SoundSystemInit|SpuGetKeyStatus|SpuSetVoiceAttr|SsPitchFromNote|SsSeqCalledTbyT|SsSetSerialAttr|SsUtChangePitch|TitleMenuUpdate|UpdateEngineRpm|UpdateLapTiming|UpdatePlayerCar|UpdateSoundSlot|WaitSwCardEvent|_SsContExternal|_SsContResetAll|_SsInitSoundSeq|_SsSetPitchBend|_SsVmDoAllocate|_SsVmGetProgPan|_SsVmGetProgVol|_SsVmGetSeqLVol|_SsVmGetSeqRVol|_SsVmSetProgPan|_SsVmSetProgVol|_padCmdParaMode|_padInitSioMode|_padLoadActInfo|_padSetActAlign|_padSetMainMode|_spu_FsetDelayR|_spu_FsetDelayW|_spu_FwriteByIO|_spu_note2pitch|_spu_pitch2note|AddTextSprites|AddTileGrid2x2|AllocBadgeRows|AllocGlyphRows|AverageCarSpec|BuildRateCurve|CalcSteerLimit|CardFreeBlocks|CardReadFileAt|CardSystemInit|CdDataCallback|CdReadCallback|CdSyncCallback|CountCardFiles|DecayCarOffset|EmitFadeSprite|EmitPolyFT4Fog|EmitPolyFT4Raw|EmitSpriteGrid|FormatRaceTime|GetHwCardEvent|GetR4FileIndex|InitMenuArenas|NormalColorCol|NormalColorDpq|OuterProduct12|PadRumbleStart|PadSetActAlign|PadSetMainMode|PointInQuadPtr|RemovePatchPad|ScanCurveAhead|SetColorMatrix|SetInitPadFlag|SetLightMatrix|SetMenuTextRow|SetTransMatrix|SpuSetAnyVoice|SsSetSerialVol|SsSetTableSize|SsUtGetProgAtr|SsVabTransBody|StepCarPhysics|TitleModeEnter|TransformPoint|UpdateCarShake|VSyncCallbacks|_SsContMainVol|_SsUtBuildADSR|_SsVmDamperOff|_SsVmGetSeqVol|_SsVmKeyOffNow|_SsVmPitchBend|_SsVmSeqKeyOff|_SsVmSetSeqVol|_padGetActSize|_padInitDirSeq|startIntrVSync|ApplyMatrixLV|ApplyMatrixSV|CD_getsector2|CardWriteFile|CdAudioUpdate|CdGetDiskType|CheckCallback|ChecksumBytes|DpqColorLight|DrawHudSprite|EmitPolyF4Fog|EmitTiledRect|EvalRateCurve|FindTrackNode|GetGraphDebug|InitParticles|InitPlayerCar|InitRect16Row|MulRotMatrix0|OuterProduct0|PadInitDirect|PadRumbleStop|PlaySfxVolPan|ResetCallback|ResetCarState|ResetEntryInt|SceneLoadStep|SetDefDispEnv|SetDefDrawEnv|SetDrawOffset|SetFogNearFar|SetGeomOffset|SetGeomScreen|SetGraphDebug|SetGraphQueue|SetupRenderer|SetupViewport|SlotKeyOffAll|SpuInitMalloc|SsSetTickMode|SsUtAllKeyOff|SsUtGetVagAtr|SsUtReverbOff|SsUtSetVagAtr|SsVabFakeHead|StCdInterrupt|StepMenuValue|StepRaceFrame|TitleMenuInit|UpdateComCars|VSyncCallback|_SsContDamper|_SsContPanpot|_SsGetSeqData|_SsSndNextSep|_SsVmDamperOn|_SsVmKeyOnNow|_SsVmSeKeyOff|_dbl_shift_us|_spu_FgetRXXa|_spu_FsetRXXa|trapIntrVSync|AngleDiffAbs|BlendCarSpec|CD_getsector|CardReadFile|CdFileFinish|CdGetSector2|CdSearchFile|DS_cachefile|DS_searchdir|DeliverEvent|DrawBadgeRow|DrawHudDigit|GetVideoMode|HookEntryInt|InitBadgeRow|InitGlyphRow|InitMenuGrid|InitRaceCars|NormalColor3|PadEnableCom|QueueDrawStp|ReadCarInput|RotMatrixYXZ|RotMatrixZYX|RotTransPers|SetBackColor|SetCarBounds|SetCdMixMode|SetRotMatrix|SetSemiTrans|SetTexWindow|SetVertexTri|SetVideoMode|SlotSetPause|SoundStopAll|SpuSetReverb|SsUtReverbOn|StGetBackloc|StopCallback|SubmitModel2|VSync_memclr|_SsContNrpn1|_SsContNrpn2|_SsSndReplay|_SsSndSetVol|_SsVmPBVoice|_SsVmSeKeyOn|_dirFailAuto|_padChkVsync|_padStartCom|_patch_card2|_sio_control|_spu_FsetPCR|_spu_FsetRXX|setIntrVSync|startIntrDMA|AddTilePrim|AllocUiRows|ApplyMatrix|CD_datasync|CD_initintr|CardHasFile|CdAudioFade|CdAudioInit|CdAudioMute|CdDiskReady|CdGetSector|ClearImage2|DMACallback|DS_newmedia|DrawOTagEnv|EmitPolyFT3|EnableEvent|GetIntrMask|InitComCars|LerpAngle12|NormalColor|PadChkVsync|PadGetState|PadInfoComb|PadInfoMode|PadPollPort|PadStartCom|PointInQuad|ReverbReset|RotAverage3|RotAverage4|ScaleMatrix|SetDispMask|SetDrawArea|SetDrawMode|SetDrawMove|SetFarColor|SetIntrMask|SetPriority|SetSXSYfifo|SetShadeTex|SoundResume|SoundUpdate|SquareRoot0|SsSepSetVol|SsSeqGetVol|SsSeqSetVol|SsSetStereo|SsUtKeyOffV|SsUtSetVVol|StClearRing|StSetStream|StUnSetRing|StoreImage2|SysDeqIntRP|SysEnqIntRP|YawToOctant|_SsContRpn1|_SsContRpn2|_SsSndPause|_SsSndTempo|_SsVmKeyOff|_SsVmSetVol|_SsVmVSetUp|__floatsidf|_add_mant_d|_card_clear|_mul_mant_d|_padStopCom|_patch_card|dma_execute|note2pitch2|restartIntr|trapIntrDMA|Atan2Table|CD_initvol|CardUpdate|CdControlB|CdControlF|CdDataSync|CdFileStep|CdIntToPos|CdPosToInt|CdReadMode|CdReadSync|CdSetDebug|ClearImage|ClearOTagR|CloseEvent|DMA_memclr|DisablePAD|EmitPolyF4|FlushCache|GPU_memset|GetDispEnv|GetDrawEnv|KeyOnCheck|LightColor|LoadImage2|LocalLight|MoveImage2|MulMatrix0|MulMatrix2|NormalClip|PadChkMtap|PadInfoAct|PadStopCom|PutDispEnv|PutDrawEnv|ResetGraph|RotMatrixX|RotMatrixY|RotMatrixZ|SetDrawEnv|SetDrawStp|SetFogNear|SetRGBfifo|SetSZfifo3|SetSZfifo4|SetVertex0|SetVertex1|SetVertex2|SoundPause|SoundReset|SsSepClose|SsSeqClose|SsUtKeyOff|SsUtKeyOnV|SsVabClose|StFreeRing|StoreImage|SystemInit|TriggerSfx|XorBytes37|_SsSeqPlay|_SsSndPlay|_SsSndStop|_SsVmAlloc|_SsVmFlush|_SsVmKeyOn|_comp_mant|_dbl_shift|_padSetAct|_padSetCmd|_patch_gte|_patch_pad|_spu_FiDMA|_spu_Fw1ts|_spu_gcSPU|chkRC2wait|note2pitch|setIntrDMA|setRC2wait|vmNoiseOff|AverageZ3|AverageZ4|CdControl|CdLastCom|CdLastPos|ClearOTag|DpqColor3|DrawOTag2|DrawUiRow|EnablePAD|FileClose|FileWrite|InitUiRow|LoadClut2|LoadImage|LoadTPage|MoveImage|MulMatrix|OpenEvent|PAD_init2|PadSetAct|PadUpdate|ResetRCnt|RotMatrix|SetData32|SetMAC123|SetPolyF3|SetReverb|SetupRace|SpuSetKey|SsSepPlay|SsSepStop|SsSeqOpen|SsSeqPlay|SsSeqStop|SsSetMVol|SsSetMono|SsUtKeyOn|StGetNext|StSetMask|StSetRing|StartCARD|StartPAD2|StartRCnt|StopCARD2|TestEvent|VsGetAddr|WaitEvent|_ExitCard|_SsNoteOn|_SsVmInit|__fixdfsi|_err_math|_padSioRW|_spu_init|checkRECT|firstfile|get_alarm|set_alarm|startIntr|AddPrims|AlignUp4|CD_flush|CD_ready|CdComstr|CdIntstr|CdStatus|ColorCol|ColorDpq|DpqColor|DrawCars|DrawOTag|DrawPrim|DrawRing|DrawSync|FileOpen|FileRead|FileSeek|GetTPage|HsvToRgb|InitCARD|InitGeom|InitHeap|InitPAD2|LoadClut|MainLoop|PAD_init|RotTrans|SetIR123|SpuStart|SpuWrite|Square12|SsStart2|StartPAD|StopCARD|StopPAD2|StopRCnt|_SpuInit|_SsClose|_SsStart|__adddf3|__divdf3|__muldf3|__subdf3|_addque2|_mainasu|_spu_Fr_|get_mode|stopIntr|trapIntr|AddPrim|CD_init|CD_sync|CatPrim|CdFlush|CdRead2|CdReady|CdReset|GetClut|GetRCnt|InitPAD|LoadVab|PlaySfx|SetRCnt|SpuFree|SpuInit|Square0|SsStart|StopPAD|_addque|_exeque|_getctl|_spu_Fr|_spu_Fw|_status|get_ofs|getintr|mem2mem|setIntr|AddSIO|CD_vol|CdInit|CdMode|CdRead|CdSync|DelSIO|GetODE|SetDQA|SetDQB|SetIR0|SetMem|SetRii|_param|_reset|_spu_t|get_ce|get_cs|get_tw|memclr|ratan2|v_wait|CD_cw|CdMix|Intpl|VSync|_sync|sin_1|_clr|_ctl|_cwb|_cwc|_drs|_dws|_otc|rcos|rsin|Lzc)(?=\()',body))-{'ovl662_func_801154C8'})
head='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "__CONTROLLER_HEADER__"
#include "rr/animated_sprite_grid.h"
'''
head=head.replace('__CONTROLLER_HEADER__',re.search(r'#include "([^"]+)"',source.read_text())[1])
(P/'native_header.c').write_text(head);pre=subprocess.check_output(['cc','-E','-Iinclude','-I'+str(P),'-I'+str(source.parent),str(P/'native_header.c')],text=True)
protos={}
for name in funcs:
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1];types=[] if m[2].strip()=='void' else [x.strip() for x in m[2].split(',')];protos[name]=(ret,types)
code=head+'\n';guarded=[]
def guard(name,typ):
 global code
 i=len(guarded);guarded.append(name);code+=f'static struct {{u8 before[64]; {typ} value; u8 after[64];}} guard{i};\n#define {name} (guard{i}.value)\n'
for name in names:
 guard('n_'+name,f'__typeof__({name})');code+=f'#define {name} n_{name}\n'
code+='typedef RrUiRow24 Rows40[40];typedef u8 Mode56[56];typedef u8 Arena4096[4096];\ntypedef struct {RrGpuScratchPointers pointers;u8 tail[4096-8];} ScratchStorage;\n'
extra_types=[('frame0','RrFrameStorage'),('frame1','RrFrameStorage'),('rows0','Rows40'),('rows1','Rows40'),('badge0','RrBadgeRow1C'),('badge1','RrBadgeRow1C'),('mode0','Mode56'),('mode1','Mode56'),('arena0','Arena4096'),('arena1','Arena4096'),('arena2','Arena4096'),('scratch','ScratchStorage')]
for name,t in extra_types:guard(name,t)
code+='void init_guards(void){'
for i in range(len(guarded)):code+=f'memset(guard{i}.before,0xA6,64);memset(guard{i}.after,0x6A,64);'
code+='}\nunsigned check_guards(void){unsigned k;'
for i in range(len(guarded)):code+=f'for(k=0;k<64;k++)if(guard{i}.before[k]!=0xA6||guard{i}.after[k]!=0x6A)return {i+1};'
code+='return 0;}\ntypedef uintptr_t(*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback p){cb=p;}\n'
for i,name in enumerate(funcs):
 ret,types=protos[name];params=','.join(t+' a'+str(j) for j,t in enumerate(types)) or 'void'
 args=','.join(('((uintptr_t)a%d.r|((uintptr_t)a%d.g<<8)|((uintptr_t)a%d.b<<16))'%(j,j,j)) if t=='RrPreviewRgb3' else '(uintptr_t)a'+str(j) for j,t in enumerate(types)) or '0'
 code+=f'{ret} {name}({params}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\n'
objects=[('n_'+name,name) for name in names]+[(name,name) for name,t in extra_types]
code+='void *object(unsigned i){switch(i){'
for i,(cname,name) in enumerate(objects):code+=f'case {i}:return &{cname};'
code+='default:abort();}}\nsize_t size(unsigned i){switch(i){'
for i,(cname,name) in enumerate(objects):code+=f'case {i}:return sizeof({cname});'
code+='default:abort();}}\n'
(P/'native.c').write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Iinclude','-I'+str(P),'-I'+str(source.parent),'-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(P/'native.c'),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.size.restype=C.c_size_t
N={name:(lib.object(i),lib.size(i)) for i,(cname,name) in enumerate(objects)}
addr=lambda name:__import__('c_functions').address_of(name)
pointers={'D_800ACC14','LoadCursor','UiRows','BadgeRows','D_800F2AB0','D_800F4D70'}
regions=[(name,p,addr(name),size) for name,(p,size) in N.items() if name in names and name not in pointers]
extra_addr={'frame0':0x80180000,'frame1':0x801a2778,'rows0':0x801d0000,'rows1':0x801d05a0,'badge0':0x801d1000,'badge1':0x801d101c,'mode0':0x801d2000,'mode1':0x801d2038,'arena0':0x80200000,'arena1':0x80201000,'arena2':0x80202000}
for name,a in extra_addr.items():p,n=N[name];regions.append((name,p,a,n))
regions.append(('scratch_tail',N['scratch'][0]+2*C.sizeof(C.c_void_p),0x1f800008,4088))
assert N['PlayerCars'][1]==1600 and N['D_800F4D98'][1]==32 and N['D_800F3A38'][1]==184
assert all(p>0xffffffff for p,size in N.values())
raw=Path('assets/PAL/main.exe').read_bytes();r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+662*4)[0]
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1f800000,0x1000);u.mem_write(0x10000,raw[0x800:]);u.mem_write(0x114878,r4[sector*2048:sector*2048+0x20000])
if os.getenv('RR_COMPILED_ELF'):
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  e=ELFFile(f)
  for n in ['.text','.rodata']:
   s=e.get_section_by_name(n)
   if s:u.mem_write(s['sh_addr']&0x1fffffff,s.data())
def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
def pal(p):
 if not p:return 0
 for _,base,a,n in regions:
  if base<=p<base+n:return a+p-base
 raise AssertionError(('unowned nativepointer',hex(p)))
def host(a):
 if not a:return 0
 for _,p,base,n in regions:
  if base<=a<base+n:return p+a-base
 raise AssertionError(('unowned PALpointer',hex(a)))
def put(a,b,native):
 if native:C.memmove(host(a),b,len(b))
 else:write(a,b)
def val(a,n,native):return int.from_bytes(C.string_at(host(a),n) if native else read(a,n),'little')
def field(a,v,n,native):put(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'),native)
def root(name,a,native,index=0):
 if native:C.c_void_p.from_address(N[name][0]+index*8).value=host(a)
 else:write(addr(name)+index*4,struct.pack('<I',a))
def scratch_set(a,native,index=0):
 if native:C.c_void_p.from_address(N['scratch'][0]+index*C.sizeof(C.c_void_p)).value=host(a)
 else:write(0x1f800000+index*4,struct.pack('<I',a))
def scratch_get(native,index=0):return pal(C.c_void_p.from_address(N['scratch'][0]+index*C.sizeof(C.c_void_p)).value or 0) if native else int.from_bytes(read(0x1f800000+index*4,4),'little')
def snapshot(native):
 if native:
  bad=lib.check_guards();assert not bad,('native redzone',case,guarded[bad-1])
 b=b''.join(C.string_at(p,n) if native else read(a,n) for _,p,a,n in regions)
 roots=[]
 for name in sorted(pointers):
  for i in range(2 if name=='D_800F4D70' else 1):roots.append(pal(C.c_void_p.from_address(N[name][0]+i*8).value or 0) if native else int.from_bytes(read(addr(name)+i*4,4),'little'))
 roots += [scratch_get(native,i) for i in range(2)]
 return b+struct.pack('<9I',*roots)
case=0;events=[];observed=[];errors=[];coverage=set();coverage_states=set();markers=set()
def service(i,args,native):
 name=funcs[i];types=protos[name][1];args=list(args);norm=[]
 for j,t in enumerate(types):
  v=args[j];norm.append(pal(v) if native and '*' in t else (v&0xffffff if t=='RrPreviewRgb3' else v&0xffffffff))
 records=observed if native else events;records.append((i,norm,snapshot(native)));step=len(records);seed=case*17+step*29+i*7
 if not native:
  coverage.add(name)
  if name=='SetCarBounds':coverage_states.add(val(0x800F4D8A,2,False))
 # Mutate full owners and roots across callbacks while preserving array domains.
 if case&32:
  field(0x800ABEF8+(seed%1600),seed,1,native);field(0x800F3A38+(seed%184),seed,1,native)
  if step%3==0:
   root('UiRows',0x801d0000+(seed%2)*1440,native);root('BadgeRows',0x801d1000+(seed%2)*28,native)
   root('D_800F2AB0',0x801d2000+(seed%2)*56,native);root('D_800ACC14',0x80180000+(seed%2)*0x22778,native)
   root('D_800F4D70',0x80200000+(seed%2)*4096,native,0)
  field(0x800F4BEC,[-2147483648,-1,0,1,4,8,39,40][seed%8],4,native)
 if case&32 and step%3==0:
  scratch_set(0x80200000+(seed%3)*4096+64+(step%32)*32,native)
  scratch_set(0x80180000+(seed%2)*0x22778+(0x70 if seed&1 else 0xb70),native,1)
  field(0x1f800008+(seed%4088),seed,1,native)
 if name=='func_800207D4':
  cursor=scratch_get(native);head=norm[0];old=val(head,4,native)
  put(cursor,struct.pack('<4I',0x03000000|(old&0xffffff),0xA0000000|seed,norm[1],norm[2]),native)
  field(head,(old&0xff000000)|(cursor&0xffffff),4,native);scratch_set(cursor+16,native)
 if name=='StepMenuValue':
  width=norm[1];a=norm[2];limit=C.c_int32(norm[3]).value;low=C.c_int32(norm[4]).value
  assert width==2 and 0<limit<=9,(norm,case,[(funcs[e[0]],e[1]) for e in records])
  field(a,low+(seed%max(1,limit-low)),width,native)
 if name=='UnpackChoiceGlyph':
  # Real helper supplies three selection bytes inside the full32-byteowner.
  for k,v in enumerate([norm[1]&255,(norm[1]>>8)&255,[0,19,39,255][case%4]]):field(norm[0]+k,v,1,native)
 if name=='PackChoiceGlyph':return [0,1,0x10001,0x80000000][(case+step//2)%4]
 if name=='func_8003AE4C':return [1,1,1,0,0x1000001,-1][case%6]
 if name=='func_8003FBA8':return [0,1,-1,0x80000000,0x7fffffff][case%5]
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
@CB
def callback(i,args):
 try:return service(i,[args[j] for j in range(len(protos[funcs[i]][1]))],True)&0xffffffffffffffff
 except BaseException as e:errors.append(e);return 0
lib.set_callback(callback)
serviceAddrs={addr(n)&0x1fffffff:i for i,n in enumerate(funcs)}
resume=None
def hook(uc,a,size,data):
 global resume
 a&=0x1fffffff
 if a==0x3ff000:uc.emu_stop();return
 if a not in serviceAddrs:return
 i=serviceAddrs[a];sp=uc.reg_read(UC_MIPS_REG_SP);args=[]
 for j in range(len(protos[funcs[i]][1])):args.append(uc.reg_read(UC_MIPS_REG_A0+j) if j<4 else int.from_bytes(read(sp+16+(j-4)*4,4),'little'))
 resume=(i,args,uc.reg_read(UC_MIPS_REG_RA));uc.emu_stop()
def service_return(item):
 i,args,ra=item;result=service(i,args,False)
 for r in [UC_MIPS_REG_AT,UC_MIPS_REG_V0,UC_MIPS_REG_V1]+list(range(UC_MIPS_REG_A0,UC_MIPS_REG_T7+1))+[UC_MIPS_REG_T8,UC_MIPS_REG_T9,UC_MIPS_REG_HI,UC_MIPS_REG_LO]:u.reg_write(r,0xBAD00000+r+len(events))
 u.reg_write(UC_MIPS_REG_V0,result&0xffffffff)
 return ra
for a in serviceAddrs:write(a,struct.pack('<3I',0,0x03e00008,0))
for a in serviceAddrs:u.hook_add(UC_HOOK_CODE,hook,begin=a|0x80000000,end=a|0x80000000)
u.hook_add(UC_HOOK_CODE,hook,begin=0x803ff000,end=0x803ff000)
allowed=[(a,n) for _,p,a,n in regions]+[(addr(name),8 if name=='D_800F4D70' else 4) for name in pointers]+[(0x803dff00,0x100),(0x1f800000,8)]
def memwrite(uc,access,a,n,value,data):
 a=a&0x1fffffff
 if a==0x1272ba:markers.add('case3_publish')
 if a==0xf3a26:markers.add('case5_scene' if value==0x1b else 'case8_scene' if value==0x27 else 'other_scene')
 assert any((base&0x1fffffff)<=a and a+n<=(base&0x1fffffff)+size for base,size in allowed),('write outside fullowners/stack',case,hex(a),n,hex(uc.reg_read(UC_MIPS_REG_PC)))
u.hook_add(UC_HOOK_MEM_WRITE,memwrite)
rng=random.Random(662154)
for case in range(count):
 initial={name:rng.randbytes(n) for name,p,a,n in regions}
 for native in [False,True]:
  if native:lib.init_guards()
  for name,p,a,n in regions:put(a,initial[name],native)
  root('D_800ACC14',0x80180000+(case%2)*0x22778,native);root('LoadCursor',0x80200000,native)
  root('UiRows',0x801d0000+(case%2)*1440,native);root('BadgeRows',0x801d1000+(case%2)*28,native);root('D_800F2AB0',0x801d2000+(case%2)*56,native)
  scratch_set(0x80202000+64,native);scratch_set(0x80180000+0x70,native,1)
  root('D_800F4D70',0x80200000,native,0);root('D_800F4D70',0x80201000,native,1)
  for a,v,n in [(0x800AC5D4,[0,1,0x80000000,0xffffffff][case%4],4),(0x800AC64A,(case//6)%4,2),(0x801272E4,(case//9)%9,2),(0x801272E0,9,2),(0x801272D4,case%3,2),(0x800F4D8A,[0,0,0,3,4,5,8,1,9,65535][(case//6)%10],2),(0x800F4BEC,[-2147483648,-1,0,1,4,8,39,40][case%8],4),(0x800AC5D0,[0,1,2,64,32768,65535][case%6],2)]:field(a,v,n,native)
  for bank in range(2):
   phase=[1,2,3,5,6,7,-1,0][(case//7)%8];field(0x801d2000+bank*56+1,phase,1,native);field(0x801d2000+bank*56+2,case%256,1,native)
   field(0x801d1000+bank*28+0x10,[3,3,11,0][(case//11)%4],2,native)
   for row in range(40):field(0x801d0000+bank*1440+row*36+20,[2,2,0,3][(case+row)%4],1,native)
  field(0x800F3A38+0x28,[0,1,0x1000000][(case//7)%3],4,native);field(0x800F3A38+0x2c,[0,1][(case//11)%2],4,native)
  if not native:
   events=[];u.reg_write(UC_MIPS_REG_SP,0x803e0000);u.reg_write(UC_MIPS_REG_RA,0x803ff000)
   pc=0x801154c8
   for budget in range(512):
    resume=None
    try:u.emu_start(pc,0,count=50000)
    except UcError:
     print("CPU",hex(pc),hex(u.reg_read(UC_MIPS_REG_PC)),flush=True);raise
    if resume is None:break
    pc=service_return(resume)
   assert u.reg_read(UC_MIPS_REG_PC)&0x1fffffff==0x3ff000,(case,hex(u.reg_read(UC_MIPS_REG_PC)))
   final=snapshot(False)
  else:
   observed=[];lib.ovl662_func_801154C8();assert not errors,errors
   assert len(events)==len(observed),(case,len(events),len(observed))
   for k,(a,b) in enumerate(zip(events,observed)):
    
    if a!=b:
     off=next((j for j,(x,y) in enumerate(zip(a[2],b[2])) if x!=y),None);cur=0
     for rn,rp,ra,rs in regions:
      if off is not None and cur<=off<cur+rs:print('OWNER',rn,hex(ra+off-cur),a[2][off:off+16].hex(),b[2][off:off+16].hex(),flush=True)
      cur+=rs
     print('ROOTS',sorted(pointers),a[2][-28:].hex(),b[2][-28:].hex());raise AssertionError((case,k,funcs[a[0]],a[:2],b[:2],off))
   assert final==snapshot(True),('final',case)
 if case%512==0:print('PASS',case,flush=True)
assert coverage==set(funcs),('uncovered helpers',sorted(set(funcs)-coverage))
assert {0,3,4,5,8}<=coverage_states,('uncovered switch states',sorted(coverage_states))
assert {'case3_publish','case5_scene','case8_scene'}<=markers,('uncovered writes',sorted(markers))
print('PASS',count,'states',sorted(coverage_states),'markers',sorted(markers),'services',len(coverage),sorted(coverage),'native redzones/fullscratch/MIPSwriteguards',flush=True)

if os.getenv('RR_AUDIT_JSON'):
 deps=subprocess.check_output(['cc','-MM','-Iinclude','-I'+str(source.parent),str(source)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 files={str(Path(x).resolve().relative_to(ROOT)) if Path(x).resolve().is_relative_to(ROOT) else str(Path(x).resolve()):hashlib.sha256(Path(x).read_bytes()).hexdigest() for x in deps}
 report={'cases':count,'source':str(source.relative_to(ROOT)),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'dependencies':files,'services':sorted(coverage),'switch_states':sorted(coverage_states),'markers':sorted(markers),'original_mips':not bool(os.getenv('RR_COMPILED_ELF')),'native_redzones':True,'scratch_bytes':4096,'mips_write_guards':True,'sanitizers':['undefined','bounds']}
 if os.getenv('RR_COMPILED_ELF'):report['compiled_elf_sha256']=hashlib.sha256(Path(os.environ['RR_COMPILED_ELF']).read_bytes()).hexdigest()
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(report,indent=2)+'\n')
