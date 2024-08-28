#pragma once

namespace Component
{
	std::string about();
	void log(std::string_view msg);
	void popup(std::string_view msg, std::string_view title = "");

	static constexpr std::string_view name = "JScript Panel 3";
	static constexpr std::string_view dll = "foo_jscript_panel3.dll";

	static constexpr uint32_t version_number = 30700;
	static constexpr std::string_view version_string = "3.7.0";
	static const std::string name_version = fmt::format("{}{}", name, version_string.substr(1));
	static const std::string default_user_agent = fmt::format("{}/{}", dll, version_string);

	static constexpr std::wstring_view Docs = L"https://jscript-panel.github.io/docs/";
	static constexpr std::wstring_view Releases = L"https://github.com/jscript-panel/release/releases";

	static constexpr uint32_t max_image_size = 64 * 1024 * 1024;

	extern cfgDialogPosition dialog_position;
	extern cfg_string pin_to;
}
