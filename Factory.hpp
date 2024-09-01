#pragma once

namespace factory
{
	static constexpr D2D1_DRAW_TEXT_OPTIONS d2d_text_options = D2D1_DRAW_TEXT_OPTIONS_CLIP | D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT;

	static constexpr std::wstring_view DefaultFont = L"Segoe UI";
	static constexpr std::wstring_view ErrorText = L"JavaScript error";

	static const auto DarkBackground = D2D1::ColorF(0.125f, 0.125f, 0.125f);
	static const auto ErrorBackground = D2D1::ColorF(0.882f, 0.235f, 0.176f);
	static const auto White = D2D1::ColorF(D2D1::ColorF::White);

	extern bool inited;
	extern std::vector<std::wstring> font_names;
	extern wil::com_ptr_t<IWICImagingFactory2> imaging;
	extern wil::com_ptr_t<ID2D1Factory1> d2d;
	extern wil::com_ptr_t<IDWriteFactory> dwrite;
	extern wil::com_ptr_t<IDWriteGdiInterop> gdi_interop;
	extern wil::com_ptr_t<IDWriteTextFormat> error_text_format;
	extern wil::com_ptr_t<IDWriteTypography> typography;

	bool check_font_name(std::wstring_view name);
	std::wstring get_font_name(IDWriteLocalizedStrings* localised_strings);
	void reset();
}
