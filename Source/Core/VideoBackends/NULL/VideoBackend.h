
#pragma once

#include "VideoCommon/VideoBackendBase.h"

namespace Null
{

class VideoBackend : public VideoBackendHardware
{
	bool Initialize(void *window_handle) override;
	void Shutdown() override;

	std::string GetName() const override;
	std::string GetDisplayName() const override;

	void Video_Prepare() override;
	void Video_Cleanup() override;

	void ShowConfig(void* parent) override;

	unsigned int PeekMessages() override;
};

}
