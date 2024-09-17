#pragma once
#define _ATL_MODULES
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define WINVER _WIN32_WINNT_WIN10
#define NOMINMAX
#define ENABLE_RESVG NDEBUG

// stl
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include <concurrent_unordered_set.h>
#include <ppl.h>

// windows SDK
#include <WinSock2.h>
#include <Windows.h>
#include <windowsx.h>

#include <ActivScp.h>
#include <ComDef.h>
#include <MLang.h>

#include <d2d1_1.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>

// microsoft/wil
#include <wil/com.h>
#include <wil/filesystem.h>
#include <wil/win32_helpers.h>

// fmtlib/fmt
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/xchar.h>

// libcpr/cpr
#include <cpr/cpr.h>

// nlohmann/json
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <nlohmann/json.hpp>

// RazrFalcon/resvg
#if ENABLE_RESVG
#include <resvg.h>
#endif

// webmproject/libwebp
#include <webp/decode.h>
#include <webp/encode.h>

// foobar2000 SDK
#include <helpers/foobar2000+atl.h>
#include <helpers/atl-misc.h>
#include <helpers/metadb_handle_set.h>
#include <helpers/WindowPositionUtils.h>
#include <libPPUI/CDialogResizeHelper.h>
#include <libPPUI/CListControlComplete.h>
#include <libPPUI/CListControl-Cells.h>
#include <libPPUI/CListControlOwnerData.h>
#include <libPPUI/Controls.h>
#include <libPPUI/EditBoxFixes.h>
#include <libPPUI/pp-COM-macros.h>
#include <libPPUI/windowLifetime.h>
#include <pfc/filetimetools.h>
#include <pfc/string-conv-lite.h>
#include <SDK/coreDarkMode.h>
#include <SDK/file_info_filter_impl.h>

// Columns UI SDK
#include <columns_ui-sdk/ui_extension.h>

// Main
#include "JScriptPanel.hpp"
