// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.



// Null Backend Documentation
/*

try not to do anything in backend,
but everything in videocommon

*/

#include "Core/Host.h"

#include "VideoCommon/BPStructs.h"
#include "VideoCommon/CommandProcessor.h"
#include "VideoCommon/Fifo.h"
#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "VideoCommon/OpcodeDecoding.h"
#include "VideoCommon/PixelEngine.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoBackendBase.h"
#include "VideoCommon/VideoConfig.h"

#include "VideoBackends/NULL/PerfQuery.h"
#include "VideoBackends/NULL/PixelShaderCache.h"
#include "VideoBackends/NULL/Render.h"
#include "VideoBackends/NULL/TextureCache.h"
#include "VideoBackends/NULL/VertexManager.h"
#include "VideoBackends/NULL/VertexShaderCache.h"
#include "VideoBackends/NULL/VideoBackend.h"

namespace Null
{

std::string VideoBackend::GetName() const
{
	return "NULL";
}

std::string VideoBackend::GetDisplayName() const
{
	return "Null";
}

static void InitBackendInfo()
{
	g_Config.backend_info.APIType = API_NONE;
	g_Config.backend_info.bSupports3DVision = false;
	g_Config.backend_info.bSupportsDualSourceBlend = true;
	g_Config.backend_info.bSupportsEarlyZ = true;
	g_Config.backend_info.bSupportsOversizedViewports = true;
	g_Config.backend_info.bSupportsPrimitiveRestart = true;

	// aamodes
	const char* caamodes[] = {"None"};
	g_Config.backend_info.AAModes.assign(caamodes, caamodes + sizeof(caamodes)/sizeof(*caamodes));
}

void VideoBackend::ShowConfig(void *_hParent)
{
	InitBackendInfo();
	Host_ShowVideoConfig(_hParent, GetDisplayName(), "gfx_null");
}

bool VideoBackend::Initialize(void *window_handle)
{
	InitializeShared();
	InitBackendInfo();

	g_Config.Load((File::GetUserPath(D_CONFIG_IDX) + "gfx_null.ini").c_str());
	g_Config.GameIniLoad();
	g_Config.UpdateProjectionHack();
	g_Config.VerifyValidity();
	UpdateActiveConfig();

	// Do our OSD callbacks
	OSD::DoCallbacks(OSD::OSD_INIT);

	return true;
}

// This is called after Initialize() from the Core
// Run from the graphics thread
void VideoBackend::Video_Prepare()
{
	g_renderer = new Renderer;

	CommandProcessor::Init();
	PixelEngine::Init();

	BPInit();
	g_vertex_manager = new VertexManager;
	g_perf_query = new PerfQuery;
	Fifo_Init(); // must be done before OpcodeDecoder_Init()
	OpcodeDecoder_Init();
	IndexGenerator::Init();
	VertexShaderManager::Init();
	PixelShaderManager::Init();
	g_texture_cache = new TextureCache();
	Renderer::Init();
	VertexLoaderManager::Init();
	PixelShaderCache::Init();
	VertexShaderCache::Init();

	// Notify the core that the video backend is ready
	Host_Message(WM_USER_CREATE);
}

void VideoBackend::Shutdown()
{
	// Do our OSD callbacks
	OSD::DoCallbacks(OSD::OSD_SHUTDOWN);
}

void VideoBackend::Video_Cleanup() {

	if (g_renderer)
	{
		Fifo_Shutdown();

		// The following calls are NOT Thread Safe
		// And need to be called from the video thread
		Renderer::Shutdown();
		PixelShaderCache::Shutdown();
		VertexShaderCache::Shutdown();
		VertexLoaderManager::Shutdown();
		delete g_texture_cache;
		g_texture_cache = NULL;
		VertexShaderManager::Shutdown();
		PixelShaderManager::Shutdown();
		delete g_perf_query;
		g_perf_query = NULL;
		delete g_vertex_manager;
		g_vertex_manager = NULL;
		OpcodeDecoder_Shutdown();
		delete g_renderer;
		g_renderer = NULL;
	}
}

unsigned int VideoBackend::PeekMessages()
{
	return 0;
}

}
