#pragma once
#define JSP_STRING_IMPL(x) #x
#define JSP_STRING(x) JSP_STRING_IMPL(x)

namespace Component
{
	void log(std::string_view msg);
	void popup(std::string_view msg, std::string_view title = "");

	static constexpr std::string_view name = "JScript Panel 3";
	static constexpr std::string_view dll = "foo_jscript_panel3.dll";

	static constexpr uint32_t version_number = 30605;
	static constexpr std::string_view version_string = "3.6.5";
	static constexpr std::string_view about =
		"Copyright (C) 2015-2024 marc2003\n\n"
		"Build: " __TIME__ ", " __DATE__ "\n\n"
		"foobar2000 SDK: " JSP_STRING(FOOBAR2000_SDK_VERSION) "\n"
		"Columns UI SDK: " UI_EXTENSION_VERSION;

	static const std::string name_version = fmt::format("{}{}", name, version_string.substr(1));
	static const std::string default_user_agent = fmt::format("{}/{}", dll, version_string);

	static constexpr std::wstring_view Docs = L"https://jscript-panel.github.io/docs/";
	static constexpr std::wstring_view Releases = L"https://github.com/jscript-panel/release/releases";

	static constexpr uint32_t max_image_size = 64 * 1024 * 1024;

	extern cfgDialogPosition dialog_position;
	extern cfg_string pin_to;
}
