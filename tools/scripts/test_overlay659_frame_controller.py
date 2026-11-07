"""Overlay659 frame controller: original/compiled MIPS vs full native owners.

Stateful mock services, full-width software pointers and precise argument lists.
Every service entry and the final guarded world are compared against MIPS.
"""
from overlay_units import read_c_source, symbol_addresses, resolve_c_source_path
SYMBOL_ADDRESSES = symbol_addresses()
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os,hashlib
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
ROOT=Path(__file__).resolve().parents[2];os.chdir(ROOT)
P=Path(os.getenv('RR_TEST_DIR','build/tests/overlay659_frame_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/659/func_80114908.c')
body=read_c_source(source);body=body[body.index('void ovl659_update_race'):]
names=sorted(set(re.findall(r'\b(?:(?:D_[0-9A-F]{8}|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)|ovl659_[a-z][a-z0-9_]*)\b',body)))
names=[n for n in names if not re.search(r"\b"+n+r"\s*\(",body)]
assert 'D_800AD4EF' not in names
functions=sorted(set(re.findall(r'\b(?:ovl659_)?func_[0-9A-F]{8}|DrawFontStringCenteredOffset|CdFileStateReloadDirectory|CdFileStateWaitShellClose|InterpolateObjectKeyframe|RelocateTrackLookupTables|DrawDialogueContinueMark|PatchOffsetTableTexPages|ScoreOvertakeOpportunity|SetupObjectLightMatrices|UpdateCarTrackProjection|ClearScreenOverlayState|GetSmoothedTrackHeading|InterpolateLanePointXYZ|LinkBattleEndModeUpdate|PlayStartCountdownVoice|ResetRankEntriesForCars|SetTrackObjectTransform|SpuSetTransferStartAddr|UpdateComCarTargetPoint|ClearTrackObjectStates|DrawFontStringCentered|DrawSplitScreenDivider|FindTrackNodeUnbounded|FormatMillisecondsTime|InitDefaultTimeRecords|InterpolateLanePointXZ|LookupTrackEventTable0|LookupTrackEventTable1|LookupTrackEventTable2|LookupTrackEventTable3|PatchModelBankTexPages|SoundMasterVolumeReset|SpuClearReverbWorkArea|SpuIsTransferCompleted|StopRaceAudioAndRumble|UpdateComCarLaneChange|ValidateRecordPassword|_SsVabOpenHeadWithMode|AddSpriteWithDrawMode|BuildCardHeaderStatic|CdAudioFindStreamFile|DrawSteeringIndicator|GetTrackTargetHeading|LookupTrackEventPair2|ProjectToTrackSegment|RestartMotionSequence|SetGteObjectTransform|SetNextModeByGameType|SetParticleBrightFade|SpuSetReverbModeParam|SsUtSetReverbFeedback|UpdateRaceChaseCamera|UpdateRaceIntroCamera|_SpuIsInAllocateArea_|_SsVmSelectToneAndVag|AllocMenuGridRecords|ApplyPauseMenuChoice|BuildRotMatrixAngles|CalcDistanceToPlayer|CalcRubberBandFactor|CdAudioFilterPending|CdAudioReadyCallback|CdFileStateStartRead|ComputeQuadMidpoints|ComputeStartDistance|DecodeRecordPassword|EncodeRecordPassword|EnterCriticalSection|FindTrackNodeScratch|FramesToMilliseconds|InitHudNumberTargets|InitMotionController|LookupTrackEventPair|PlanComCarCornerTurn|RelocateRelativeFile|RequestVramImageLoad|ResolveCarCollisions|SelectDialogueScript|SetGlyphRowImageBase|SetVoiceVolumeScaled|SoundFindNearestCars|SoundSetMasterVolume|TransformCourseModel|UpdateComCarOvertake|UpdatePlayerSteering|_SpuIsInAllocateArea|AllocCarSlotBuffers|ApplyRubberBandSpec|BuildCameraMatrices|BuildCardHeaderAnim|CD_set_test_parmnum|CdAudioRequestPause|CdAudioSetVolumeRaw|CdFileStateReadSync|ComputeWheelOffsets|ConstrainCarToTrack|CopyStringClearTail|CopyStringPadSpaces|DrawCourseInfoPanel|DrawFontStringRight|DrawGlyphListSloped|DrawHudRotatingQuad|DrawLinkEntryScreen|DrawLoadingTriangle|EmitSemiTransG3List|EmitSemiTransG4List|ExitCriticalSection|FindNearestCarAhead|FindTrackEventIndex|FindTrackNodeBehind|FindWheelTrackNodes|FormatDecimalDigits|FormatFrameTimeMMSS|GetTrackNodeHeading|InitCdFileDirectory|InitHudSpritePacket|InterpTrackPosition|LinkErrorModeUpdate|LoadComSlotBankStep|RelocateOffsetTable|ResetScreenCounters|ReturnFromException|SetMenuFrameOptions|SetModelTexturePage|SetTrackLaneOffsets|SlotSetLevelProgram|SoundListenerUpdate|SoundMasterFadeStep|SoundUpdateComSlots|SpuGetVoiceEnvelope|SsVabOpenHeadSticky|SsVabTransCompleted|StartCollisionShake|SteerCarTowardTrack|StepCarStateSpinOut|UnrelocateModelBank|UpdateDialoguePanel|UpdateLinkHandshake|UpdateRaceStandings|UpdateSelectionRank|_SsSetControlChange|_SsSetNrpnVabAttr10|_SsSetNrpnVabAttr11|_SsSetNrpnVabAttr12|_SsSetNrpnVabAttr13|_SsSetNrpnVabAttr14|_SsSetNrpnVabAttr15|_SsSetNrpnVabAttr16|_SsSetNrpnVabAttr17|_SsSetNrpnVabAttr18|_SsSetNrpnVabAttr19|_SsSetProgramChange|_SsSndSetReplayMode|_copy_memcard_patch|data_ready_callback|AddScreenFadeTiles|CardSystemShutdown|CdAudioSelectTrack|CdFileStateSetMode|CheckFinalLapEvent|CountProgressFlags|DrawMenuStepArrows|DrawModeSelectRows|DrawStartCountdown|EncodePasswordCode|FormatCountOfTotal|GameQueueTileTrans|GetTrackPointAhead|InitRaceHudPackets|IntegrateCarMotion|LinkWaitModeUpdate|LoadSoundBanksStep|LoadTrackNodesStep|ModeSelectMenuStep|PadSetRumbleParams|SetReverbDepthBoth|SetupMenuCarCamera|SoundCalcCarParams|SpuSetTransferMode|SsSetReservedVoice|SsUtSetReverbDelay|SsUtSetReverbDepth|StepCarStateNormal|SubmitCourseModel2|UpdateBadgeRowAnim|UpdateCameraMotion|UpdateCarTrackNode|UpdateSaveProgress|UpdateTrackObjects|_SsSetNrpnVabAttr0|_SsSetNrpnVabAttr1|_SsSetNrpnVabAttr2|_SsSetNrpnVabAttr3|_SsSetNrpnVabAttr4|_SsSetNrpnVabAttr5|_SsSetNrpnVabAttr6|_SsSetNrpnVabAttr7|_SsSetNrpnVabAttr8|_SsSetNrpnVabAttr9|_SsSndSetPauseMode|_card_sector_write|_padRecvAtLoadInfo|_padSendAtLoadInfo|_spu_getInTransfer|_spu_setInTransfer|_spu_setReverbAttr|AddOrSubtractByte|BuildObjectMatrix|CdAudioFilterStep|CdAudioResumeStep|CdFileBytesLoaded|CdFileStateSetloc|ClearHwCardEvents|ClearSwCardEvents|CountChoiceGlyphs|DecodeCourseIndex|DrawBadgeTitleBar|DrawBigFontString|DrawDigitString12|DrawDigitString16|EmitHudGlyph12Alt|EmitTexturedPanel|FindAxleTrackNode|FindTrackNodeAtXZ|GetRaceLoadCursor|GetTrackHeadingAt|InitDialogueState|InitSelectionRows|InterpCurveLinear|InterruptCallback|LinkWaitModeEnter|LoadRaceSoundStep|LoadTrackResource|PadSetRumbleFlags|PlayTrackEventSfx|RegisterModelBank|ResetVehicleShake|SetCarImpactState|SetIdentityMatrix|SetRaceLoadCursor|SetTrackStartNode|SetTrackWorkspace|SetupDrawDispEnvs|SetupViewLighting|SlotsStartForRace|SortRaceRankTable|SoundListenerInit|SoundQueueEnqueue|SoundReverbOffAll|SpawnParticlePair|SpuGetVoiceVolume|SpuSetReverbVoice|SsSetMarkCallback|SsUtGetReverbType|SsUtSetReverbType|StepCarStateSlide|SubmitCourseModel|UnpackChoiceGlyph|UpdateCarSteering|UpdateComCarDrive|UpdateComCarSpeed|UpdateDialogueBox|UpdateMenuTextRow|UpdateMotionTrack|UpdateRaceRanking|UpdateRumbleStrip|UpdateScrollTrack|VramImageLoadStep|_SsContBankChange|_SsContExpression|_SsReadDeltaValue|_padSetVsyncParam|_remove_ChgclrPAD|AddJitterSprites|ApplyCameraState|BuildLightMatrix|CdAudioBeginPlay|CdAudioPauseStep|CdAudioSetFilter|CdAudioSetTarget|CdAudioSetVolume|CdFileStateGetTN|CdFileStateRetry|CenterGlyphListX|ComSlotSetupBank|DrawDigitString8|DrawMeshPrimList|DrawSyncCallback|DrawTrackObjects|EmitPresetSprite|FindCurveSegment|FindFreeParticle|InitCameraMotion|InitMenuTextRows|InitViewLighting|LoadCarRangeStep|LoadSlotBankStep|LoadTrackObjects|LookupTrackEvent|PadRangeCheckSet|PollHwCardEvents|ReplayModeUpdate|RequestSceneLoad|ResetRankEntries|SaveCarSnapshots|SetTrackNodeData|ShuffleSlotOrder|SkipDialoguePage|SoundQueueUpdate|SplashLogoUpdate|SplashModeUpdate|SpuSetCommonAttr|SpuSetNoiseClock|SpuSetNoiseVoice|StartAttractDemo|SubmitModelFaces|TriggerWheelBump|UpdateCarHeading|UpdateMenuScript|UpdateScreenFade|UpdateWheelBumps|_SpuDataCallback|_SsContDataEntry|_SsUtResolveADSR|init_ring_status|AddDrawAreaPrim|AddDrawModePrim|AllocRect16Rows|AngleDiffSigned|BuildRotMatrixX|BuildRotMatrixY|BuildRotMatrixZ|ByteToCodeChars|CarLocalToWorld|CardWriteFileAt|CdAudioFadeStep|CdAudioLoopStep|CdAudioPlayStep|CdAudioStopStep|CdFileReadRange|CdFileReadStart|CdReadyCallback|ChangeClearRCnt|CheckRaceFinish|CloseLinkSerial|CodeCharsToByte|DrawLapTimeList|DrawMenuTextRow|DrawMeshQuadGT4|DrawRaceMinimap|GameQueueSprite|InitCameraState|InitMotionTrack|InitScrollTrack|InterpCurveEase|IntroScriptStep|LoadAllCarsStep|LoadCarSpecStep|LoadR4Directory|NormalColorCol3|NormalColorDpq3|PackChoiceGlyph|PadUpdateRumble|ReadInitPadFlag|ReplayModeEnter|RestartCallback|SelectModelBank|SeqMarkCallback|SetCarSpecTable|SetVehicleShake|SetupViewMatrix|SlotInterpCurve|Snd_SetPlayMode|SoundLoopUpdate|SoundQueueClear|SoundSystemInit|SplashModeEnter|SpuGetKeyStatus|SpuSetVoiceAttr|SsPitchFromNote|SsSeqCalledTbyT|SsSetSerialAttr|SsUtChangePitch|TitleMenuUpdate|UpdateCarEngine|UpdateEngineRpm|UpdateLapTiming|UpdateParticles|UpdatePauseMenu|UpdatePlayerCar|UpdateRect16Row|UpdateSoundSlot|WaitSwCardEvent|_SsContExternal|_SsContResetAll|_SsGetMetaEvent|_SsInitSoundSeq|_SsSetPitchBend|_SsSndCrescendo|_SsVmDoAllocate|_SsVmGetProgPan|_SsVmGetProgVol|_SsVmGetSeqLVol|_SsVmGetSeqRVol|_SsVmSetProgPan|_SsVmSetProgVol|_padCmdParaMode|_padInitSioMode|_padLoadActInfo|_padSetActAlign|_padSetMainMode|_padWaitRXready|_spu_FsetDelayR|_spu_FsetDelayW|_spu_FwriteByIO|_spu_note2pitch|_spu_pitch2note|AddPartialLine|AddTextSprites|AddTileGrid2x2|AllocBadgeRows|AllocGlyphRows|AllocImageRows|ApplyRotMatrix|AverageCarSpec|BuildRateCurve|CalcSteerLimit|CardFreeBlocks|CardReadFileAt|CardSystemInit|CdDataCallback|CdReadCallback|CdSyncCallback|ChangeClearPAD|CountCardFiles|DecayCarOffset|DrawHudNumber3|DrawHudNumber5|DrawLapCounter|DrawMeshQuadF4|DrawMeshQuadG4|EmitFadeSprite|EmitHudGlyph12|EmitPolyFT4Fog|EmitPolyFT4Raw|EmitSpriteGrid|FormatRaceTime|GetHwCardEvent|GetR4FileIndex|InitMenuArenas|InitMenuScript|LoadSfxVabStep|NormalColorCol|NormalColorDpq|OpenLinkSerial|OuterProduct12|PadRumbleStart|PadSetActAlign|PadSetMainMode|PlaceCarOnGrid|PointInQuadPtr|RemovePatchPad|RequestCarLoad|ScanCurveAhead|SeqMusicResume|SetColorMatrix|SetInitPadFlag|SetLightMatrix|SetMenuTextRow|SetTransMatrix|SlotLoadCurves|SlotSetProgram|SoundLoopReset|SpuSetAnyVoice|SsSetSerialVol|SsSetTableSize|SsUtGetDetVVol|SsUtGetProgAtr|SsUtSetDetVVol|SsVabTransBody|StCdInterrupt2|StepCarPhysics|StopRaceRumble|TitleModeEnter|TransformPoint|UpdateCarShake|UpdateDialogue|UpdateGlyphRow|UpdateImageRow|UpdateParticle|VSyncCallbacks|_SsContMainVol|_SsUtBuildADSR|_SsVmDamperOff|_SsVmGetSeqVol|_SsVmKeyOffNow|_SsVmPitchBend|_SsVmSeqKeyOff|_SsVmSetSeqVol|_padClrIntSio0|_padGetActSize|_padInitDirSeq|startIntrVSync|ApplyMatrixLV|ApplyMatrixSV|ApplySaveData|BuildSaveData|CD_getsector2|CardWriteFile|CdAudioUpdate|CdGetDiskType|CheckCallback|ChecksumBytes|DpqColorLight|DrawGlyphList|DrawHudSprite|DrawMenuFrame|DrawPauseMenu|EmitPolyF4Fog|EmitTiledRect|EvalRateCurve|FindTrackNode|GetGraphDebug|InitParticles|InitPlayerCar|InitRect16Row|LoadSceneStep|LoadTrackStep|MulRotMatrix0|OuterProduct0|PadClearPulse|PadInitDirect|PadRumbleStop|PadStartPulse|PlaySfxVolPan|RaceLoadReset|ResetCallback|ResetCarState|ResetEntryInt|SceneLoadStep|SeqMusicPause|SetDefDispEnv|SetDefDrawEnv|SetDrawOffset|SetFogNearFar|SetGeomOffset|SetGeomScreen|SetGraphDebug|SetGraphQueue|SetupRenderer|SetupViewport|SlotKeyOffAll|SlotSetupBank|SpuInitMalloc|SsSetTickMode|SsUtAllKeyOff|SsUtGetVagAtr|SsUtReverbOff|SsUtSetVagAtr|SsVabFakeHead|StCdInterrupt|StepMenuValue|StepRaceFrame|TitleMenuInit|UpdateCarGear|UpdateComCars|UploadCarClut|VSyncCallback|_SsContDamper|_SsContPanpot|_SsGetSeqData|_SsSndNextSep|_SsVmDamperOn|_SsVmKeyOnNow|_SsVmSeKeyOff|_dbl_shift_us|_spu_FgetRXXa|_spu_FsetRXXa|cd_read_retry|trapIntrVSync|AngleDiffAbs|BlendCarSpec|CD_getsector|CardReadFile|CdFileFinish|CdGetSector2|CdSearchFile|DS_cachefile|DS_searchdir|DeliverEvent|DrawBadgeRow|DrawHudDigit|DrawImageRow|DrawParticle|DrawRaceTime|GetVideoMode|HookEntryInt|InitBadgeRow|InitCarTable|InitGamePads|InitGlyphRow|InitImageRow|InitMenuGrid|InitRaceCars|LinkExchange|NormalColor3|PadEnableCom|PlaySfxBank2|QueueDrawStp|ReadCarInput|RotMatrixYXZ|RotMatrixZYX|RotTransPers|SeqSetVolume|SetBackColor|SetCarBounds|SetCdMixMode|SetRotMatrix|SetSemiTrans|SetTexWindow|SetVertexTri|SetVideoMode|SlotSetPause|SoundLoopSet|SoundStopAll|SpuSetReverb|SsUtReverbOn|StGetBackloc|StepNameChar|StopCallback|SubmitModel2|VSync_memclr|_SsContNrpn1|_SsContNrpn2|_SsSeqGetEof|_SsSndReplay|_SsSndSetVol|_SsVmPBVoice|_SsVmSeKeyOn|_card_format|_card_status|_dirFailAuto|_padChkVsync|_padStartCom|_patch_card2|_sio_control|_spu_FsetPCR|_spu_FsetRXX|setIntrVSync|startIntrDMA|AddTilePrim|AllocUiRows|ApplyMatrix|CD_datasync|CD_initintr|CarLoadStep|CardHasFile|CdAudioFade|CdAudioInit|CdAudioMute|CdDiskReady|CdGetSector|CdReadBreak|ClearImage2|DMACallback|DS_newmedia|DrawOTagEnv|EmitPolyFT3|EnableEvent|GetIntrMask|GetNameChar|InitComCars|LerpAngle12|MemCardSync|NormalColor|PadChkVsync|PadGetState|PadInfoComb|PadInfoMode|PadPollPort|PadStartCom|PointInQuad|ReverbReset|RotAverage3|RotAverage4|ScaleMatrix|SeqFadeStep|SetDispMask|SetDrawArea|SetDrawEnv2|SetDrawMode|SetDrawMove|SetFarColor|SetIntrMask|SetPriority|SetSXSYfifo|SetShadeTex|SoundResume|SoundUpdate|SquareRoot0|SsSepReplay|SsSepSetVol|SsSeqGetVol|SsSeqReplay|SsSeqSetVol|SsSetStereo|SsUtGetVVol|SsUtKeyOffV|SsUtSetVVol|StClearRing|StSetStream|StUnSetRing|StoreImage2|SysDeqIntRP|SysEnqIntRP|YawToOctant|_SsContRpn1|_SsContRpn2|_SsSndPause|_SsSndTempo|_SsVmKeyOff|_SsVmSetVol|_SsVmVSetUp|__floatsidf|_add_mant_d|_card_clear|_card_write|_mul_mant_d|_padIsVsync|_padStopCom|_patch_card|def_cbready|dma_execute|note2pitch2|restartIntr|trapIntrDMA|Atan2Table|CD_initvol|CardUpdate|CdControlB|CdControlF|CdDataSync|CdFileStep|CdIntToPos|CdPosToInt|CdReadMode|CdReadSync|CdSetDebug|ClearImage|ClearOTagR|CloseEvent|DMA_memclr|DisablePAD|EmitPolyF4|FinishRace|FlushCache|GPU_memset|GetDispEnv|GetDrawEnv|KeyOnCheck|LightColor|LoadImage2|LocalLight|MoveImage2|MulMatrix0|MulMatrix2|NormalClip|PadChkMtap|PadInfoAct|PadStopCom|PutDispEnv|PutDrawEnv|ResetGraph|RotMatrixX|RotMatrixY|RotMatrixZ|SetDrawEnv|SetDrawStp|SetFogNear|SetPolyFT3|SetRGBfifo|SetSZfifo3|SetSZfifo4|SetVertex0|SetVertex1|SetVertex2|SoundPause|SoundReset|SsSepClose|SsSepPause|SsSeqClose|SsSeqPause|SsUtKeyOff|SsUtKeyOnV|SsVabClose|StFreeRing|StartCARD2|StoreImage|SystemInit|TriggerSfx|XorBytes37|_96_remove|_SsSeqPlay|_SsSndPlay|_SsSndStop|_SsVmAlloc|_SsVmFlush|_SsVmKeyOn|_card_info|_card_load|_card_read|_comp_mant|_dbl_shift|_padIntPad|_padSetAct|_padSetCmd|_padSioRW2|_patch_gte|_patch_pad|_spu_FiDMA|_spu_Fw1ts|_spu_gcSPU|chkRC2wait|def_cbread|def_cbsync|firstfile2|note2pitch|setIntrDMA|setRC2wait|vmNoiseOff|AverageZ3|AverageZ4|CdControl|CdLastCom|CdLastPos|ClearOTag|DpqColor3|DrawOTag2|DrawUiBox|DrawUiRow|EnablePAD|FileClose|FileWrite|InitCARD2|InitUiRow|IntroInit|LoadClut2|LoadImage|LoadTPage|MoveImage|MulMatrix|OpenEvent|PAD_init2|PadSetAct|PadUpdate|ResetRCnt|RotMatrix|SetData32|SetLineF2|SetLineG2|SetMAC123|SetPolyF3|SetReverb|SetupRace|SlotKeyOn|SlotStart|SpuSetKey|SsSepPlay|SsSepStop|SsSeqOpen|SsSeqPlay|SsSeqStop|SsSetMVol|SsSetMono|SsUtKeyOn|StGetNext|StSetMask|StSetRing|StartCARD|StartPAD2|StartRCnt|StopCARD2|TestEvent|VsGetAddr|WaitEvent|_ExitCard|_SsNoteOn|_SsVmInit|__fixdfsi|_err_math|_new_card|_padSioRW|_spu_init|checkRECT|firstfile|get_alarm|set_alarm|startIntr|vmNoiseOn|AddPrims|AlignUp4|CD_flush|CD_ready|CdComstr|CdIntstr|CdStatus|ColorCol|ColorDpq|DpqColor|DrawCars|DrawOTag|DrawPrim|DrawRing|DrawSync|FileOpen|FileRead|FileSeek|GetTPage|HsvToRgb|InitCARD|InitGeom|InitHeap|InitPAD2|LoadClut|MainLoop|PAD_init|RotTrans|SetIR123|SpuStart|SpuWrite|Square12|SsStart2|StartPAD|StopCARD|StopPAD2|StopRCnt|_SpuInit|_SsClose|_SsStart|__adddf3|__divdf3|__muldf3|__negdf2|__subdf3|_addque2|_bu_init|_mainasu|_spu_Fr_|callback|get_mode|stopIntr|trapIntr|AddPrim|CD_init|CD_sync|CatPrim|CdFlush|CdRead2|CdReady|CdReset|GetClut|GetRCnt|InitPAD|LoadVab|PlaySfx|SetRCnt|SetSprt|SetTile|SpuFree|SpuInit|Square0|SsStart|StopPAD|_SsInit|__gtdf2|__ltdf2|_addque|_exeque|_getctl|_spu_Fr|_spu_Fw|_status|cb_data|cb_read|cd_read|csqrt_1|get_ofs|getintr|mem2mem|setIntr|AddDrv|AddSIO|CD_vol|CdInit|CdMode|CdRead|CdSync|DelDrv|DelSIO|GetODE|SetDQA|SetDQB|SetIR0|SetMem|SetRii|SsInit|_param|_reset|_spu_t|get_ce|get_cs|get_tw|memclr|ratan2|v_wait|CD_cw|CdMix|Intpl|VSync|_sync|sin_1|_clr|_cmp|_ctl|_cwb|_cwc|_drs|_dws|_otc|rcos|rsin|Lzc(?=\()',body))-{'ovl659_update_race'})
head='''#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#include "rr/frame_storage_types.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==(n),"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==(n),"PAL offset")
#include "rr/overlay659_frame_controller.h"
'''
(P/'header.c').write_text(head);pre=subprocess.check_output(['cc','-E','-Iinclude',str(P/'header.c')],text=True)
code=head+'\n';guarded={'PlayerCars','D_800ADAF0'}
for name in names:
 if name in guarded:code+=f'static struct {{u32 guard[4];__typeof__({name}) data;u32 tail[4];}} block_{name};\n#define {name} block_{name}.data\n'
 else:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
code+='''
static struct {u32 guard[4];RrOvl659Scratch data;u32 tail[4];} scratchBlock;
#define scratch scratchBlock.data
#undef RR_659_SCRATCH
#define RR_659_SCRATCH (&scratch)
static struct {u32 guard[4];RrRaceVehicle data[8];u32 tail[4];} extraBlock;
static struct {u32 guard[4];u32 data[3][1024];u32 tail[4];} streamBlock;
_Static_assert(sizeof(RrRaceVehicle)==0x320,"complete vehicle");
_Static_assert(sizeof(RrFrameStorage)==0x22778,"complete frame arena");
_Static_assert(sizeof(RrMenuSettings2C)==44,"settings owner");
_Static_assert(offsetof(RrMenuSettings2C,phase17)==23,"true AD4EF alias");
typedef uintptr_t (*Callback)(unsigned,uintptr_t*);
static Callback cb;void set_callback(Callback c){cb=c;}
'''
protos={}
for i,name in enumerate(functions):
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre);assert m,name
 ret=m[1].strip().split('\n')[-1].removeprefix('extern ');params=m[2]
 types=[] if params.strip() in ['', 'void'] else [t.strip() for t in params.split(',')];clean=[]
 for t in types:
  match=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if match:t=match[1]
  elif '*' not in t and len(t.split())>1:t=t.rsplit(' ',1)[0]
  clean.append(t)
 protos[name]=(ret,clean);args=','.join('(uintptr_t)a'+str(j) for j in range(len(clean))) or '0';decl=','.join(t+' a'+str(j) for j,t in enumerate(clean)) or 'void'
 code+=f'{ret} {name}({decl}){{uintptr_t a[10]={{{args}}};uintptr_t r=cb({i},a);'+('return ('+ret+')r;' if ret!='void' else '')+'}\n'
