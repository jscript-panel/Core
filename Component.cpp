#include "stdafx.hpp"

namespace Component
{
	std::wstring get_path()
	{
		const auto path = wil::GetModuleFileNameW(core_api::get_my_instance());
		return FileHelper(path.get()).parent_path();
	}

	void log(std::string_view msg)
	{
		FB2K_console_formatter() << fmt::format("{}: {}", name_version, msg);
	}

	void popup(std::string_view msg)
	{
		popup_message::g_show(msg.data(), name.data());
	}

	cfgDialogPosition dialog_position(guids::dialog_position);
	cfg_string pin_to(guids::cfg_string_pin_to, "");

	DECLARE_COMPONENT_VERSION(name, version_string, about)

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
