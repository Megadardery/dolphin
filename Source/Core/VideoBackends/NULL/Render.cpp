// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cstdio>

#include "Core/Core.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VideoConfig.h"

#include "VideoBackends/NULL/Render.h"

namespace Null
{

// Init functions
Renderer::Renderer()
{
	g_Config.bRunning = true;
	UpdateActiveConfig();
}

Renderer::~Renderer()
{
}

void Renderer::Shutdown()
{
	g_Config.bRunning = false;
	UpdateActiveConfig();
}

void Renderer::Init()
{
}

void Renderer::RenderText(const std::string& text, int left, int top, u32 color)
{
	printf("RenderText: %s\n", text.c_str());
}

TargetRectangle Renderer::ConvertEFBRectangle(const EFBRectangle& rc)
{
	TargetRectangle result;
	result.left   = rc.left;
	result.top    = rc.top;
	result.right  = rc.right;
	result.bottom = rc.bottom;
	return result;
}

u32 Renderer::AccessEFB(EFBAccessType type, u32 x, u32 y, u32 poke_data)
{
	return 0;
}

void Renderer::ClearScreen(const EFBRectangle& rc, bool colorEnable, bool alphaEnable, bool zEnable, u32 color, u32 z)
{
}

void Renderer::ReinterpretPixelData(unsigned int convtype)
{
}

void Renderer::SwapImpl(u32 xfbAddr, u32 fbWidth, u32 fbStride, u32 fbHeight,const EFBRectangle& rc,float Gamma)
{
}

bool Renderer::SaveScreenshot(const std::string& filename, const TargetRectangle& rc)
{
	return false;
}
}
