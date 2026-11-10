#ifndef RR_RENDERER_SETUP_H
#define RR_RENDERER_SETUP_H
#include "game/data/cdfile2.h"
#include "rr/gpu.h"
#include "rr/rro_model.h"

typedef RroSceneScratchPrefix RendererScratch;
extern s32 MirrorRenderFlag;
extern GpuColorCommand DefaultScreenClearColor,LoadingScreenClearColor;


#if defined(mips)
RR_ASSERT_SIZE(RendererScratch, 0x7C);
RR_ASSERT_OFFSET(RendererScratch, depthShift60, 0x60);
RR_ASSERT_OFFSET(RendererScratch, renderFlags64, 0x64);
RR_ASSERT_OFFSET(RendererScratch, reserved68, 0x68);
RR_ASSERT_OFFSET(RendererScratch, viewport6C, 0x6C);
#endif
#endif
