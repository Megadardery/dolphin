// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/StringUtil.h"
#include "Common/Logging/Log.h"

#include "Core/ConfigManager.h"

#include "VideoCommon/AVDump.h"

#if defined _WIN32 || defined HAVE_LIBAV
#include "VideoCommon/AVIDump.h"
#endif
#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "VideoCommon/VideoConfig.h"

AVDump* g_av_dump = new AVDump();

AVDump::AVDump()
{
	avi_filenumber = 0;
	s_avi_dump_started = false;
	s_get_screenshot.Clear();
	thread_is_working = false;
}

bool AVDump::DumpAudioEnabled()
{
	return false;
}

bool AVDump::DumpVideoEnabled()
{
	return SConfig::GetInstance().m_DumpFrames || s_get_screenshot.IsSet();
}

void AVDump::PushAIAudio(const s16* data, int samples, int samplerate, u64 ticks)
{

}

void AVDump::PushDTKAudio(const s16* data, int samples, int samplerate, u64 ticks)
{

}

// The input format is always bgra as it's the native format for gpus
void AVDump::PushVideo(const void* data, int width, int height, u64 ticks, bool upside_down)
{
	video_data = data;
	video_width = width;
	video_height = height;
	video_ticks = ticks;
	video_upside_down = upside_down;

	if (!worker)
	{
		worker = std::unique_ptr<std::thread>(new std::thread([] (AVDump* av) { av->EncodeLoop(); }, this));
	}

	start_event.Set();
	thread_is_working = true;
}

void AVDump::EncodeLoop()
{
	Common::SetCurrentThreadName("Video Encoder");
	while (true)
	{
		start_event.Wait();
		EncodeVideo();
		end_event.Set();
	}
}

void AVDump::SyncVideo()
{
	if (thread_is_working)
	{
		end_event.Wait();
		thread_is_working = false;
	}

	if (s_avi_dump_started && !SConfig::GetInstance().m_DumpFrames)
	{
#if defined _WIN32 || defined HAVE_LIBAV
		AVIDump::Stop();
#endif
		OSD::AddMessage("Stop dumping frames", 2000);
		s_avi_dump_started = false;
	}
}

void AVDump::EncodeVideo()
{
	DEBUG_LOG(VIDEO, "encode  video frame at %ld, %dx%d from %p", video_ticks, video_width, video_height, video_data);

	// OpenGL always flips the image, so we have to reverse it ourself
	// TODO: check if libav + png support flipped images (or negativ strides)
	if (video_upside_down)
	{
		swaped_data.resize(video_width * video_height * 4);
		for (int y = 0; y < video_height; ++y)
		{
			memcpy(&swaped_data[y * video_width * 4], &((u8*)video_data)[(video_height - y - 1) * video_width * 4], video_width * 4);
		}
		video_data = &swaped_data[0];
	}

	if (!s_avi_dump_started && SConfig::GetInstance().m_DumpFrames)
	{
		std::string filename = StringFromFormat("%sframedump%d.avi", File::GetUserPath(D_DUMPFRAMES_IDX).c_str(), avi_filenumber++);
#ifdef _WIN32
		s_avi_dump_started = AVIDump::Start(nullptr, video_width, video_height, filename);
#elif defined HAVE_LIBAV
		s_avi_dump_started = AVIDump::Start(video_width, video_height, filename);
#endif
		if (s_avi_dump_started)
		{
			OSD::AddMessage(StringFromFormat(
				"Dumping Frames to \"%s\" (%dx%d RGBA32)", filename.c_str(), video_width, video_height), 2000);
		}
		else
		{
			OSD::AddMessage("AVIDump Start failed", 2000);
		}
	}
	if (s_avi_dump_started)
	{
#if defined _WIN32 || defined HAVE_LIBAV
		AVIDump::AddFrame((u8*)video_data, video_width, video_height, video_ticks);
#endif
	}

	if (s_get_screenshot.IsSet())
	{
		bool saved_png = TextureToPng((const u8*)video_data, video_width*4, s_screenshot_path, video_width, video_height, false);
		if (saved_png)
		{
			OSD::AddMessage(StringFromFormat("Saved %i x %i %s", video_width, video_height, s_screenshot_path.c_str()));
		}
		else
		{
			OSD::AddMessage(StringFromFormat("Error saving %s", s_screenshot_path.c_str()));
		}
		s_get_screenshot.Clear();
	}
}

void AVDump::Screenshot(const std::string& filename)
{
	if (!s_get_screenshot.IsSet())
	{
		s_screenshot_path = filename;
		s_get_screenshot.Set();
	}
}

