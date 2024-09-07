#include "stdafx.hpp"

namespace Component
{
	std::string about()
	{
		const std::string date = pfc::string(__DATE__).replace("  ", " ").get_ptr();
		const std::string msvc = std::to_string(_MSC_FULL_VER);

		return fmt::format(
			"Copyright(C) 2015 - 2024 marc2003\n\n"
			"Build: {}, {}\n\n"
			"foobar2000 SDK: {}\n"
			"Columns UI SDK: {}\n"
			"MSVC: {}.{}.{}",
			__TIME__,
			date,
			FOOBAR2000_SDK_VERSION,
			UI_EXTENSION_VERSION,
			msvc.substr(0, 2),
			msvc.substr(2, 2),
			msvc.substr(4)
		);
	}

	void log(std::string_view msg)
	{
		FB2K_console_formatter() << fmt::format("{}: {}", name_version, msg);
	}

	void popup(std::string_view msg, std::string_view title)
	{
		if (title.empty())
		{
			popup_message::g_show(msg.data(), name.data());
		}
		else
		{
			popup_message::g_show(msg.data(), title.data());
		}
	}

	cfgDialogPosition dialog_position(guids::dialog_position);

	DECLARE_COMPONENT_VERSION(name, version_string, about())

	class InstallationValidator : public component_installation_validator
	{
	public:
		bool is_installed_correctly() final
		{
			return test_my_name(dll.data()) && IsWindows10OrGreater() && core_version_info_v2::get()->test_version(2, 1, 0, 0);
		}
	};

	FB2K_SERVICE_FACTORY(InstallationValidator);
}