code+=body+'\nvoid *object(unsigned i){switch(i){\n'
for i,name in enumerate(names):code+=f'case {i}:return &{name};\n'
G=len(names);code+=f'case {G}:return &scratch;case {G+1}:return extraBlock.data;case {G+2}:return streamBlock.data;default:abort();}}}}\n'
code+='size_t object_size(unsigned i){switch(i){\n'
for i,name in enumerate(names):code+=f'case {i}:return sizeof({name});\n'
code+=f'case {G}:return sizeof(scratch);case {G+1}:return sizeof(extraBlock.data);case {G+2}:return sizeof(streamBlock.data);default:abort();}}}}\n'
code+='''
size_t scratch_offset(void){return offsetof(RrOvl659Scratch,word78);}
void pointers(unsigned f,unsigned c){D_800ACC14=(void*)&D_800ADAF0[f];scratch.pointers.cursor=(void*)streamBlock.data[c];scratch.pointers.order=streamBlock.data[(c+2)%3];}
void npc(unsigned slot,unsigned record){CarTable[slot]=record<2?&PlayerCars[record]:&extraBlock.data[record-2];}
'''
native=P/'native.c';native.write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Iinclude','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(native),'-o',str(P/'native.dylib')],check=True)
lib=C.CDLL(str(P/'native.dylib'));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t;lib.scratch_offset.restype=C.c_size_t
N={n:(lib.object(i),lib.object_size(i)) for i,n in enumerate(names)}
scratch=lib.object(G);scratch_size=lib.object_size(G);scratch_field=lib.scratch_offset();extra=lib.object(G+1);streams=lib.object(G+2)
frame=N['D_800ADAF0'][0];frame_size=N['D_800ADAF0'][1];assert frame_size==0x44EF0 and N['PlayerCars'][1]==0x640 and N['CarTable'][1]==64
assert min(scratch,extra,streams,frame)>0xffffffff and scratch_field==0x80 and scratch_size==0x88
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1F7FF000,0x2000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+659*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as f:
  elf=ELFFile(f)
  for n in ['.text','.rodata']:
   sec=elf.get_section_by_name(n)
   if sec:u.mem_write(sec['sh_addr']&0x1fffffff,sec.data())
