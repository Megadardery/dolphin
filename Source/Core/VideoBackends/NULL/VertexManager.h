// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <vector>

#include "VideoCommon/VertexManagerBase.h"

namespace Null
{

class VertexManager : public ::VertexManager
{
public:
	VertexManager();
	~VertexManager();
	NativeVertexFormat* CreateNativeVertexFormat() override;

protected:
	virtual void ResetBuffer(u32 stride);
	u16* GetIndexBuffer() { return &LocalIBuffer[0]; }
private:
	void vFlush(bool useDstAlpha) override;
	std::vector<u8> LocalVBuffer;
	std::vector<u16> LocalIBuffer;
};

}
