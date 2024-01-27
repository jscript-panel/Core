#pragma once

using VariantArgs = std::vector<_variant_t>;

static void** arg_helper(auto arg) { return reinterpret_cast<void**>(arg); }

template <typename T>
static pfc::array_t<T> pfc_array(size_t count)
{
	pfc::array_t<T> arr;
	arr.set_size(count);
	return arr;
}

#include "To.hpp"
#include "StringHelpers.hpp"
#include "FileHelper.hpp"
#include "JSONHelper.hpp"

#include "CallbackID.hpp"
#include "GUIDS.hpp"

#include "Component.hpp"
#include "PlaybackStatistics.hpp"
#include "MetadbIndex.hpp"

#include "JScriptPanel_h.h"
#include "ComArrayReader.hpp"
#include "ComArrayWriter.hpp"
#include "ComObject.hpp"
#include "ComObjectSingleton.hpp"
#include "JSDispatchBase.hpp"
#include "JSDispatch.hpp"
#include "JSDisposable.hpp"

#include "ContextMenuManager.hpp"
#include "DropAction.hpp"
#include "FileInfo.hpp"
#include "JSGraphics.hpp"
#include "JSImage.hpp"
#include "MainMenuManager.hpp"
#include "MenuObj.hpp"
#include "MetadbHandle.hpp"
#include "MetadbHandleList.hpp"
#include "PlayingItemLocation.hpp"
#include "Profiler.hpp"
#include "SelectionHolder.hpp"
#include "TextLayout.hpp"
#include "ThemeManager.hpp"
#include "TitleFormat.hpp"
#include "Tooltip.hpp"

#include "Console.hpp"
#include "Fb.hpp"
#include "Plman.hpp"
#include "Utils.hpp"
#include "Window.hpp"

#include "PanelBase.hpp"
#include "PanelManager.hpp"

#include "IStreamHelpers.hpp"
#include "AlbumArt.hpp"
#include "ImageHelpers.hpp"
