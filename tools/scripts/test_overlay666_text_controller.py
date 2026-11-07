"""Overlay666 text controller: original/compiled MIPS vs guarded native owners.

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
P=Path(os.getenv('RR_TEST_DIR','build/tests/overlay666_text_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/666/func_80115590.c')
body=read_c_source(source);body=body[body.index('void ovl666_update_results'):]
global_names=sorted(set(re.findall(r'\b(?:(?:D_[0-9A-F]{8}|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)|ovl666_[a-z][a-z0-9_]*)\b',body)))
global_names=[n for n in global_names if not re.search(r"\b"+n+r"\s*\(",body)]
functions=sorted(set(re.findall(r'\b(?:func_[0-9A-F]{8}|DrawFontStringCenteredOffset|CdFileStateReloadDirectory|CdFileStateWaitShellClose|InterpolateObjectKeyframe|RelocateTrackLookupTables|DrawDialogueContinueMark|PatchOffsetTableTexPages|ScoreOvertakeOpportunity|SetupObjectLightMatrices|UpdateCarTrackProjection|ClearScreenOverlayState|GetSmoothedTrackHeading|InterpolateLanePointXYZ|LinkBattleEndModeUpdate|PlayStartCountdownVoice|ResetRankEntriesForCars|SetTrackObjectTransform|SpuSetTransferStartAddr|UpdateComCarTargetPoint|ClearTrackObjectStates|DrawFontStringCentered|DrawSplitScreenDivider|FindTrackNodeUnbounded|FormatMillisecondsTime|InitDefaultTimeRecords|InterpolateLanePointXZ|LookupTrackEventTable0|LookupTrackEventTable1|LookupTrackEventTable2|LookupTrackEventTable3|PatchModelBankTexPages|SoundMasterVolumeReset|SpuClearReverbWorkArea|SpuIsTransferCompleted|StopRaceAudioAndRumble|UpdateComCarLaneChange|ValidateRecordPassword|_SsVabOpenHeadWithMode|AddSpriteWithDrawMode|BuildCardHeaderStatic|CdAudioFindStreamFile|DrawSteeringIndicator|GetTrackTargetHeading|LookupTrackEventPair2|ProjectToTrackSegment|RestartMotionSequence|SetGteObjectTransform|SetNextModeByGameType|SetParticleBrightFade|SpuSetReverbModeParam|SsUtSetReverbFeedback|UpdateRaceChaseCamera|UpdateRaceIntroCamera|_SpuIsInAllocateArea_|_SsVmSelectToneAndVag|AllocMenuGridRecords|ApplyPauseMenuChoice|BuildRotMatrixAngles|CalcDistanceToPlayer|CalcRubberBandFactor|CdAudioFilterPending|CdAudioReadyCallback|CdFileStateStartRead|ComputeQuadMidpoints|ComputeStartDistance|DecodeRecordPassword|EncodeRecordPassword|EnterCriticalSection|FindTrackNodeScratch|FramesToMilliseconds|InitHudNumberTargets|InitMotionController|LookupTrackEventPair|PlanComCarCornerTurn|RelocateRelativeFile|RequestVramImageLoad|ResolveCarCollisions|SelectDialogueScript|SetGlyphRowImageBase|SetVoiceVolumeScaled|SoundFindNearestCars|SoundSetMasterVolume|TransformCourseModel|UpdateComCarOvertake|UpdatePlayerSteering|_SpuIsInAllocateArea|AllocCarSlotBuffers|ApplyRubberBandSpec|BuildCameraMatrices|BuildCardHeaderAnim|CD_set_test_parmnum|CdAudioRequestPause|CdAudioSetVolumeRaw|CdFileStateReadSync|ComputeWheelOffsets|ConstrainCarToTrack|CopyStringClearTail|CopyStringPadSpaces|DrawCourseInfoPanel|DrawFontStringRight|DrawGlyphListSloped|DrawHudRotatingQuad|DrawLinkEntryScreen|DrawLoadingTriangle|EmitSemiTransG3List|EmitSemiTransG4List|ExitCriticalSection|FindNearestCarAhead|FindTrackEventIndex|FindTrackNodeBehind|FindWheelTrackNodes|FormatDecimalDigits|FormatFrameTimeMMSS|GetTrackNodeHeading|InitCdFileDirectory|InitHudSpritePacket|InterpTrackPosition|LinkErrorModeUpdate|LoadComSlotBankStep|RelocateOffsetTable|ResetScreenCounters|ReturnFromException|SetMenuFrameOptions|SetModelTexturePage|SetTrackLaneOffsets|SlotSetLevelProgram|SoundListenerUpdate|SoundMasterFadeStep|SoundUpdateComSlots|SpuGetVoiceEnvelope|SsVabOpenHeadSticky|SsVabTransCompleted|StartCollisionShake|SteerCarTowardTrack|StepCarStateSpinOut|UnrelocateModelBank|UpdateDialoguePanel|UpdateLinkHandshake|UpdateRaceStandings|UpdateSelectionRank|_SsSetControlChange|_SsSetNrpnVabAttr10|_SsSetNrpnVabAttr11|_SsSetNrpnVabAttr12|_SsSetNrpnVabAttr13|_SsSetNrpnVabAttr14|_SsSetNrpnVabAttr15|_SsSetNrpnVabAttr16|_SsSetNrpnVabAttr17|_SsSetNrpnVabAttr18|_SsSetNrpnVabAttr19|_SsSetProgramChange|_SsSndSetReplayMode|_copy_memcard_patch|data_ready_callback|AddScreenFadeTiles|CardSystemShutdown|CdAudioSelectTrack|CdFileStateSetMode|CheckFinalLapEvent|CountProgressFlags|DrawMenuStepArrows|DrawModeSelectRows|DrawStartCountdown|EncodePasswordCode|FormatCountOfTotal|GameQueueTileTrans|GetTrackPointAhead|InitRaceHudPackets|IntegrateCarMotion|LinkWaitModeUpdate|LoadSoundBanksStep|LoadTrackNodesStep|ModeSelectMenuStep|PadSetRumbleParams|SetReverbDepthBoth|SetupMenuCarCamera|SoundCalcCarParams|SpuSetTransferMode|SsSetReservedVoice|SsUtSetReverbDelay|SsUtSetReverbDepth|StepCarStateNormal|SubmitCourseModel2|UpdateBadgeRowAnim|UpdateCameraMotion|UpdateCarTrackNode|UpdateSaveProgress|UpdateTrackObjects|_SsSetNrpnVabAttr0|_SsSetNrpnVabAttr1|_SsSetNrpnVabAttr2|_SsSetNrpnVabAttr3|_SsSetNrpnVabAttr4|_SsSetNrpnVabAttr5|_SsSetNrpnVabAttr6|_SsSetNrpnVabAttr7|_SsSetNrpnVabAttr8|_SsSetNrpnVabAttr9|_SsSndSetPauseMode|_card_sector_write|_padRecvAtLoadInfo|_padSendAtLoadInfo|_spu_getInTransfer|_spu_setInTransfer|_spu_setReverbAttr|AddOrSubtractByte|BuildObjectMatrix|CdAudioFilterStep|CdAudioResumeStep|CdFileBytesLoaded|CdFileStateSetloc|ClearHwCardEvents|ClearSwCardEvents|CountChoiceGlyphs|DecodeCourseIndex|DrawBadgeTitleBar|DrawBigFontString|DrawDigitString12|DrawDigitString16|EmitHudGlyph12Alt|EmitTexturedPanel|FindAxleTrackNode|FindTrackNodeAtXZ|GetRaceLoadCursor|GetTrackHeadingAt|InitDialogueState|InitSelectionRows|InterpCurveLinear|InterruptCallback|LinkWaitModeEnter|LoadRaceSoundStep|LoadTrackResource|PadSetRumbleFlags|PlayTrackEventSfx|RegisterModelBank|ResetVehicleShake|SetCarImpactState|SetIdentityMatrix|SetRaceLoadCursor|SetTrackStartNode|SetTrackWorkspace|SetupDrawDispEnvs|SetupViewLighting|SlotsStartForRace|SortRaceRankTable|SoundListenerInit|SoundQueueEnqueue|SoundReverbOffAll|SpawnParticlePair|SpuGetVoiceVolume|SpuSetReverbVoice|SsSetMarkCallback|SsUtGetReverbType|SsUtSetReverbType|StepCarStateSlide|SubmitCourseModel|UnpackChoiceGlyph|UpdateCarSteering|UpdateComCarDrive|UpdateComCarSpeed|UpdateDialogueBox|UpdateMenuTextRow|UpdateMotionTrack|UpdateRaceRanking|UpdateRumbleStrip|UpdateScrollTrack|VramImageLoadStep|_SsContBankChange|_SsContExpression|_SsReadDeltaValue|_padSetVsyncParam|_remove_ChgclrPAD|AddJitterSprites|ApplyCameraState|BuildLightMatrix|CdAudioBeginPlay|CdAudioPauseStep|CdAudioSetFilter|CdAudioSetTarget|CdAudioSetVolume|CdFileStateGetTN|CdFileStateRetry|CenterGlyphListX|ComSlotSetupBank|DrawDigitString8|DrawMeshPrimList|DrawSyncCallback|DrawTrackObjects|EmitPresetSprite|FindCurveSegment|FindFreeParticle|InitCameraMotion|InitMenuTextRows|InitViewLighting|LoadCarRangeStep|LoadSlotBankStep|LoadTrackObjects|LookupTrackEvent|PadRangeCheckSet|PollHwCardEvents|ReplayModeUpdate|RequestSceneLoad|ResetRankEntries|SaveCarSnapshots|SetTrackNodeData|ShuffleSlotOrder|SkipDialoguePage|SoundQueueUpdate|SplashLogoUpdate|SplashModeUpdate|SpuSetCommonAttr|SpuSetNoiseClock|SpuSetNoiseVoice|StartAttractDemo|SubmitModelFaces|TriggerWheelBump|UpdateCarHeading|UpdateMenuScript|UpdateScreenFade|UpdateWheelBumps|_SpuDataCallback|_SsContDataEntry|_SsUtResolveADSR|init_ring_status|AddDrawAreaPrim|AddDrawModePrim|AllocRect16Rows|AngleDiffSigned|BuildRotMatrixX|BuildRotMatrixY|BuildRotMatrixZ|ByteToCodeChars|CarLocalToWorld|CardWriteFileAt|CdAudioFadeStep|CdAudioLoopStep|CdAudioPlayStep|CdAudioStopStep|CdFileReadRange|CdFileReadStart|CdReadyCallback|ChangeClearRCnt|CheckRaceFinish|CloseLinkSerial|CodeCharsToByte|DrawLapTimeList|DrawMenuTextRow|DrawMeshQuadGT4|DrawRaceMinimap|GameQueueSprite|InitCameraState|InitMotionTrack|InitScrollTrack|InterpCurveEase|IntroScriptStep|LoadAllCarsStep|LoadCarSpecStep|LoadR4Directory|NormalColorCol3|NormalColorDpq3|PackChoiceGlyph|PadUpdateRumble|ReadInitPadFlag|ReplayModeEnter|RestartCallback|SelectModelBank|SeqMarkCallback|SetCarSpecTable|SetVehicleShake|SetupViewMatrix|SlotInterpCurve|Snd_SetPlayMode|SoundLoopUpdate|SoundQueueClear|SoundSystemInit|SplashModeEnter|SpuGetKeyStatus|SpuSetVoiceAttr|SsPitchFromNote|SsSeqCalledTbyT|SsSetSerialAttr|SsUtChangePitch|TitleMenuUpdate|UpdateCarEngine|UpdateEngineRpm|UpdateLapTiming|UpdateParticles|UpdatePauseMenu|UpdatePlayerCar|UpdateRect16Row|UpdateSoundSlot|WaitSwCardEvent|_SsContExternal|_SsContResetAll|_SsGetMetaEvent|_SsInitSoundSeq|_SsSetPitchBend|_SsSndCrescendo|_SsVmDoAllocate|_SsVmGetProgPan|_SsVmGetProgVol|_SsVmGetSeqLVol|_SsVmGetSeqRVol|_SsVmSetProgPan|_SsVmSetProgVol|_padCmdParaMode|_padInitSioMode|_padLoadActInfo|_padSetActAlign|_padSetMainMode|_padWaitRXready|_spu_FsetDelayR|_spu_FsetDelayW|_spu_FwriteByIO|_spu_note2pitch|_spu_pitch2note|AddPartialLine|AddTextSprites|AddTileGrid2x2|AllocBadgeRows|AllocGlyphRows|AllocImageRows|ApplyRotMatrix|AverageCarSpec|BuildRateCurve|CalcSteerLimit|CardFreeBlocks|CardReadFileAt|CardSystemInit|CdDataCallback|CdReadCallback|CdSyncCallback|ChangeClearPAD|CountCardFiles|DecayCarOffset|DrawHudNumber3|DrawHudNumber5|DrawLapCounter|DrawMeshQuadF4|DrawMeshQuadG4|EmitFadeSprite|EmitHudGlyph12|EmitPolyFT4Fog|EmitPolyFT4Raw|EmitSpriteGrid|FormatRaceTime|GetHwCardEvent|GetR4FileIndex|InitMenuArenas|InitMenuScript|LoadSfxVabStep|NormalColorCol|NormalColorDpq|OpenLinkSerial|OuterProduct12|PadRumbleStart|PadSetActAlign|PadSetMainMode|PlaceCarOnGrid|PointInQuadPtr|RemovePatchPad|RequestCarLoad|ScanCurveAhead|SeqMusicResume|SetColorMatrix|SetInitPadFlag|SetLightMatrix|SetMenuTextRow|SetTransMatrix|SlotLoadCurves|SlotSetProgram|SoundLoopReset|SpuSetAnyVoice|SsSetSerialVol|SsSetTableSize|SsUtGetDetVVol|SsUtGetProgAtr|SsUtSetDetVVol|SsVabTransBody|StCdInterrupt2|StepCarPhysics|StopRaceRumble|TitleModeEnter|TransformPoint|UpdateCarShake|UpdateDialogue|UpdateGlyphRow|UpdateImageRow|UpdateParticle|VSyncCallbacks|_SsContMainVol|_SsUtBuildADSR|_SsVmDamperOff|_SsVmGetSeqVol|_SsVmKeyOffNow|_SsVmPitchBend|_SsVmSeqKeyOff|_SsVmSetSeqVol|_padClrIntSio0|_padGetActSize|_padInitDirSeq|startIntrVSync|ApplyMatrixLV|ApplyMatrixSV|ApplySaveData|BuildSaveData|CD_getsector2|CardWriteFile|CdAudioUpdate|CdGetDiskType|CheckCallback|ChecksumBytes|DpqColorLight|DrawGlyphList|DrawHudSprite|DrawMenuFrame|DrawPauseMenu|EmitPolyF4Fog|EmitTiledRect|EvalRateCurve|FindTrackNode|GetGraphDebug|InitParticles|InitPlayerCar|InitRect16Row|LoadSceneStep|LoadTrackStep|MulRotMatrix0|OuterProduct0|PadClearPulse|PadInitDirect|PadRumbleStop|PadStartPulse|PlaySfxVolPan|RaceLoadReset|ResetCallback|ResetCarState|ResetEntryInt|SceneLoadStep|SeqMusicPause|SetDefDispEnv|SetDefDrawEnv|SetDrawOffset|SetFogNearFar|SetGeomOffset|SetGeomScreen|SetGraphDebug|SetGraphQueue|SetupRenderer|SetupViewport|SlotKeyOffAll|SlotSetupBank|SpuInitMalloc|SsSetTickMode|SsUtAllKeyOff|SsUtGetVagAtr|SsUtReverbOff|SsUtSetVagAtr|SsVabFakeHead|StCdInterrupt|StepMenuValue|StepRaceFrame|TitleMenuInit|UpdateCarGear|UpdateComCars|UploadCarClut|VSyncCallback|_SsContDamper|_SsContPanpot|_SsGetSeqData|_SsSndNextSep|_SsVmDamperOn|_SsVmKeyOnNow|_SsVmSeKeyOff|_dbl_shift_us|_spu_FgetRXXa|_spu_FsetRXXa|cd_read_retry|trapIntrVSync|AngleDiffAbs|BlendCarSpec|CD_getsector|CardReadFile|CdFileFinish|CdGetSector2|CdSearchFile|DS_cachefile|DS_searchdir|DeliverEvent|DrawBadgeRow|DrawHudDigit|DrawImageRow|DrawParticle|DrawRaceTime|GetVideoMode|HookEntryInt|InitBadgeRow|InitCarTable|InitGamePads|InitGlyphRow|InitImageRow|InitMenuGrid|InitRaceCars|LinkExchange|NormalColor3|PadEnableCom|PlaySfxBank2|QueueDrawStp|ReadCarInput|RotMatrixYXZ|RotMatrixZYX|RotTransPers|SeqSetVolume|SetBackColor|SetCarBounds|SetCdMixMode|SetRotMatrix|SetSemiTrans|SetTexWindow|SetVertexTri|SetVideoMode|SlotSetPause|SoundLoopSet|SoundStopAll|SpuSetReverb|SsUtReverbOn|StGetBackloc|StepNameChar|StopCallback|SubmitModel2|VSync_memclr|_SsContNrpn1|_SsContNrpn2|_SsSeqGetEof|_SsSndReplay|_SsSndSetVol|_SsVmPBVoice|_SsVmSeKeyOn|_card_format|_card_status|_dirFailAuto|_padChkVsync|_padStartCom|_patch_card2|_sio_control|_spu_FsetPCR|_spu_FsetRXX|setIntrVSync|startIntrDMA|AddTilePrim|AllocUiRows|ApplyMatrix|CD_datasync|CD_initintr|CarLoadStep|CardHasFile|CdAudioFade|CdAudioInit|CdAudioMute|CdDiskReady|CdGetSector|CdReadBreak|ClearImage2|DMACallback|DS_newmedia|DrawOTagEnv|EmitPolyFT3|EnableEvent|GetIntrMask|GetNameChar|InitComCars|LerpAngle12|MemCardSync|NormalColor|PadChkVsync|PadGetState|PadInfoComb|PadInfoMode|PadPollPort|PadStartCom|PointInQuad|ReverbReset|RotAverage3|RotAverage4|ScaleMatrix|SeqFadeStep|SetDispMask|SetDrawArea|SetDrawEnv2|SetDrawMode|SetDrawMove|SetFarColor|SetIntrMask|SetPriority|SetSXSYfifo|SetShadeTex|SoundResume|SoundUpdate|SquareRoot0|SsSepReplay|SsSepSetVol|SsSeqGetVol|SsSeqReplay|SsSeqSetVol|SsSetStereo|SsUtGetVVol|SsUtKeyOffV|SsUtSetVVol|StClearRing|StSetStream|StUnSetRing|StoreImage2|SysDeqIntRP|SysEnqIntRP|YawToOctant|_SsContRpn1|_SsContRpn2|_SsSndPause|_SsSndTempo|_SsVmKeyOff|_SsVmSetVol|_SsVmVSetUp|__floatsidf|_add_mant_d|_card_clear|_card_write|_mul_mant_d|_padIsVsync|_padStopCom|_patch_card|def_cbready|dma_execute|note2pitch2|restartIntr|trapIntrDMA|Atan2Table|CD_initvol|CardUpdate|CdControlB|CdControlF|CdDataSync|CdFileStep|CdIntToPos|CdPosToInt|CdReadMode|CdReadSync|CdSetDebug|ClearImage|ClearOTagR|CloseEvent|DMA_memclr|DisablePAD|EmitPolyF4|FinishRace|FlushCache|GPU_memset|GetDispEnv|GetDrawEnv|KeyOnCheck|LightColor|LoadImage2|LocalLight|MoveImage2|MulMatrix0|MulMatrix2|NormalClip|PadChkMtap|PadInfoAct|PadStopCom|PutDispEnv|PutDrawEnv|ResetGraph|RotMatrixX|RotMatrixY|RotMatrixZ|SetDrawEnv|SetDrawStp|SetFogNear|SetPolyFT3|SetRGBfifo|SetSZfifo3|SetSZfifo4|SetVertex0|SetVertex1|SetVertex2|SoundPause|SoundReset|SsSepClose|SsSepPause|SsSeqClose|SsSeqPause|SsUtKeyOff|SsUtKeyOnV|SsVabClose|StFreeRing|StartCARD2|StoreImage|SystemInit|TriggerSfx|XorBytes37|_96_remove|_SsSeqPlay|_SsSndPlay|_SsSndStop|_SsVmAlloc|_SsVmFlush|_SsVmKeyOn|_card_info|_card_load|_card_read|_comp_mant|_dbl_shift|_padIntPad|_padSetAct|_padSetCmd|_padSioRW2|_patch_gte|_patch_pad|_spu_FiDMA|_spu_Fw1ts|_spu_gcSPU|chkRC2wait|def_cbread|def_cbsync|firstfile2|note2pitch|setIntrDMA|setRC2wait|vmNoiseOff|AverageZ3|AverageZ4|CdControl|CdLastCom|CdLastPos|ClearOTag|DpqColor3|DrawOTag2|DrawUiBox|DrawUiRow|EnablePAD|FileClose|FileWrite|InitCARD2|InitUiRow|IntroInit|LoadClut2|LoadImage|LoadTPage|MoveImage|MulMatrix|OpenEvent|PAD_init2|PadSetAct|PadUpdate|ResetRCnt|RotMatrix|SetData32|SetLineF2|SetLineG2|SetMAC123|SetPolyF3|SetReverb|SetupRace|SlotKeyOn|SlotStart|SpuSetKey|SsSepPlay|SsSepStop|SsSeqOpen|SsSeqPlay|SsSeqStop|SsSetMVol|SsSetMono|SsUtKeyOn|StGetNext|StSetMask|StSetRing|StartCARD|StartPAD2|StartRCnt|StopCARD2|TestEvent|VsGetAddr|WaitEvent|_ExitCard|_SsNoteOn|_SsVmInit|__fixdfsi|_err_math|_new_card|_padSioRW|_spu_init|checkRECT|firstfile|get_alarm|set_alarm|startIntr|vmNoiseOn|AddPrims|AlignUp4|CD_flush|CD_ready|CdComstr|CdIntstr|CdStatus|ColorCol|ColorDpq|DpqColor|DrawCars|DrawOTag|DrawPrim|DrawRing|DrawSync|FileOpen|FileRead|FileSeek|GetTPage|HsvToRgb|InitCARD|InitGeom|InitHeap|InitPAD2|LoadClut|MainLoop|PAD_init|RotTrans|SetIR123|SpuStart|SpuWrite|Square12|SsStart2|StartPAD|StopCARD|StopPAD2|StopRCnt|_SpuInit|_SsClose|_SsStart|__adddf3|__divdf3|__muldf3|__negdf2|__subdf3|_addque2|_bu_init|_mainasu|_spu_Fr_|callback|get_mode|stopIntr|trapIntr|AddPrim|CD_init|CD_sync|CatPrim|CdFlush|CdRead2|CdReady|CdReset|GetClut|GetRCnt|InitPAD|LoadVab|PlaySfx|SetRCnt|SetSprt|SetTile|SpuFree|SpuInit|Square0|SsStart|StopPAD|_SsInit|__gtdf2|__ltdf2|_addque|_exeque|_getctl|_spu_Fr|_spu_Fw|_status|cb_data|cb_read|cd_read|csqrt_1|get_ofs|getintr|mem2mem|setIntr|AddDrv|AddSIO|CD_vol|CdInit|CdMode|CdRead|CdSync|DelDrv|DelSIO|GetODE|SetDQA|SetDQB|SetIR0|SetMem|SetRii|SsInit|_param|_reset|_spu_t|get_ce|get_cs|get_tw|memclr|ratan2|v_wait|CD_cw|CdMix|Intpl|VSync|_sync|sin_1|_clr|_cmp|_ctl|_cwb|_cwc|_drs|_dws|_otc|rcos|rsin|Lzc|ovl666_[a-z][a-z0-9_]*)(?=\()',body))-{'ovl666_update_results'})
head='#include <stdint.h>\n#include <stddef.h>\n#include <stdlib.h>\n#include "rr/overlay666_text_controller.h"\n'
(P/'header.c').write_text(head)
pre=subprocess.check_output(['cc','-E','-Iinclude',str(P/'header.c')],text=True)
protos={};code=head+'\n'
for name in global_names:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
code+='''
static struct {u32 guard[4];RrFrameStorage data[2];u32 tail[4];} frames;
static struct {u32 guard[4];RrMenuTextRow60 data[2][44];u32 tail[4];} texts;
_Static_assert(sizeof(RrFrameStorage)==0x22778,"complete frame owner");
_Static_assert(sizeof(RrMenuTextRow60)==60,"real row stride");
typedef uintptr_t (*Callback)(unsigned,uintptr_t*);
static Callback cb;void set_callback(Callback c){cb=c;}
'''
for i,name in enumerate(functions):
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1].removeprefix('extern ');params=m[2];types=[] if params.strip() in ['', 'void'] else [p.strip() for p in params.split(',')]
 protos[name]=(ret,types);args=','.join('(uintptr_t)a'+str(j) for j in range(len(types))) or '0'
 decl=','.join(t+' a'+str(j) for j,t in enumerate(types)) or 'void'
 code+=f'{ret} {name}({decl}){{uintptr_t a[4]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\nvoid *object(unsigned i){switch(i){\n'
for i,name in enumerate(global_names):code+=f'case {i}:return &n_{name};\n'
G=len(global_names)
code+=f'case {G}:return &frames;case {G+1}:return &texts;default:abort();}}}}\n'
code+='size_t object_size(unsigned i){switch(i){\n'
for i,name in enumerate(global_names):code+=f'case {i}:return sizeof(n_{name});\n'
code+=f'case {G}:return sizeof(frames);case {G+1}:return sizeof(texts);default:abort();}}}}\n'
code+='void roots(unsigned f,unsigned t){D_800ACC14=&frames.data[f];MenuTextRows=texts.data[t];}\n'
native=P/'native.c';native.write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Iinclude','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(native),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
N={n:(lib.object(i),lib.object_size(i)) for i,n in enumerate(global_names)}
frame=lib.object(G);frame_size=lib.object_size(G);text=lib.object(G+1);text_size=lib.object_size(G+1)
assert frame>0xffffffff and text>0xffffffff and frame_size==32+2*0x22778 and text_size==32+2*44*60
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+666*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
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
phase_values=[0,3,4,5,6,8];signed_bytes=[-128,-1,0,1,2,3,7,8,11,12,20,21,127];timers=[-32768,-1,0,1,2,3,63,64,65,32767]
case=0;events=[];seen=0;errors=[];coverage=set();statecoverage=set();groupcoverage=set();returns=set();mutations=set()
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
 if name=='UpdateMenuTextRow':groupcoverage.add(normalized[0]//11)
 if case&64:
  # Helpers can replace both live roots and all scalar state. The phase was
  # already captured by the switch; fresh timer/row reads must still agree.
  value=(case*0x9e3779b9+event*97+serial*31)&0xffffffff
  changes={'ModeFrame':value,'ScreenFrame':~value,'D_800AC5D0':timers[(case+serial)%len(timers)],'D_800F3A60':(case+serial)%3-1,'TotalRaceTime':[-1,0,1][(case+serial)%3],'ovl666_result_kind':(case+serial)&1,'MenuCursor':(case+serial)%3,'D_800F4D8A':value&65535,'ModeGroup':value,'D_800F3A26':value>>8,'MenuScriptId':value>>16}
  for n,v in changes.items():scalar(n,v,native)
  for bank in range(2):
   for row,off in [(0,2),(11,2),(5,3),(38,3)]:textbyte(bank,row,off,signed_bytes[(case+serial+row)%len(signed_bytes)],native)
  mutations.add(name)
 if case&128:roots((case+serial)&1,((case//2)+serial)&1,native)
 result=0
 if name=='ovl666_update_retry_choice':result=[0xffffffff,0,1,0x80000000,0x7fffffff][(case//6)%5]
 if name=='func_8003B12C':result=[0,1,2,0xffffffff,0x10001,0x80000000][(case//6)%6]
 if name in ['ovl666_update_retry_choice','func_8003B12C']:returns.add((name,result))
 return result
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:errors.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
argregs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
volatile_regs=[UC_MIPS_REG_V1]+argregs+[globals()[f'UC_MIPS_REG_T{i}'] for i in range(10)]
service_map={addr(n):i for i,n in enumerate(functions)}
def hook(uc,address,size,data):
 event=service_map[address];argc=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in argregs[:argc]]
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
 values={'ModeFrame':[0x7fffffff,0x80000000,0xffffffff,0,17][n%5],'ScreenFrame':[0,1,0xffffffff,0x7fffffff,0x80000000][(n//5)%5],
 'D_800F4D8A':phase_values[n%6],'TotalRaceTime':[-1,0,1,0x80000000][(n//6)%4],'D_800F3A60':[0,1,-1][(n//24)%3],
 'ovl666_result_kind':(n//12)&1,'D_800AC5D0':timers[(n//6)%len(timers)],'MenuCursor':(n//36)%3}
 for name,value in values.items():scalar(name,value,native)
 for bank in range(2):
  for row,off in [(0,2),(11,2),(5,3),(38,3)]:textbyte(bank,row,off,signed_bytes[(n//6+row+bank)%len(signed_bytes)],native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 4096
for case in range(count):
 events=[];seen=0;initialize(case,False);statecoverage.add(phase_values[case%6])
 for r in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(r,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800);u.reg_write(UC_MIPS_REG_HI,0x12345678);u.reg_write(UC_MIPS_REG_LO,0x87654321)
 u.emu_start(0x80115590,0x803FF800,count=30000);assert u.reg_read(UC_MIPS_REG_PC)==0x803FF800
 expected=snapshot(False);initialize(case,True);lib.ovl666_update_results()
 if errors:raise errors[0]
 assert seen==len(events),(case,'missing services');assert expected==snapshot(True),(case,'final full owners')
 if case and case%512==0:print(case,'cases',flush=True)
assert coverage==set(functions),(coverage,set(functions));assert statecoverage==set(phase_values) and groupcoverage=={0,1,3}
print('PASS',count,('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS'),'services',len(coverage),'states',sorted(statecoverage),'groups',sorted(groupcoverage),flush=True)
if os.getenv('RR_AUDIT_JSON'):
 sha=lambda f:hashlib.sha256(resolve_c_source_path(f).read_bytes()).hexdigest()
 deps=subprocess.check_output(['cc','-MM','-Iinclude',str(native)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 audit=dict(source=str(resolve_c_source_path(source)),source_sha256=sha(source),test_sha256=sha(__file__),dependencies={f:sha(f) for f in deps},cases=count,engine='compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS',services=sorted(coverage),phases=sorted(statecoverage),groups=sorted(groupcoverage),returns=sorted(returns),mutating_services=sorted(mutations),full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
