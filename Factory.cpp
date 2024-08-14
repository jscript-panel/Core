#include "stdafx.hpp"
#include "Font.hpp"

#if ENABLE_RESVG
extern void destroy_resvg_font_options();
#endif

namespace factory
{
	bool inited{};
	wil::com_ptr_nothrow<IWICImagingFactory2> imaging;
	wil::com_ptr_t<ID2D1Factory1> d2d;
	wil::com_ptr_t<IDWriteFactory> dwrite;
	wil::com_ptr_t<IDWriteGdiInterop> gdi_interop;
	wil::com_ptr_t<IDWriteTextFormat> error_text_format;
	wil::com_ptr_t<IDWriteTypography> typography;

	HRESULT create_error_text_format()
	{
		auto font = Font();
		font.m_size = 24.f;
		font.m_weight = DWRITE_FONT_WEIGHT_BOLD;

		auto params = FormatParams();
		params.m_text_alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		params.m_paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

		return font.create_format(error_text_format, params);
	}

	void init()
	{
		imaging = wil::CoCreateInstanceNoThrow<IWICImagingFactory2>(CLSID_WICImagingFactory);
		if (!imaging)
			return;

		const auto hr = []
			{
				RETURN_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
				RETURN_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwrite), dwrite.put_unknown()));
				RETURN_IF_FAILED(dwrite->GetGdiInterop(&gdi_interop));
				RETURN_IF_FAILED(dwrite->CreateTypography(&typography));
				RETURN_IF_FAILED(typography->AddFontFeature({ DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES, 1 }));
				RETURN_IF_FAILED(create_error_text_format());
				return S_OK;
			}();

		if SUCCEEDED(hr)
		{
			inited = true;
		}
		else
		{
			Component::log(format_hresult(hr));
		}
	}

	void reset()
	{
		d2d.reset();
		dwrite.reset();
		gdi_interop.reset();
		imaging.reset();
		error_text_format.reset();
		typography.reset();

#if ENABLE_RESVG
		destroy_resvg_font_options();
#endif
	}

	FB2K_ON_INIT_STAGE(init, init_stages::before_ui_init)
}
