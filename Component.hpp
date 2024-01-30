#pragma once

namespace Component
{
	static const auto White = D2D1::ColorF(D2D1::ColorF::White);
	static const auto DarkBackground = D2D1::ColorF(0.125f, 0.125f, 0.125f);
	static const auto ErrorBackground = D2D1::ColorF(0.882f, 0.235f, 0.176f);
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