read=lambda a,n:bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
addr=lambda n:SYMBOL_ADDRESSES[n] if n in SYMBOL_ADDRESSES else __import__('c_functions').address_of(n)
regions=[]
for n,(p,size) in N.items():
 if n in ['D_800ACC14','CarTable']:continue
 if n in guarded:regions.append((n,p-16,addr(n)-16,size+32))
 else:regions.append((n,p,addr(n),size))
regions += [('extra',extra-16,0x8017FFF0,8*0x320+32),('streams',streams-16,0x8018FFF0,12288+32)]
mappings=[(p,a,s) for n,p,a,s in regions]
def palpointer(value):
 for p,a,s in mappings:
  if p<=value<p+s:return a+value-p
 raise AssertionError(('unowned native pointer',hex(value)))
def nativepointer(value):
 for p,a,s in mappings:
  if a<=value<a+s:return p+value-a
 raise AssertionError(('unowned PAL pointer',hex(value)))
def scalar(n,value,native):
 p,size=N[n];data=(value&((1<<(size*8))-1)).to_bytes(size,'little')
 if native:C.memmove(p,data,size)
 else:write(addr(n),data)
def field(n,offset,data,native):
 if native:C.memmove(N[n][0]+offset,data,len(data))
 else:write(addr(n)+offset,data)
