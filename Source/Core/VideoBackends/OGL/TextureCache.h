// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <map>

#include "VideoBackends/OGL/GLUtil.h"
#include "VideoCommon/BPStructs.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/VideoCommon.h"

namespace OGL
{

class TextureCache : public ::TextureCache
{
public:
	TextureCache();
	static void DisableStage(unsigned int stage);
	static void SetStage();

private:
	struct TCacheEntry : TCacheEntryBase
	{
		GLuint texture;
		GLuint framebuffer;

		PC_TexFormat pcfmt;

		int gl_format;
		int gl_iformat;
		int gl_type;

		//TexMode0 mode; // current filter and clamp modes that texture is set to
		//TexMode1 mode1; // current filter and clamp modes that texture is set to

		TCacheEntry(int width, int height, int _maxlevel, bool _efbcopy);
		~TCacheEntry();

		void Load(unsigned int width, unsigned int height,
			unsigned int expanded_width, unsigned int level) override;

		void Bind(unsigned int stage) override;
		bool Save(const std::string& filename, unsigned int level) override;
	};

	~TextureCache();

	TCacheEntryBase* CreateTexture(unsigned int width, unsigned int height,
		unsigned int expanded_width, unsigned int maxlevel, PC_TexFormat pcfmt) override;

	TCacheEntryBase* CreateRenderTargetTexture(unsigned int scaled_tex_w, unsigned int scaled_tex_h) override;

	void FromRenderTargetToTexture(TCacheEntryBase* entry, PEControl::PixelFormat srcFormat, const EFBRectangle& srcRect,
		bool scaleByHalf, unsigned int cbufid, const float *colmat) override;
	size_t FromRenderTargetToRam(u8* dst, unsigned int dstFormat, PEControl::PixelFormat srcFormat, const EFBRectangle& srcRect,
		bool isIntensity, bool scaleByHalf) override;
};

bool SaveTexture(const std::string& filename, u32 textarget, u32 tex, int virtual_width, int virtual_height, unsigned int level);

}
