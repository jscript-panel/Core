#pragma once
#define JSP_STRING_IMPL(x) #x
#define JSP_STRING(x) JSP_STRING_IMPL(x)

namespace Component
{
	std::string get_resource_text(int id);
	std::wstring get_path();
	void popup(wil::zstring_view msg);

	static constexpr wil::zstring_view name = "JScript Panel 3";
	static constexpr wil::zstring_view dll = "foo_jscript_panel3.dll";

	static constexpr uint32_t version_number = 30418;
	static constexpr wil::zstring_view version_string = "3.4.18";
	static constexpr wil::zstring_view about =
		"Copyright (C) 2015-2024 marc2003\n\n"
		"Build: " __TIME__ ", " __DATE__ "\n\n"
		"foobar2000 SDK: " JSP_STRING(FOOBAR2000_SDK_VERSION) "\n"
		"Columns UI SDK: " UI_EXTENSION_VERSION;

	static const std::string name_version = fmt::format("{}{}", name, version_string.substr(1));
	static const std::string default_user_agent = fmt::format("{}/{}", dll, version_string);

	static constexpr wil::zwstring_view DefaultFont = L"Segoe UI";
	static constexpr wil::zwstring_view ErrorText = L"JavaScript error";
	static constexpr wil::zwstring_view Docs = L"https://jscript-panel.github.io/docs/";
	static constexpr wil::zwstring_view Releases = L"https://github.com/jscript-panel/release/releases";

	static constexpr uint32_t max_image_size = 64 * 1024 * 1024;

	static const auto White = D2D1::ColorF(D2D1::ColorF::White);
	static const auto DarkBackground = D2D1::ColorF(0.125f, 0.125f, 0.125f);
	static const auto ErrorBackground = D2D1::ColorF(0.882f, 0.235f, 0.176f);

	extern cfgDialogPosition dialog_position;
	extern cfg_string pin_to;
}
