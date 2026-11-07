"""Overlay 666 race controller: original MIPS/native service-state oracle.

Usage: python test_overlay666_race_controller.py [source.c [case_count]]
Default: public source, 32768 cases; generated files stay under build/tests.
RR_COMPILED_ELF optionally substitutes candidate MIPS code for compiler audits.
Services are deterministic mocks, not physical GPU/audio/serial emulation.
"""
from overlay_units import read_c_source, symbol_addresses
SYMBOL_ADDRESSES = symbol_addresses()
from pathlib import Path
import re,sys,subprocess,ctypes as C,struct,json,os
from unicorn import Uc,UC_ARCH_MIPS,UC_MODE_MIPS32,UC_MODE_LITTLE_ENDIAN,UC_HOOK_CODE
from unicorn.mips_const import *
os.chdir(Path(__file__).resolve().parents[2])
P=Path(os.environ.get('RR_TEST_DIR','build/tests/overlay666_race_controller'));P.mkdir(parents=True,exist_ok=True)
source=Path(sys.argv[1]) if len(sys.argv)>1 else Path('src/overlays/PAL/666/func_80116730.c')
body=read_c_source(source)
globals=sorted(set(re.findall(r'\b(?:(?:D_[0-9A-F]{8}|_spu_transferCallback|_spu_voice_centerNote|TrackObjectKeyframes|CdFileStateHandlers|_spu_mem_mode_unitM|_spu_rev_offsetaddr|_spu_rev_reserve_wa|CdFileIndexCurrent|SoundSequenceTable|_spu_AllocBlockNum|_spu_mem_mode_plus|_spu_mem_mode_unit|_spu_rev_startaddr|_svm_auto_kof_mode|CdAudioTrackTable|CdDirectoryBuffer|MenuStepDirection|TrackObjectStates|_snd_seq_tick_env|_spu_AllocLastNum|CD_read_dma_mode|CdFileSyncResult|HudNumberTargets|MenuSceneEntries|MenuTextRowCount|ModelTexturePage|MotionController|RaceLoadSubState|ScreenFadeColors|TrackLaneOffsets|TrackObjectCount|VoiceVolumeScale|_svm_stereo_mono|CarUpdateStages|CdFileBytesRead|CourseBestTotal|CourseLapCounts|FontGlyphWidths|HwCardEvTimeout|LinkWriteHandle|MenuGridRecords|ParticleEntries|RubberBandCurve|ScreenFadeState|SwCardEvTimeout|TrackObjectDefs|_SsMarkCallback|_spu_inTransfer|_spu_trans_mode|_svm_sreg_dirty|CarBoundsTable|CardDirEntries|CardFileHandle|CardPortResult|CdFileLocation|LapFrameCounts|LinkEntryState|LinkReadHandle|MenuFrameState|MenuScriptStep|ModelBankTable|ParticleSystem|RaceLoadCursor|RaceVoiceFlags|Rect16RowCount|SavedRaceState|SceneLoadState|SelectionState|TrackNodeCount|TrackStartNode|VSyncThreshold|_snd_seq_s_max|_snd_seq_t_max|_spu_transMode|_svm_envx_hist|_svm_vab_count|_svm_vab_start|_svm_vab_total|BadgeRowCount|CardPortState|CardPortTimer|CdFileSectors|CourseBestLap|GlyphRowCount|HwCardEvError|IntroTimeline|MenuItemCount|MenuScriptOps|ModeFuncTable|PadPulseState|PasswordBytes|PasswordChars|PendingModeId|RaceLoadState|RaceStandings|ReverseCourse|SelectionRows|SlotBankSetup|SwCardEvError|TotalRaceTime|_snd_openflag|_spu_isCalled|_spu_mem_mode|_spu_rev_flag|_svm_envx_ptr|_svm_sreg_buf|_svm_vab_used|spuVmMaxVoice|CarSnapshots|CarSpecTable|DrawBufIndex|FontGlyphMap|FrameCounter|MenuScriptId|MenuTextRows|PadButtonMap|PanGainTable|ReplayInputs|ReplayLength|SaveProgress|VBLANK_MINUS|_snd_ev_flag|_spu_RQvoice|_spu_keystat|_spu_memList|kMaxPrograms|BestLapTime|CdFileState|CdFileTable|ColorMatrix|CourseNames|HwCardEvIoe|HwCardEvNew|MenuScripts|NameCharset|PauseCursor|RaceContext|RaceRecords|RankEntries|ReplayFrame|ReplayState|ScreenFrame|SwCardEvIoe|SwCardEvNew|TrackLength|VabLoadArgs|ViewOffsetX|ViewOffsetY|ViewScreenH|_spu_RQmask|_svm_damper|_svm_vab_pg|_svm_vab_tn|_svm_vab_vh|BackColorB|BackColorG|BackColorR|CD_cbready|CD_status1|GPU_STATUS|GPU_printf|LapRecords|LoadCursor|MenuCursor|NumPlayers|PlayerCars|RacePaused|Rect16Rows|SaveBuffer|SoundSlots|SoundState|TrackNodes|UiRowCount|_snd_vmask|_spu_EVdma|_svm_okof1|_svm_okof2|_svm_okon1|_svm_okon2|_svm_onos1|_svm_onos2|_svm_orev1|_svm_orev2|_svm_rattr|_svm_voice|AtanTable|BadgeRows|CD_cbread|CD_cbsync|CD_comstr|CD_intstr|CD_status|CarInputs|DMA2_CHCR|DMA2_MADR|DMA6_CHCR|DMA6_MADR|FarColorB|FarColorG|FarColorR|GlyphRows|LinkState|LoadIndex|LoadState|ModeFrame|ModeGroup|R4StrPath|RaceState|RankTable|_ss_score|CD_debug|CD_nopen|CarTable|DMA2_BCR|DMA6_BCR|Dialogue|GPU_DATA|LapTimes|PadPorts|SinTable|_autopan|_autovol|_spu_RXX|_spu_env|_spu_tsa|_svm_cur|ComCars|NumCars|NumLaps|Options|SsFCALL|_spu_RQ|_svm_pg|_svm_tn|_svm_vh|CD_pos|UiRows|Vcount|_qout|DPCR|_qin)|ovl666_[a-z][a-z0-9_]*)\b',body)))
globals=[n for n in globals if not re.search(r"\b"+n+r"\s*\(",body)]
aliases={'D_800AC0E2':'half1EA','D_800AC0E6':'half1EE'}
globals=[x for x in globals if x not in aliases]
functions=sorted(set(re.findall(r'\b(?:func_[0-9A-F]{8}|DrawFontStringCenteredOffset|CdFileStateReloadDirectory|CdFileStateWaitShellClose|InterpolateObjectKeyframe|RelocateTrackLookupTables|DrawDialogueContinueMark|PatchOffsetTableTexPages|ScoreOvertakeOpportunity|SetupObjectLightMatrices|UpdateCarTrackProjection|ClearScreenOverlayState|GetSmoothedTrackHeading|InterpolateLanePointXYZ|LinkBattleEndModeUpdate|PlayStartCountdownVoice|ResetRankEntriesForCars|SetTrackObjectTransform|SpuSetTransferStartAddr|UpdateComCarTargetPoint|ClearTrackObjectStates|DrawFontStringCentered|DrawSplitScreenDivider|FindTrackNodeUnbounded|FormatMillisecondsTime|InitDefaultTimeRecords|InterpolateLanePointXZ|LookupTrackEventTable0|LookupTrackEventTable1|LookupTrackEventTable2|LookupTrackEventTable3|PatchModelBankTexPages|SoundMasterVolumeReset|SpuClearReverbWorkArea|SpuIsTransferCompleted|StopRaceAudioAndRumble|UpdateComCarLaneChange|ValidateRecordPassword|_SsVabOpenHeadWithMode|AddSpriteWithDrawMode|BuildCardHeaderStatic|CdAudioFindStreamFile|DrawSteeringIndicator|GetTrackTargetHeading|LookupTrackEventPair2|ProjectToTrackSegment|RestartMotionSequence|SetGteObjectTransform|SetNextModeByGameType|SetParticleBrightFade|SpuSetReverbModeParam|SsUtSetReverbFeedback|UpdateRaceChaseCamera|UpdateRaceIntroCamera|_SpuIsInAllocateArea_|_SsVmSelectToneAndVag|AllocMenuGridRecords|ApplyPauseMenuChoice|BuildRotMatrixAngles|CalcDistanceToPlayer|CalcRubberBandFactor|CdAudioFilterPending|CdAudioReadyCallback|CdFileStateStartRead|ComputeQuadMidpoints|ComputeStartDistance|DecodeRecordPassword|EncodeRecordPassword|EnterCriticalSection|FindTrackNodeScratch|FramesToMilliseconds|InitHudNumberTargets|InitMotionController|LookupTrackEventPair|PlanComCarCornerTurn|RelocateRelativeFile|RequestVramImageLoad|ResolveCarCollisions|SelectDialogueScript|SetGlyphRowImageBase|SetVoiceVolumeScaled|SoundFindNearestCars|SoundSetMasterVolume|TransformCourseModel|UpdateComCarOvertake|UpdatePlayerSteering|_SpuIsInAllocateArea|AllocCarSlotBuffers|ApplyRubberBandSpec|BuildCameraMatrices|BuildCardHeaderAnim|CD_set_test_parmnum|CdAudioRequestPause|CdAudioSetVolumeRaw|CdFileStateReadSync|ComputeWheelOffsets|ConstrainCarToTrack|CopyStringClearTail|CopyStringPadSpaces|DrawCourseInfoPanel|DrawFontStringRight|DrawGlyphListSloped|DrawHudRotatingQuad|DrawLinkEntryScreen|DrawLoadingTriangle|EmitSemiTransG3List|EmitSemiTransG4List|ExitCriticalSection|FindNearestCarAhead|FindTrackEventIndex|FindTrackNodeBehind|FindWheelTrackNodes|FormatDecimalDigits|FormatFrameTimeMMSS|GetTrackNodeHeading|InitCdFileDirectory|InitHudSpritePacket|InterpTrackPosition|LinkErrorModeUpdate|LoadComSlotBankStep|RelocateOffsetTable|ResetScreenCounters|ReturnFromException|SetMenuFrameOptions|SetModelTexturePage|SetTrackLaneOffsets|SlotSetLevelProgram|SoundListenerUpdate|SoundMasterFadeStep|SoundUpdateComSlots|SpuGetVoiceEnvelope|SsVabOpenHeadSticky|SsVabTransCompleted|StartCollisionShake|SteerCarTowardTrack|StepCarStateSpinOut|UnrelocateModelBank|UpdateDialoguePanel|UpdateLinkHandshake|UpdateRaceStandings|UpdateSelectionRank|_SsSetControlChange|_SsSetNrpnVabAttr10|_SsSetNrpnVabAttr11|_SsSetNrpnVabAttr12|_SsSetNrpnVabAttr13|_SsSetNrpnVabAttr14|_SsSetNrpnVabAttr15|_SsSetNrpnVabAttr16|_SsSetNrpnVabAttr17|_SsSetNrpnVabAttr18|_SsSetNrpnVabAttr19|_SsSetProgramChange|_SsSndSetReplayMode|_copy_memcard_patch|data_ready_callback|AddScreenFadeTiles|CardSystemShutdown|CdAudioSelectTrack|CdFileStateSetMode|CheckFinalLapEvent|CountProgressFlags|DrawMenuStepArrows|DrawModeSelectRows|DrawStartCountdown|EncodePasswordCode|FormatCountOfTotal|GameQueueTileTrans|GetTrackPointAhead|InitRaceHudPackets|IntegrateCarMotion|LinkWaitModeUpdate|LoadSoundBanksStep|LoadTrackNodesStep|ModeSelectMenuStep|PadSetRumbleParams|SetReverbDepthBoth|SetupMenuCarCamera|SoundCalcCarParams|SpuSetTransferMode|SsSetReservedVoice|SsUtSetReverbDelay|SsUtSetReverbDepth|StepCarStateNormal|SubmitCourseModel2|UpdateBadgeRowAnim|UpdateCameraMotion|UpdateCarTrackNode|UpdateSaveProgress|UpdateTrackObjects|_SsSetNrpnVabAttr0|_SsSetNrpnVabAttr1|_SsSetNrpnVabAttr2|_SsSetNrpnVabAttr3|_SsSetNrpnVabAttr4|_SsSetNrpnVabAttr5|_SsSetNrpnVabAttr6|_SsSetNrpnVabAttr7|_SsSetNrpnVabAttr8|_SsSetNrpnVabAttr9|_SsSndSetPauseMode|_card_sector_write|_padRecvAtLoadInfo|_padSendAtLoadInfo|_spu_getInTransfer|_spu_setInTransfer|_spu_setReverbAttr|AddOrSubtractByte|BuildObjectMatrix|CdAudioFilterStep|CdAudioResumeStep|CdFileBytesLoaded|CdFileStateSetloc|ClearHwCardEvents|ClearSwCardEvents|CountChoiceGlyphs|DecodeCourseIndex|DrawBadgeTitleBar|DrawBigFontString|DrawDigitString12|DrawDigitString16|EmitHudGlyph12Alt|EmitTexturedPanel|FindAxleTrackNode|FindTrackNodeAtXZ|GetRaceLoadCursor|GetTrackHeadingAt|InitDialogueState|InitSelectionRows|InterpCurveLinear|InterruptCallback|LinkWaitModeEnter|LoadRaceSoundStep|LoadTrackResource|PadSetRumbleFlags|PlayTrackEventSfx|RegisterModelBank|ResetVehicleShake|SetCarImpactState|SetIdentityMatrix|SetRaceLoadCursor|SetTrackStartNode|SetTrackWorkspace|SetupDrawDispEnvs|SetupViewLighting|SlotsStartForRace|SortRaceRankTable|SoundListenerInit|SoundQueueEnqueue|SoundReverbOffAll|SpawnParticlePair|SpuGetVoiceVolume|SpuSetReverbVoice|SsSetMarkCallback|SsUtGetReverbType|SsUtSetReverbType|StepCarStateSlide|SubmitCourseModel|UnpackChoiceGlyph|UpdateCarSteering|UpdateComCarDrive|UpdateComCarSpeed|UpdateDialogueBox|UpdateMenuTextRow|UpdateMotionTrack|UpdateRaceRanking|UpdateRumbleStrip|UpdateScrollTrack|VramImageLoadStep|_SsContBankChange|_SsContExpression|_SsReadDeltaValue|_padSetVsyncParam|_remove_ChgclrPAD|AddJitterSprites|ApplyCameraState|BuildLightMatrix|CdAudioBeginPlay|CdAudioPauseStep|CdAudioSetFilter|CdAudioSetTarget|CdAudioSetVolume|CdFileStateGetTN|CdFileStateRetry|CenterGlyphListX|ComSlotSetupBank|DrawDigitString8|DrawMeshPrimList|DrawSyncCallback|DrawTrackObjects|EmitPresetSprite|FindCurveSegment|FindFreeParticle|InitCameraMotion|InitMenuTextRows|InitViewLighting|LoadCarRangeStep|LoadSlotBankStep|LoadTrackObjects|LookupTrackEvent|PadRangeCheckSet|PollHwCardEvents|ReplayModeUpdate|RequestSceneLoad|ResetRankEntries|SaveCarSnapshots|SetTrackNodeData|ShuffleSlotOrder|SkipDialoguePage|SoundQueueUpdate|SplashLogoUpdate|SplashModeUpdate|SpuSetCommonAttr|SpuSetNoiseClock|SpuSetNoiseVoice|StartAttractDemo|SubmitModelFaces|TriggerWheelBump|UpdateCarHeading|UpdateMenuScript|UpdateScreenFade|UpdateWheelBumps|_SpuDataCallback|_SsContDataEntry|_SsUtResolveADSR|init_ring_status|AddDrawAreaPrim|AddDrawModePrim|AllocRect16Rows|AngleDiffSigned|BuildRotMatrixX|BuildRotMatrixY|BuildRotMatrixZ|ByteToCodeChars|CarLocalToWorld|CardWriteFileAt|CdAudioFadeStep|CdAudioLoopStep|CdAudioPlayStep|CdAudioStopStep|CdFileReadRange|CdFileReadStart|CdReadyCallback|ChangeClearRCnt|CheckRaceFinish|CloseLinkSerial|CodeCharsToByte|DrawLapTimeList|DrawMenuTextRow|DrawMeshQuadGT4|DrawRaceMinimap|GameQueueSprite|InitCameraState|InitMotionTrack|InitScrollTrack|InterpCurveEase|IntroScriptStep|LoadAllCarsStep|LoadCarSpecStep|LoadR4Directory|NormalColorCol3|NormalColorDpq3|PackChoiceGlyph|PadUpdateRumble|ReadInitPadFlag|ReplayModeEnter|RestartCallback|SelectModelBank|SeqMarkCallback|SetCarSpecTable|SetVehicleShake|SetupViewMatrix|SlotInterpCurve|Snd_SetPlayMode|SoundLoopUpdate|SoundQueueClear|SoundSystemInit|SplashModeEnter|SpuGetKeyStatus|SpuSetVoiceAttr|SsPitchFromNote|SsSeqCalledTbyT|SsSetSerialAttr|SsUtChangePitch|TitleMenuUpdate|UpdateCarEngine|UpdateEngineRpm|UpdateLapTiming|UpdateParticles|UpdatePauseMenu|UpdatePlayerCar|UpdateRect16Row|UpdateSoundSlot|WaitSwCardEvent|_SsContExternal|_SsContResetAll|_SsGetMetaEvent|_SsInitSoundSeq|_SsSetPitchBend|_SsSndCrescendo|_SsVmDoAllocate|_SsVmGetProgPan|_SsVmGetProgVol|_SsVmGetSeqLVol|_SsVmGetSeqRVol|_SsVmSetProgPan|_SsVmSetProgVol|_padCmdParaMode|_padInitSioMode|_padLoadActInfo|_padSetActAlign|_padSetMainMode|_padWaitRXready|_spu_FsetDelayR|_spu_FsetDelayW|_spu_FwriteByIO|_spu_note2pitch|_spu_pitch2note|AddPartialLine|AddTextSprites|AddTileGrid2x2|AllocBadgeRows|AllocGlyphRows|AllocImageRows|ApplyRotMatrix|AverageCarSpec|BuildRateCurve|CalcSteerLimit|CardFreeBlocks|CardReadFileAt|CardSystemInit|CdDataCallback|CdReadCallback|CdSyncCallback|ChangeClearPAD|CountCardFiles|DecayCarOffset|DrawHudNumber3|DrawHudNumber5|DrawLapCounter|DrawMeshQuadF4|DrawMeshQuadG4|EmitFadeSprite|EmitHudGlyph12|EmitPolyFT4Fog|EmitPolyFT4Raw|EmitSpriteGrid|FormatRaceTime|GetHwCardEvent|GetR4FileIndex|InitMenuArenas|InitMenuScript|LoadSfxVabStep|NormalColorCol|NormalColorDpq|OpenLinkSerial|OuterProduct12|PadRumbleStart|PadSetActAlign|PadSetMainMode|PlaceCarOnGrid|PointInQuadPtr|RemovePatchPad|RequestCarLoad|ScanCurveAhead|SeqMusicResume|SetColorMatrix|SetInitPadFlag|SetLightMatrix|SetMenuTextRow|SetTransMatrix|SlotLoadCurves|SlotSetProgram|SoundLoopReset|SpuSetAnyVoice|SsSetSerialVol|SsSetTableSize|SsUtGetDetVVol|SsUtGetProgAtr|SsUtSetDetVVol|SsVabTransBody|StCdInterrupt2|StepCarPhysics|StopRaceRumble|TitleModeEnter|TransformPoint|UpdateCarShake|UpdateDialogue|UpdateGlyphRow|UpdateImageRow|UpdateParticle|VSyncCallbacks|_SsContMainVol|_SsUtBuildADSR|_SsVmDamperOff|_SsVmGetSeqVol|_SsVmKeyOffNow|_SsVmPitchBend|_SsVmSeqKeyOff|_SsVmSetSeqVol|_padClrIntSio0|_padGetActSize|_padInitDirSeq|startIntrVSync|ApplyMatrixLV|ApplyMatrixSV|ApplySaveData|BuildSaveData|CD_getsector2|CardWriteFile|CdAudioUpdate|CdGetDiskType|CheckCallback|ChecksumBytes|DpqColorLight|DrawGlyphList|DrawHudSprite|DrawMenuFrame|DrawPauseMenu|EmitPolyF4Fog|EmitTiledRect|EvalRateCurve|FindTrackNode|GetGraphDebug|InitParticles|InitPlayerCar|InitRect16Row|LoadSceneStep|LoadTrackStep|MulRotMatrix0|OuterProduct0|PadClearPulse|PadInitDirect|PadRumbleStop|PadStartPulse|PlaySfxVolPan|RaceLoadReset|ResetCallback|ResetCarState|ResetEntryInt|SceneLoadStep|SeqMusicPause|SetDefDispEnv|SetDefDrawEnv|SetDrawOffset|SetFogNearFar|SetGeomOffset|SetGeomScreen|SetGraphDebug|SetGraphQueue|SetupRenderer|SetupViewport|SlotKeyOffAll|SlotSetupBank|SpuInitMalloc|SsSetTickMode|SsUtAllKeyOff|SsUtGetVagAtr|SsUtReverbOff|SsUtSetVagAtr|SsVabFakeHead|StCdInterrupt|StepMenuValue|StepRaceFrame|TitleMenuInit|UpdateCarGear|UpdateComCars|UploadCarClut|VSyncCallback|_SsContDamper|_SsContPanpot|_SsGetSeqData|_SsSndNextSep|_SsVmDamperOn|_SsVmKeyOnNow|_SsVmSeKeyOff|_dbl_shift_us|_spu_FgetRXXa|_spu_FsetRXXa|cd_read_retry|trapIntrVSync|AngleDiffAbs|BlendCarSpec|CD_getsector|CardReadFile|CdFileFinish|CdGetSector2|CdSearchFile|DS_cachefile|DS_searchdir|DeliverEvent|DrawBadgeRow|DrawHudDigit|DrawImageRow|DrawParticle|DrawRaceTime|GetVideoMode|HookEntryInt|InitBadgeRow|InitCarTable|InitGamePads|InitGlyphRow|InitImageRow|InitMenuGrid|InitRaceCars|LinkExchange|NormalColor3|PadEnableCom|PlaySfxBank2|QueueDrawStp|ReadCarInput|RotMatrixYXZ|RotMatrixZYX|RotTransPers|SeqSetVolume|SetBackColor|SetCarBounds|SetCdMixMode|SetRotMatrix|SetSemiTrans|SetTexWindow|SetVertexTri|SetVideoMode|SlotSetPause|SoundLoopSet|SoundStopAll|SpuSetReverb|SsUtReverbOn|StGetBackloc|StepNameChar|StopCallback|SubmitModel2|VSync_memclr|_SsContNrpn1|_SsContNrpn2|_SsSeqGetEof|_SsSndReplay|_SsSndSetVol|_SsVmPBVoice|_SsVmSeKeyOn|_card_format|_card_status|_dirFailAuto|_padChkVsync|_padStartCom|_patch_card2|_sio_control|_spu_FsetPCR|_spu_FsetRXX|setIntrVSync|startIntrDMA|AddTilePrim|AllocUiRows|ApplyMatrix|CD_datasync|CD_initintr|CarLoadStep|CardHasFile|CdAudioFade|CdAudioInit|CdAudioMute|CdDiskReady|CdGetSector|CdReadBreak|ClearImage2|DMACallback|DS_newmedia|DrawOTagEnv|EmitPolyFT3|EnableEvent|GetIntrMask|GetNameChar|InitComCars|LerpAngle12|MemCardSync|NormalColor|PadChkVsync|PadGetState|PadInfoComb|PadInfoMode|PadPollPort|PadStartCom|PointInQuad|ReverbReset|RotAverage3|RotAverage4|ScaleMatrix|SeqFadeStep|SetDispMask|SetDrawArea|SetDrawEnv2|SetDrawMode|SetDrawMove|SetFarColor|SetIntrMask|SetPriority|SetSXSYfifo|SetShadeTex|SoundResume|SoundUpdate|SquareRoot0|SsSepReplay|SsSepSetVol|SsSeqGetVol|SsSeqReplay|SsSeqSetVol|SsSetStereo|SsUtGetVVol|SsUtKeyOffV|SsUtSetVVol|StClearRing|StSetStream|StUnSetRing|StoreImage2|SysDeqIntRP|SysEnqIntRP|YawToOctant|_SsContRpn1|_SsContRpn2|_SsSndPause|_SsSndTempo|_SsVmKeyOff|_SsVmSetVol|_SsVmVSetUp|__floatsidf|_add_mant_d|_card_clear|_card_write|_mul_mant_d|_padIsVsync|_padStopCom|_patch_card|def_cbready|dma_execute|note2pitch2|restartIntr|trapIntrDMA|Atan2Table|CD_initvol|CardUpdate|CdControlB|CdControlF|CdDataSync|CdFileStep|CdIntToPos|CdPosToInt|CdReadMode|CdReadSync|CdSetDebug|ClearImage|ClearOTagR|CloseEvent|DMA_memclr|DisablePAD|EmitPolyF4|FinishRace|FlushCache|GPU_memset|GetDispEnv|GetDrawEnv|KeyOnCheck|LightColor|LoadImage2|LocalLight|MoveImage2|MulMatrix0|MulMatrix2|NormalClip|PadChkMtap|PadInfoAct|PadStopCom|PutDispEnv|PutDrawEnv|ResetGraph|RotMatrixX|RotMatrixY|RotMatrixZ|SetDrawEnv|SetDrawStp|SetFogNear|SetPolyFT3|SetRGBfifo|SetSZfifo3|SetSZfifo4|SetVertex0|SetVertex1|SetVertex2|SoundPause|SoundReset|SsSepClose|SsSepPause|SsSeqClose|SsSeqPause|SsUtKeyOff|SsUtKeyOnV|SsVabClose|StFreeRing|StartCARD2|StoreImage|SystemInit|TriggerSfx|XorBytes37|_96_remove|_SsSeqPlay|_SsSndPlay|_SsSndStop|_SsVmAlloc|_SsVmFlush|_SsVmKeyOn|_card_info|_card_load|_card_read|_comp_mant|_dbl_shift|_padIntPad|_padSetAct|_padSetCmd|_padSioRW2|_patch_gte|_patch_pad|_spu_FiDMA|_spu_Fw1ts|_spu_gcSPU|chkRC2wait|def_cbread|def_cbsync|firstfile2|note2pitch|setIntrDMA|setRC2wait|vmNoiseOff|AverageZ3|AverageZ4|CdControl|CdLastCom|CdLastPos|ClearOTag|DpqColor3|DrawOTag2|DrawUiBox|DrawUiRow|EnablePAD|FileClose|FileWrite|InitCARD2|InitUiRow|IntroInit|LoadClut2|LoadImage|LoadTPage|MoveImage|MulMatrix|OpenEvent|PAD_init2|PadSetAct|PadUpdate|ResetRCnt|RotMatrix|SetData32|SetLineF2|SetLineG2|SetMAC123|SetPolyF3|SetReverb|SetupRace|SlotKeyOn|SlotStart|SpuSetKey|SsSepPlay|SsSepStop|SsSeqOpen|SsSeqPlay|SsSeqStop|SsSetMVol|SsSetMono|SsUtKeyOn|StGetNext|StSetMask|StSetRing|StartCARD|StartPAD2|StartRCnt|StopCARD2|TestEvent|VsGetAddr|WaitEvent|_ExitCard|_SsNoteOn|_SsVmInit|__fixdfsi|_err_math|_new_card|_padSioRW|_spu_init|checkRECT|firstfile|get_alarm|set_alarm|startIntr|vmNoiseOn|AddPrims|AlignUp4|CD_flush|CD_ready|CdComstr|CdIntstr|CdStatus|ColorCol|ColorDpq|DpqColor|DrawCars|DrawOTag|DrawPrim|DrawRing|DrawSync|FileOpen|FileRead|FileSeek|GetTPage|HsvToRgb|InitCARD|InitGeom|InitHeap|InitPAD2|LoadClut|MainLoop|PAD_init|RotTrans|SetIR123|SpuStart|SpuWrite|Square12|SsStart2|StartPAD|StopCARD|StopPAD2|StopRCnt|_SpuInit|_SsClose|_SsStart|__adddf3|__divdf3|__muldf3|__negdf2|__subdf3|_addque2|_bu_init|_mainasu|_spu_Fr_|callback|get_mode|stopIntr|trapIntr|AddPrim|CD_init|CD_sync|CatPrim|CdFlush|CdRead2|CdReady|CdReset|GetClut|GetRCnt|InitPAD|LoadVab|PlaySfx|SetRCnt|SetSprt|SetTile|SpuFree|SpuInit|Square0|SsStart|StopPAD|_SsInit|__gtdf2|__ltdf2|_addque|_exeque|_getctl|_spu_Fr|_spu_Fw|_status|cb_data|cb_read|cd_read|csqrt_1|get_ofs|getintr|mem2mem|setIntr|AddDrv|AddSIO|CD_vol|CdInit|CdMode|CdRead|CdSync|DelDrv|DelSIO|GetODE|SetDQA|SetDQB|SetIR0|SetMem|SetRii|SsInit|_param|_reset|_spu_t|get_ce|get_cs|get_tw|memclr|ratan2|v_wait|CD_cw|CdMix|Intpl|VSync|_sync|sin_1|_clr|_cmp|_ctl|_cwb|_cwc|_drs|_dws|_otc|rcos|rsin|Lzc|ovl666_[a-z][a-z0-9_]*)(?=\()',body))-{'ovl666_update_race'})
head='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n)
#define RR_ASSERT_OFFSET(t,f,n)
#include "rr/overlay666_race_controller.h"
'''
(P/'native_header.c').write_text(head)
pre=subprocess.check_output(['cc','-E','-Iinclude','-I'+str(P),str(P/'native_header.c')],text=True)
protos={}
for name in functions:
 m=re.search(r'([\w *]+)\b'+name+r'\(([^;{}]*)\);',pre)
 assert m,name
 protos[name]=(m[1].strip().split('\n')[-1],m[2])
# Each object retains its public declared type. Frames and pointer owners remain full-sized.
code=head+'\n'
for name in globals:code+=f'__typeof__({name}) n_{name};\n#define {name} n_{name}\n'
for name,off in aliases.items():code+=f'#define {name} (n_D_800ABEF8[0].{off})\n'
code+='static RrGpuScratchPointers scratch;\n#undef RR_666_SCRATCH\n#define RR_666_SCRATCH (&scratch)\nstatic RrRaceVehicle extra[8];static u32 streams[3][1024];\n'
code+='typedef uintptr_t (*Callback)(unsigned,uintptr_t*);static Callback cb;void set_callback(Callback c){cb=c;}\n'
for i,name in enumerate(functions):
 ret,params=protos[name];types=[] if params.strip() in ['', 'void'] else [p.strip() for p in params.split(',')]
 # Prototypes here contain only types, except established GPU APIs with parameter names.
 clean=[]
 for t in types:
  m=re.fullmatch(r'(.*?\*+)\s*(\w+)',t)
  if m:t=m[1]
  elif '*' not in t and len(t.split())>1 and t not in ['unsigned long','struct w3g2p *']:
   t=t.rsplit(' ',1)[0]
  clean.append(t)
 params=','.join(t+' a'+str(j) for j,t in enumerate(clean)) or 'void'
 args=','.join('(uintptr_t)a'+str(j) for j in range(len(clean))) or '0'
 code+=f'{ret} {name}({params}){{uintptr_t args[10]={{{args}}};uintptr_t result=cb({i},args);'+('return ('+ret+')result;' if ret!='void' else '')+'}\n'
 protos[name]=(ret,clean)
code+=body+'\n'
code+='void *object(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return &n_{name};\n'
code+=f'case {len(globals)}:return &scratch;case {len(globals)+1}:return extra;case {len(globals)+2}:return streams;default:abort();}}}}\n'
code+='size_t object_size(unsigned id){switch(id){\n'
for i,name in enumerate(globals):code+=f'case {i}:return sizeof(n_{name});\n'
code+=f'case {len(globals)}:return sizeof(scratch);case {len(globals)+1}:return sizeof(extra);case {len(globals)+2}:return sizeof(streams);default:abort();}}}}\n'
code+='void pointers(unsigned root,unsigned cursor){D_800ACC14=(void*)&D_800ADAF0[root];scratch.cursor=(void*)streams[cursor];scratch.order=streams[(cursor+2)%3];}\n'
(P/(source.stem+'_native.c')).write_text(code)
subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Iinclude','-I'+str(P),'-fsanitize=undefined,bounds','-fno-sanitize-recover=all',str(P/(source.stem+'_native.c')),'-o',str(P/(source.stem+'_native.dylib'))],check=True)
lib=C.CDLL(str(P/(source.stem+'_native.dylib')));lib.object.restype=C.c_void_p;lib.object_size.restype=C.c_size_t
print('Native compiled',len(globals),'owners',len(functions),'services',flush=True)
G=len(globals);N={name:(lib.object(i),lib.object_size(i)) for i,name in enumerate(globals)}
scratch=lib.object(G);streams=lib.object(G+2);frame=N['D_800ADAF0'][0];frameSize=0x22778
assert scratch>0xffffffff and N['PlayerCars'][1]==0x640 and N['D_800ADAF0'][1]==0x44ef0 and lib.object_size(G)==16
u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);u.mem_map(0,0x400000);u.mem_map(0x1F800000,0x1000)
u.mem_write(0x10000,Path('assets/PAL/main.exe').read_bytes()[0x800:])
r4=Path('disc/PAL/R4.BIN').read_bytes();sector=struct.unpack_from('<I',r4,4+666*4)[0];u.mem_write(0x114878,r4[sector*2048:sector*2048+0x6000])
if os.getenv('RR_COMPILED_ELF'):
 from elftools.elf.elffile import ELFFile
 with open(os.environ['RR_COMPILED_ELF'],'rb') as file:u.mem_write(0x116730,ELFFile(file).get_section_by_name('.text').data())
def read(a,n):return bytes(u.mem_read(a&0x1fffffff,n))
def write(a,b):u.mem_write(a&0x1fffffff,b)
addr=lambda name:SYMBOL_ADDRESSES[name] if name in SYMBOL_ADDRESSES else __import__('c_functions').address_of(name)
regions=[(name,p,addr(name),size) for name,(p,size) in N.items() if name not in ['D_800ACC14','D_800ADAF0']]
mappings=[(p,a,size) for name,p,a,size in regions]+[(frame,0x800ADAF0,frameSize*2),(streams,0x80190000,12288)]
def palpointer(p):
 for base,a,size in mappings:
  if base<=p<base+size:return a+p-base
 raise AssertionError(('unowned native pointer',hex(p)))
def nativepointer(a):
 for p,base,size in mappings:
  if base<=a<base+size:return p+a-base
 raise AssertionError(('unowned PAL pointer',hex(a)))
def snapshot(native):
 parts=[C.string_at(p,size) if native else read(a,size) for name,p,a,size in regions]
 for i in range(2):parts.append(C.string_at(frame+i*frameSize+0x18,1) if native else read(0x800ADAF0+i*frameSize+0x18,1))
 if native:
  root=palpointer(C.c_void_p.from_address(N['D_800ACC14'][0]).value)
  cursor=palpointer(C.c_void_p.from_address(scratch).value);order=palpointer(C.c_void_p.from_address(scratch+8).value)
 else:root=struct.unpack('<I',read(0x800ACC14,4))[0];cursor,order=struct.unpack('<2I',read(0x1F800000,8))
 return b''.join(parts)+struct.pack('<3I',root,cursor,order)
def setscalar(name,value,native):
 p,size=N[name];b=(value&((1<<(size*8))-1)).to_bytes(size,'little')
 if native:C.memmove(p,b,size)
 else:write(addr(name),b)
def sethalf(offset,value,native):
 b=struct.pack('<H',value&65535)
 if native:C.memmove(N['PlayerCars'][0]+offset,b,2)
 else:write(0x800ABEF8+offset,b)
def setinput(value,native):
 b=struct.pack('<H',value&65535)
 if native:C.memmove(N['D_800F3A38'][0]+8,b,2)
 else:write(0x800F3A40,b)
events=[];observed=[];case=0;callbackError=[]
def service(event,args,native):
 name=functions[event];normalized=[palpointer(v) if native and '*' in t else v&0xffffffff for v,t in zip(args,protos[name][1])]
 (observed if native else events).append((event,normalized,snapshot(native)))
 if case&64:
  changes={}
  if name=='SetupViewport':
   # Values consumed after the first render preparation: fresh timer, phase, pause and input.
   changes={'D_800AC5D0':[-32768,-1,0,1,32767][case%5],'RaceState':[0,1,2,3,4,5,6,-1][(case//5)%8], 'RacePaused':[-128,-1,0,1,127][(case//7)%5]}
   setinput([0,0x800,0xffff,0x8800][case%4],native)
  if name in ['UpdatePauseMenu','ApplyPauseMenuChoice','SoundResume']:
   changes['ModeFrame']=[0x7fffffff,0x80000000,0xffffffff,0,74][case%5]
   changes['PauseCursor']=[-128,-1,0,1,127][case%5]
  if name=='SoundPause':changes['PauseCursor']=[-128,-1,0,1,127][case%5]
  if name in ['SlotSetLevelProgram','StopRaceRumble']:changes['RaceState']=[0,1,4,5,6,-1,0x80000000,0x7fffffff][case%8]
  if name=='UpdateRaceRanking':
   changes={'D_800AC5A4':[0,1,0x80000000,0x7fffffff][case%4],'ScreenFrame':[0xffffffff,0x7fffffff,0x80000000,0][case%4]}
  if name in ['func_80022CD4','CdAudioBeginPlay','SlotsStartForRace','CheckFinalLapEvent']:
   # Both aliases are in the same actual vehicle owner.
   sethalf(0x1ee,[0,-1,1,32767,-32768][case%5],native)
   sethalf(0x1ea,case%2,native)
   if name=='CheckFinalLapEvent':
    sethalf(0x1d8,[-32768,-1,0,1,32767][case%5],native)
    sethalf(0x27a,[-32768,-1,0,1,32767][(case//5)%5],native)
    sethalf(0x2aa,[-32768,-1,0,1,32767][(case//25)%5],native)
  for key,value in changes.items():setscalar(key,value,native)
 # Fresh frame roots after services; every later head must be reread.
 if case&128 and name in ['AddScreenFadeTiles','DrawSteeringIndicator','DrawStartCountdown','UpdatePauseMenu','UpdateRaceRanking']:
  root=(case+event)%2
  if native:C.c_void_p.from_address(N['D_800ACC14'][0]).value=frame+root*frameSize
  else:write(0x800ACC14,struct.pack('<I',0x800ADAF0+root*frameSize))
 if name=='AddDrawAreaPrim':
  result=0x80190000+((case+len(observed if native else events))%3)*4096+((event*16)&1023)
  scratchresult=0x80190000+((case+1)%3)*4096
  if native:C.c_void_p.from_address(scratch).value=nativepointer(scratchresult)
  else:write(0x1F800000,struct.pack('<I',scratchresult))
  return nativepointer(result) if native else result
 return 0
CB=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t))
def callback(event,args):
 try:return service(event,list(args[:len(protos[functions[event]][1])]),True)
 except BaseException as e:callbackError.append(e);return 0
cb=CB(callback);lib.set_callback(cb)
regs=[UC_MIPS_REG_A0,UC_MIPS_REG_A1,UC_MIPS_REG_A2,UC_MIPS_REG_A3]
def hook(uc,a,size,data):
 event=serviceMap[a];count=len(protos[functions[event]][1]);args=[uc.reg_read(r)&0xffffffff for r in regs[:min(count,4)]]
 if count>4:args+=list(struct.unpack('<'+'I'*(count-4),read(uc.reg_read(UC_MIPS_REG_SP)+16,4*(count-4))))
 value=service(event,args,False);uc.reg_write(UC_MIPS_REG_V0,value);uc.reg_write(UC_MIPS_REG_PC,uc.reg_read(UC_MIPS_REG_RA))
serviceMap={addr(name):i for i,name in enumerate(functions)}
for a in serviceMap:u.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
def initialize(n,native):
 for name,p,a,size in regions:
  data=bytes(((n*17+j*29+(a>>8))&255) for j in range(size))
  if native:C.memmove(p,data,size)
  else:write(a,data)
 data=b'\xa5'*(frameSize*2)
 if native:C.memmove(frame,data,len(data));lib.pointers(n%2,n%3)
 else:
  write(0x800ADAF0,data);write(0x800ACC14,struct.pack('<I',0x800ADAF0+(n%2)*frameSize));write(0x1F800000,struct.pack('<2I',0x80190000+(n%3)*4096,0x80190000+((n+2)%3)*4096))
 values={'ModeFrame':[0xffffffff,0,1,23,24,25,49,50,51,73,74,75,174,175,176,0x7fffffff,0x80000000][n%17],
 'RaceState':[0,1,2,3,4,5,6,7,-1,0x7fffffff,0x80000000][(n//17)%11],
 'D_800AC5D0':[-32768,-1,0,1,14,15,24,25,75,32767][(n//187)%10],
 'TotalRaceTime':[-1,0,1,0x80000000][n%4],'RacePaused':[-128,-1,0,1,127][(n//3)%5],
 'PauseCursor':[-128,-1,0,1,2,127][(n//11)%6],
 'D_800AC5A4':[0,1,2,0x80000000,0x7fffffff][n%5],
 'ScreenFrame':[0,1,0xffffffff,0x7fffffff,0x80000000][(n//5)%5]}
 for name,v in values.items():setscalar(name,v,native)
 sethalf(0x1ee,[0,-1,1,32767,-32768][n%5],native);sethalf(0x1ea,(n//7)%2,native)
 # Independent full input-owner buttons; high bits also exercise unsigned halfword reads.
 setinput([0,0x800,0xffff,0x7ff,0x8800][(n//13)%5],native)
count=int(sys.argv[2]) if len(sys.argv)>2 else 32768
coverage=set()
for case in range(count):
 events=[];observed=[];initialize(case,False)
 u.reg_write(UC_MIPS_REG_SP,0x803F0000);u.reg_write(UC_MIPS_REG_RA,0x803FF800)
 try:u.emu_start(0x80116730,0x803FF800,count=30000)
 except Exception:print('MIPS failure',case,hex(u.reg_read(UC_MIPS_REG_PC)));raise
 expected=snapshot(False);expectedFrames=read(0x800ADAF0,frameSize*2)
 initialize(case,True);lib.ovl666_update_race()
 if callbackError:raise callbackError[0]
 assert len(events)==len(observed),(case,len(events),len(observed))
 for j,(e,o) in enumerate(zip(events,observed)):
  assert e[:2]==o[:2],(case,j,functions[e[0]],e[1],o[1])
  if e[2]!=o[2]:raise AssertionError((case,j,functions[e[0]],next((i,x,y) for i,(x,y) in enumerate(zip(e[2],o[2])) if x!=y)))
  coverage.add(functions[e[0]])
 assert expected==snapshot(True),('final',case)
 assert expectedFrames==C.string_at(frame,frameSize*2),('full frame guards',case)
 if case and case%512==0:print(case,'cases',flush=True)
print(count,'cases passed;',('compiled MIPS' if os.getenv('RR_COMPILED_ELF') else 'original MIPS')+'/native full pointers; state before all services; complete frame owners;',len(coverage),'services',flush=True)
(P/'native_coverage.json').write_text(json.dumps(sorted(coverage),indent=2)+'\n')