def half(off,value,native):field('PlayerCars',off,struct.pack('<H',value&65535),native)
def roots(f,c,native):
 if native:lib.pointers(f,c)
 else:write(0x800ACC14,struct.pack('<I',0x800ADAF0+f*0x22778));write(0x1F800000,struct.pack('<2I',0x80190000+c*4096,0x80190000+((c+2)%3)*4096))
def npc(slot,record,native):
 if native:lib.npc(slot,record)
 else:write(0x800FFC20+slot*4,struct.pack('<I',0x800ABEF8+record*0x320 if record<2 else 0x80180000+(record-2)*0x320))
def snapshot(native):
 parts=[C.string_at(p,s) if native else read(a,s) for n,p,a,s in regions]
 if native:
  ptrs=[palpointer(C.c_void_p.from_address(N['D_800ACC14'][0]).value)]+[palpointer(C.c_void_p.from_address(N['CarTable'][0]+j*8).value) for j in range(8)]
  cur=palpointer(C.c_void_p.from_address(scratch).value);order=palpointer(C.c_void_p.from_address(scratch+8).value)
  tail=C.string_at(scratch+16,0x70)+C.string_at(scratch+scratch_field,4)
  assert C.string_at(scratch+scratch_field+4,4)==b'\xA5'*4,'native tail padding'
  guards=C.string_at(scratch-16,16)+C.string_at(scratch+scratch_size,16)
 else:
  ptrs=[struct.unpack('<I',read(0x800ACC14,4))[0]]+list(struct.unpack('<8I',read(0x800FFC20,32)))
  cur,order=struct.unpack('<2I',read(0x1F800000,8));tail=read(0x1F800008,0x74);guards=read(0x1F7FFFF0,16)+read(0x1F80007C,16)
 return b''.join(parts)+struct.pack('<11I',*(ptrs+[cur,order]))+tail+guards
