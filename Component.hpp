#pragma once

namespace Component
{
	namespace urls
	{
		static constexpr wil::zwstring_view docs = L"https://jscript-panel.github.io/docs/";
		static constexpr wil::zwstring_view releases = L"https://github.com/jscript-panel/release/releases";
	}

	std::string get_resource_text(int id);
	std::wstring get_path();
	uint32_t get_threaded_process_flags();
	void popup(wil::zstring_view msg);

	static const std::string default_user_agent = fmt::format("{}/{}", dll, version_string);
	static constexpr uint32_t max_image_size = 64 * 1024 * 1024;

	extern cfg_string pin_to;
	extern cfg_window_placement_v2 window_placement;
}
