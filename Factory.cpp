#include "stdafx.hpp"

#ifdef NDEBUG
extern void destroy_resvg_font_options();
#endif

D2D1_DRAW_TEXT_OPTIONS g_d2d_text_options = D2D1_DRAW_TEXT_OPTIONS_CLIP;
bool g_factory_init{};
wil::com_ptr_nothrow<IWICImagingFactory2> g_imaging_factory;
wil::com_ptr_t<ID2D1Factory> g_d2d_factory;
wil::com_ptr_t<IDWriteFactory> g_dwrite_factory;
wil::com_ptr_t<IDWriteGdiInterop> g_gdi_interop;

void factory_init()
{
	if (IsWindows10OrGreater())
	{
		g_d2d_text_options |= D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT;
	}

	g_imaging_factory = wil::CoCreateInstanceNoThrow<IWICImagingFactory2>(CLSID_WICImagingFactory);
	if (!g_imaging_factory) return;

	if FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_d2d_factory)) return;
	if FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(g_dwrite_factory), reinterpret_cast<IUnknown**>(&g_dwrite_factory))) return;
	if FAILED(g_dwrite_factory->GetGdiInterop(&g_gdi_interop)) return;

	g_factory_init = true;
}

void factory_reset()
{
	g_d2d_factory.reset();
	g_dwrite_factory.reset();
	g_gdi_interop.reset();
	g_imaging_factory.reset();

#ifdef NDEBUG
	destroy_resvg_font_options();
#endif
}

FB2K_ON_INIT_STAGE(factory_init, init_stages::before_ui_init)
