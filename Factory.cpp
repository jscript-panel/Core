#include "stdafx.hpp"
#include "WriteText.hpp"

#ifdef NDEBUG
extern void destroy_resvg_font_options();
#endif

namespace factory
{
	D2D1_DRAW_TEXT_OPTIONS d2d_text_options = D2D1_DRAW_TEXT_OPTIONS_CLIP;
	bool inited{};
	wil::com_ptr_nothrow<IWICImagingFactory2> imaging;
	wil::com_ptr_t<ID2D1Factory1> d2d;
	wil::com_ptr_t<IDWriteFactory> dwrite;
	wil::com_ptr_t<IDWriteGdiInterop> gdi_interop;
	wil::com_ptr_t<IDWriteTextFormat> error_text_format;

	void init()
	{
		if (IsWindows10OrGreater())
		{
			d2d_text_options |= D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT;
		}

		imaging = wil::CoCreateInstanceNoThrow<IWICImagingFactory2>(CLSID_WICImagingFactory);
		if (!imaging) return;

		const auto hr = []
			{
				RETURN_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
				RETURN_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwrite), reinterpret_cast<IUnknown**>(&dwrite)));
				RETURN_IF_FAILED(dwrite->GetGdiInterop(&gdi_interop));
				RETURN_IF_FAILED(WriteText::create_format(error_text_format, Component::default_font_name, 24.f, 700));
				RETURN_IF_FAILED(error_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
				RETURN_IF_FAILED(error_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
				return S_OK;
			}();

		inited = SUCCEEDED(hr);
	}

	void reset()
	{
		d2d.reset();
		dwrite.reset();
		gdi_interop.reset();
		imaging.reset();
		error_text_format.reset();

#ifdef NDEBUG
		destroy_resvg_font_options();
#endif
	}

	FB2K_ON_INIT_STAGE(init, init_stages::before_ui_init)
}
