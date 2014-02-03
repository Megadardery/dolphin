// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "VideoCommon/TextureCacheBase.h"

namespace Null
{

class TextureCache : public ::TextureCache
{
public:
	TextureCache() {}

	void CompileShaders() override {};
	void DeleteShaders() override {};
	void ConvertTexture(TCacheEntryBase* entry, TCacheEntryBase* unconverted, void* palette, TlutFormat format) override {};

private:
	struct TCacheEntry : TCacheEntryBase
	{
		TCacheEntry(const TCacheEntryConfig& _config) : TCacheEntryBase(_config) {}
		~TCacheEntry() {}

		void Load(unsigned int width, unsigned int height,
			unsigned int expanded_width, unsigned int level) override {}

		void FromRenderTarget(u32 dstAddr, unsigned int dstFormat,
			PEControl::PixelFormat srcFormat, const EFBRectangle& srcRect,
			bool isIntensity, bool scaleByHalf, unsigned int cbufid,
			const float *colmat) override {}

		void CopyRectangleFromTexture(
			const TCacheEntryBase* source,
			const MathUtil::Rectangle<int> &srcrect,
			const MathUtil::Rectangle<int> &dstrect) override {}

		void Bind(unsigned int stage) override {}

		bool Save(const std::string& filename, unsigned int level) override {return false;}
	};

	~TextureCache() {}

	TCacheEntryBase* CreateTexture(const TCacheEntryConfig& config) override
	{
		return new TCacheEntry(config);
	}
};

} // Null name space
