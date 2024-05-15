#pragma once

namespace factory
{
	static constexpr D2D1_DRAW_TEXT_OPTIONS d2d_text_options = D2D1_DRAW_TEXT_OPTIONS_CLIP | D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT;

	extern bool inited;
	extern wil::com_ptr_nothrow<IWICImagingFactory2> imaging;
	extern wil::com_ptr_t<ID2D1Factory1> d2d;
	extern wil::com_ptr_t<IDWriteFactory> dwrite;
	extern wil::com_ptr_t<IDWriteGdiInterop> gdi_interop;
	extern wil::com_ptr_t<IDWriteTextFormat> error_text_format;

	void reset();
}
