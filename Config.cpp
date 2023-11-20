#include "stdafx.hpp"

namespace Component
{
	cfg_string pin_to(guids::cfg_string_pin_to, "");
	cfg_window_placement_v2 window_placement(guids::window_placement);

	namespace
	{
		advconfig_branch_factory advconfig_branch(Component::name.data(), guids::advconfig_branch, advconfig_branch::guid_branch_tools, 0);
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
