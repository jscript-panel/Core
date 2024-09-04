#include "stdafx.hpp"
#include "Font.hpp"

#if ENABLE_RESVG
extern void destroy_resvg_font_options();
#endif

namespace factory
{
	bool inited{};
	std::vector<std::wstring> font_names;
	wil::com_ptr_t<IWICImagingFactory2> imaging;
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

	bool check_font_name(std::wstring_view name)
	{
		const auto it = std::ranges::find_if(font_names, [name](std::wstring_view font_name)
			{
				return js::compare_string(name, font_name);
			});

		return it != font_names.end();
	}

	std::wstring get_font_name(IDWriteLocalizedStrings* localised_strings)
	{
		uint32_t length{};

		if SUCCEEDED(localised_strings->GetStringLength(0, &length))
		{
			std::wstring name;
			name.resize(length);

			if SUCCEEDED(localised_strings->GetString(0, name.data(), length + 1))
				return name;
		}

		return DefaultFont.data();
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

	namespace
	{
		HRESULT init_fonts()
		{
			wil::com_ptr_t<IDWriteFontCollection> font_collection;
			RETURN_IF_FAILED(dwrite->GetSystemFontCollection(&font_collection, TRUE));

			const uint32_t family_count = font_collection->GetFontFamilyCount();
			wil::com_ptr_t<IDWriteFontFamily> font_family;
			wil::com_ptr_t<IDWriteLocalizedStrings> family_names;

			for (const uint32_t i : std::views::iota(0U, family_count))
			{
				if FAILED(font_collection->GetFontFamily(i, &font_family))
					continue;

				if FAILED(font_family->GetFamilyNames(&family_names))
					continue;

				const auto name = get_font_name(family_names.get());
				font_names.emplace_back(name);
			}

			js::sort_strings(font_names);
			return S_OK;
		}

		void init()
		{
			imaging = wil::CoCreateInstance<IWICImagingFactory2>(CLSID_WICImagingFactory2);
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
					RETURN_IF_FAILED(init_fonts());
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

		FB2K_ON_INIT_STAGE(init, init_stages::before_ui_init)
	}
}