case=0;events=[];seen=0;errors=[];coverage=set();mutations=set();modes=set();root_changes=set()
words=[0,1,0xffffffff,0x7fffffff,0x80000000]
halves=[-32768,-1,0,1,32767]
heights=[0x80000000,-1,0,1,15,16,17,128,0x7fffffff]
def service(event,args,native):
 global seen
 name=functions[event];norm=[palpointer(v) if native and '*' in t else v&0xffffffff for v,t in zip(args,protos[name][1])];before=snapshot(native)
 if native:
  assert seen<len(events),(case,name,'unexpected service');e=events[seen];assert e[:2]==(event,norm),(case,seen,name,norm,e[:2]);assert e[2]==before,(case,seen,name,'complete world before service');serial=seen;seen+=1
 else:serial=len(events);events.append((event,norm,before));coverage.add(name)
 if case&64:
  changes={'D_800F2A64':(case*0x9e3779b9+event*97+serial*31)&0xffffffff}
  if name=='SetupViewport':
   changes.update(D_800AC5D0=halves[case%5],RaceState=[0,1,2,3,4,5,6,7,-1][(case//5)%9],RacePaused=[-128,-1,0,1,127][(case//7)%5])
   field('D_800F3A38',8,struct.pack('<H',[0,0x800,0xffff,0x8800][case%4]),native)
  if name in ['UpdatePauseMenu','ApplyPauseMenuChoice','SoundResume']:
   changes.update(ModeFrame=words[case%5],PauseCursor=[-128,-1,0,1,127][case%5])
  if name=='SoundPause':changes['PauseCursor']=[-128,-1,0,1,127][case%5]
  if name in ['UpdateLapTiming','SlotSetLevelProgram','StopRaceRumble']:
   changes['RaceState']=[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(case//3+serial)%11]
  if name=='UpdateRaceRanking':changes.update(D_800AC5A4=words[case%5],ScreenFrame=words[(case//5)%5])
  if name in ['func_80022CD4','CdAudioBeginPlay','SlotsStartForRace','CheckFinalLapEvent','AddScreenFadeTiles']:
   half(0x1EE,halves[case%5],native);half(0x1EA,(case//7)%2,native);half(0x1D8,halves[(case//5)%5],native);half(0x27A,halves[(case//25)%5],native)
  for n,v in changes.items():scalar(n,v,native)
  field('SelectionState',0x17,bytes([(case+serial)%8]),native)
  npc(0,(case+serial)%10,native)
  mutations.add(name)
 if case&128:
  roots((case+serial)%2,(case+serial)%3,native);root_changes.add(name)
 if name=='AddDrawAreaPrim':
  # Mutate the captured incoming packet, then publish an unrelated cursor.
  # The return is a third pointer; callers must use the returned full pointer.
  data=struct.pack('<2I',(case*17+serial)&0xffffffff,event)
  if native:C.memmove(args[1],data,8)
  else:write(args[1],data)
  result=0x80190000+((case+serial)%3)*4096+((event*16)&1023)
  alternate=0x80190000+((case+serial+1)%3)*4096
  if native:C.c_void_p.from_address(scratch).value=nativepointer(alternate)
  else:write(0x1F800000,struct.pack('<I',alternate))
  if case&64:scalar('D_80107190',heights[(case+serial)%len(heights)],native)
  return nativepointer(result) if native else result
 return 0
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
 uc.reg_write(UC_MIPS_REG_HI,0x11223344);uc.reg_write(UC_MIPS_REG_LO,0x55667788);uc.reg_write(UC_MIPS_REG_V0,result);uc.reg_write(UC_MIPS_REG_PC,ra)
for a in service_map:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
framefill=b'\xA5'*(frame_size+32);streamfill=b'\x96'*(12288+32)
def initialize(n,native):
 for name,p,a,size in regions:
  data=framefill if name=='D_800ADAF0' else streamfill if name=='streams' else bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 if native:C.memset(scratch-16,0xA5,scratch_size+32)
 else:write(0x1F7FFFF0,b'\xA5'*(0x7C+32))
 roots(n%2,n%3,native)
 for j in range(8):npc(j,(n+j)%10,native)
 values={'ModeFrame':[0xffffffff,0,1,23,24,25,49,50,51,73,74,75,174,175,176,0x7fffffff,0x80000000][n%17],
 'RaceState':[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(n//17)%11],
 'D_800AC5D0':[-32768,-1,0,1,14,15,24,25,37,75,32767][(n//187)%11],
 'RacePaused':[-128,-1,0,1,127][(n//3)%5],'PauseCursor':[-128,-1,0,1,2,127][(n//11)%6],
 'D_800AC5A4':words[n%5],'ScreenFrame':words[(n//5)%5], 'D_800AC6EC':[0,1,-1][(n//11)%3],
 'TrackLength':words[(n//7)%5], 'D_80107190':heights[(n//11)%len(heights)],'ReverseCourse':halves[(n//13)%5]}
 for name,v in values.items():scalar(name,v,native)
 field('SelectionState',0x17,bytes([n%8]),native)
 field('ovl659_finish_rank_limits',0,bytes([0,1,127,255][(n+j)%4] for j in range(4)),native)
 half(0x1EE,halves[n%5],native);half(0x1EA,(n//7)%2,native);half(0x1D8,halves[(n//5)%5],native);half(0x27A,halves[(n//25)%5],native)
 for j in range(8):
  data=struct.pack('<H',halves[(n+j)%5]&65535)
  if native:C.memmove(extra+j*0x320+0x1EE,data,2)
  else:write(0x80180000+j*0x320+0x1EE,data)
 field('D_800F3A38',8,struct.pack('<H',[0,0x800,0xffff,0x7ff,0x8800][(n//13)%5]),native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 8192
for case in range(count):
 events=[];seen=0;initialize(case,False);modes.add([0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(case//17)%11])
 for r in range(UC_MIPS_REG_0,UC_MIPS_REG_31+1):u.reg_write(r,0x5A5AA5A5)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800);u.reg_write(UC_MIPS_REG_HI,0x12345678);u.reg_write(UC_MIPS_REG_LO,0x87654321)
 u.emu_start(0x80114908,0x803FF800,count=30000);assert u.reg_read(UC_MIPS_REG_PC)==0x803FF800
 expected=snapshot(False);initialize(case,True);lib.ovl659_update_race()
 if errors:raise errors[0]
 assert seen==len(events),(case,'missing services');assert expected==snapshot(True),(case,'final full owners')
 if case and case%512==0:print(case,'cases',flush=True)
assert coverage==set(functions),(coverage,set(functions))
assert mutations==set(functions) and root_changes==set(functions),(mutations,root_changes)
print('PASS',count,('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS'),'services',len(coverage),'mutating',len(mutations),'root_changes',len(root_changes),flush=True)
if os.getenv('RR_AUDIT_JSON'):
 sha=lambda f:hashlib.sha256(resolve_c_source_path(f).read_bytes()).hexdigest()
 deps=subprocess.check_output(['cc','-MM','-Iinclude',str(native)],text=True).replace('\\\n',' ').split(':',1)[1].split()
 audit=dict(source=str(resolve_c_source_path(source)),source_sha256=sha(source),test_sha256=sha(__file__),dependencies={f:sha(f) for f in deps},cases=count,engine='compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS',services=sorted(coverage),mutating_services=sorted(mutations),root_replacing_services=sorted(root_changes),modes=sorted(modes),full_owner_guards=True,full_host_pointers=True,caller_save_hi_lo_poison=True,physical_settings_alias='D_800AD4EF = SelectionState.phase17',scratch_wire_bytes=0x7C,scratch_native_bytes=scratch_size,sanitizers=['undefined','bounds'])
 if os.getenv('RR_COMPILED_ELF'):audit['elf_sha256']=sha(os.environ['RR_COMPILED_ELF'])
 Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
