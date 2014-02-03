
#pragma once

#include "VideoCommon/RenderBase.h"

namespace Null
{

class Renderer : public ::Renderer
{
public:
	Renderer();
	~Renderer();

	static void Init();
	static void Shutdown();

	void SetColorMask() override {};
	void SetBlendMode(bool forceUpdate) override {};
	void SetScissorRect(const EFBRectangle& rc) override {};
	void SetGenerationMode() override {};
	void SetDepthMode() override {};
	void SetLogicOpMode() override {};
	void SetDitherMode() override {};
	void SetSamplerState(int stage,int texindex, bool custom_tex) override {};
	void SetInterlacingMode() override {};
	void SetViewport() override {};

	void ApplyState(bool bUseDstAlpha) override {}
	void RestoreState() override {}

	void RenderText(const std::string& pstr, int left, int top, u32 color) override;
	u32 AccessEFB(EFBAccessType type, u32 x, u32 y, u32 poke_data) override;

	u16 BBoxRead(int index) override { return 0; };
	void BBoxWrite(int index, u16 value) override {};

	int GetMaxTextureSize() override { return 16*1024; };

	void ResetAPIState() override {};
	void RestoreAPIState() override {};

	TargetRectangle ConvertEFBRectangle(const EFBRectangle& rc) override;

	void SwapImpl(u32 xfbAddr, u32 fbWidth, u32 fbStride, u32 fbHeight, const EFBRectangle& rc, float Gamma) override;

	void ClearScreen(const EFBRectangle& rc, bool colorEnable, bool alphaEnable, bool zEnable, u32 color, u32 z) override;

	void ReinterpretPixelData(unsigned int convtype) override;

	bool SaveScreenshot(const std::string &filename, const TargetRectangle &rc) override;
};

}
