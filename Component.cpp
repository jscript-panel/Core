#include "stdafx.hpp"

namespace Component
{
	std::string get_resource_text(int id)
	{
		const auto res = uLoadResource(core_api::get_my_instance(), uMAKEINTRESOURCE(id), "TEXT");
		return std::string(static_cast<const char*>(res->GetPointer()), res->GetSize());
	}

	std::wstring get_path()
	{
		const auto path = wil::GetModuleFileNameW(core_api::get_my_instance());
		return FileHelper(path.get()).parent_path();
	}

	uint32_t get_threaded_process_flags()
	{
		if (Fb::is_v2())
		{
			return threaded_process::flag_silent;
		}
		return threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item;
	}

	void popup(wil::zstring_view msg)
	{
		popup_message::g_show(msg.data(), name.data());
	}

	cfg_string pin_to(guids::cfg_string_pin_to, "");
	cfg_window_placement_v2 window_placement(guids::window_placement);

	namespace
	{
		advconfig_branch_factory advconfig_branch(name.data(), guids::advconfig_branch, advconfig_branch::guid_branch_tools, 0.0);
		advconfig_string_factory advconfig_pin_to("Playback Statistics Title Format", guids::advconfig_pin_to, guids::advconfig_branch, 0.0, "$lower($meta(artist,0) - %title%)", preferences_state::needs_restart);

		void init_stage()
		{
			string8 str;
			advconfig_pin_to.get(str);

			if (str != pin_to.get())
			{
				// If pattern has changed, nuke old data before GUID is registered
				pin_to.set(str);
				PlaybackStatistics::api()->erase_orphaned_data(guids::metadb_index);
			}

			MetadbIndex::init();
		}

		FB2K_ON_INIT_STAGE(init_stage, init_stages::after_config_read);
	}
}
