#pragma once

namespace Component
{
	static const D2D1::ColorF White = D2D1::ColorF(D2D1::ColorF::White);
	static constexpr auto ErrorBackground = RGB(225, 60, 45);
	static constexpr wil::zwstring_view ErrorText = L"JavaScript error";

	static constexpr wil::zwstring_view Docs = L"https://jscript-panel.github.io/docs/";
	static constexpr wil::zwstring_view Releases = L"https://github.com/jscript-panel/release/releases";

	std::string get_resource_text(int id);
	std::wstring get_path();
	void popup(wil::zstring_view msg);

	static const std::string default_user_agent = fmt::format("{}/{}", dll, version_string);
	static constexpr wil::zwstring_view default_font_name = L"Segoe UI";
	static constexpr uint32_t max_image_size = 64 * 1024 * 1024;

	extern cfg_string pin_to;
	extern cfg_window_placement_v2 window_placement;
}
