#include "stdafx.hpp"

namespace Component
{
	std::wstring get_path()
	{
		const auto path = wil::GetModuleFileNameW(core_api::get_my_instance());
		return FileHelper(path.get()).parent_path();
	}

	void popup(wil::zstring_view msg)
	{
		popup_message::g_show(msg.data(), name.data());
	}

	cfgDialogPosition dialog_position(guids::dialog_position);
	cfg_string pin_to(guids::cfg_string_pin_to, "");

	namespace
	{
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

		advconfig_branch_factory advconfig_branch(name.data(), guids::advconfig_branch, advconfig_branch::guid_branch_tools, 0.0);
		advconfig_string_factory advconfig_pin_to("Playback Statistics Title Format", guids::advconfig_pin_to, guids::advconfig_branch, 0.0, "$lower($meta(artist,0) - %title%)", preferences_state::needs_restart);

		void init_stage()
		{
			string8 str;
			advconfig_pin_to.get(str);

			if (str != pin_to)
			{
				// If pattern has changed, nuke old data before GUID is registered
				pin_to = str;
				PlaybackStatistics::api()->erase_orphaned_data(guids::metadb_index);
			}

			MetadbIndex::init();
		}

		FB2K_ON_INIT_STAGE(init_stage, init_stages::after_config_read)
	}
}
