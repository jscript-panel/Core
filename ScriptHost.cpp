#include "stdafx.hpp"
#include "ScriptHost.hpp"

ScriptHost::ScriptHost(PanelBase* panel)
	: m_panel(panel)
	, m_console(ComObjectSingleton<Console>::get())
	, m_fb(ComObjectSingleton<Fb>::get())
	, m_plman(ComObjectSingleton<Plman>::get())
	, m_utils(ComObjectSingleton<Utils>::get())
	, m_window(new ImplementCOMRefCounter<Window>(panel)) {}

HRESULT ScriptHost::InitCallbackMap()
{
	RETURN_HR_IF(E_POINTER, !m_script_root);

	for (const auto& [id, name] : g_callback_name_map)
	{
		auto cname = const_cast<LPOLESTR>(name.data());
		DISPID dispId{};

		if SUCCEEDED(m_script_root->GetIDsOfNames(IID_NULL, &cname, 1, LOCALE_USER_DEFAULT, &dispId))
		{
			m_callback_map.emplace(id, dispId);

			switch (id)
			{
			case CallbackID::on_char:
			case CallbackID::on_focus:
			case CallbackID::on_key_down:
			case CallbackID::on_key_up:
				m_panel->m_grab_focus = true;
				break;
			case CallbackID::on_drag_drop:
			case CallbackID::on_drag_enter:
			case CallbackID::on_drag_leave:
			case CallbackID::on_drag_over:
				m_panel->m_drag_drop = true;
				break;
			}
		}
	}
	return S_OK;
}

HRESULT ScriptHost::ParseImports()
{
	for (auto&& path : m_imports)
	{
		FileHelper f(path);
		if (f.is_file())
		{
			RETURN_IF_FAILED(ParseScript(f.read(), path));
		}
		else
		{
			FB2K_console_formatter() << fmt::format("{}: file not found {}", m_build_string, path);
		}
	}
	return S_OK;
}

HRESULT ScriptHost::ParseScript(std::string_view code, std::string_view path)
{
	m_context_to_path_map.emplace(++m_last_source_context, path);
	const auto wcode = js::to_wide(code);
	return m_parser->ParseScriptText(wcode.data(), nullptr, nullptr, nullptr, m_last_source_context, 0, SCRIPTTEXT_HOSTMANAGESSOURCE | SCRIPTTEXT_ISVISIBLE, nullptr, nullptr);
}

STDMETHODIMP ScriptHost::GetDocVersionString(BSTR*)
{
	return E_NOTIMPL;
}

STDMETHODIMP ScriptHost::GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** ppunk, ITypeInfo**)
{
	if (WI_IsFlagSet(mask, SCRIPTINFO_IUNKNOWN))
	{
		RETURN_HR_IF_NULL(E_INVALIDARG, name);

		if (js::compare_string(name, L"console"))
		{
			m_console->AddRef();
			*ppunk = m_console.get();
			return S_OK;
		}
		else if (js::compare_string(name, L"fb"))
		{
			m_fb->AddRef();
			*ppunk = m_fb.get();
			return S_OK;
		}
		else if (js::compare_string(name, L"plman"))
		{
			m_plman->AddRef();
			*ppunk = m_plman.get();
			return S_OK;
		}
		else if (js::compare_string(name, L"utils"))
		{
			m_utils->AddRef();
			*ppunk = m_utils.get();
			return S_OK;
		}
		else if (js::compare_string(name, L"window"))
		{
			m_window->AddRef();
			*ppunk = m_window.get();
			return S_OK;
		}
	}
	return TYPE_E_ELEMENTNOTFOUND;
}

STDMETHODIMP ScriptHost::GetLCID(LCID*)
{
	return E_NOTIMPL;
}

STDMETHODIMP ScriptHost::OnEnterScript()
{
	return S_OK;
}

STDMETHODIMP ScriptHost::OnLeaveScript()
{
	return S_OK;
}

STDMETHODIMP ScriptHost::OnScriptError(IActiveScriptError* err)
{
	RETURN_HR_IF_NULL(E_POINTER, err);

	const std::string error_text = GetErrorText(err);
	FB2K_console_formatter() << error_text;
	fb2k::inMainThread([error_text] { Component::popup(error_text); });

	MessageBeep(MB_ICONASTERISK);
	if (m_script_engine) m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	m_panel->Reset();
	return S_OK;
}

STDMETHODIMP ScriptHost::OnScriptTerminate(const VARIANT*, const EXCEPINFO*)
{
	return E_NOTIMPL;
}

STDMETHODIMP ScriptHost::OnStateChange(SCRIPTSTATE state)
{
	m_state = state;
	return S_OK;
}

bool ScriptHost::CheckCallbackID(CallbackID id)
{
	return Connected() && m_callback_map.contains(id);
}

bool ScriptHost::Connected()
{
	return m_state == SCRIPTSTATE_CONNECTED;
}

bool ScriptHost::Error()
{
	return m_state == SCRIPTSTATE_DISCONNECTED;
}

bool ScriptHost::Initialise()
{
	ParsePreprocessor();

	const auto hr = [this]
		{
			static constexpr CLSID jscript9clsid = { 0x16d51579, 0xa30b, 0x4c8b, { 0xa2, 0x76, 0x0f, 0xf4, 0xdc, 0x41, 0xe7, 0x55 } };
			static constexpr long version = 1L + SCRIPTLANGUAGEVERSION_5_8;
			auto var = _variant_t(version);

			m_script_engine = wil::CoCreateInstanceNoThrow<IActiveScript>(jscript9clsid);

			RETURN_HR_IF(E_FAIL, !factory::inited);
			RETURN_HR_IF(E_FAIL, !m_script_engine);
			RETURN_IF_FAILED(m_script_engine->QueryInterface(&m_script_property));
			RETURN_IF_FAILED(m_script_property->SetProperty(SCRIPTPROP_INVOKEVERSIONING, nullptr, &var));
			RETURN_IF_FAILED(m_script_engine->SetScriptSite(this));
			RETURN_IF_FAILED(m_script_engine->QueryInterface(&m_parser));
			RETURN_IF_FAILED(m_parser->InitNew());
			RETURN_IF_FAILED(m_script_engine->AddNamedItem(L"console", SCRIPTITEM_ISVISIBLE));
			RETURN_IF_FAILED(m_script_engine->AddNamedItem(L"fb", SCRIPTITEM_ISVISIBLE));
			RETURN_IF_FAILED(m_script_engine->AddNamedItem(L"plman", SCRIPTITEM_ISVISIBLE));
			RETURN_IF_FAILED(m_script_engine->AddNamedItem(L"utils", SCRIPTITEM_ISVISIBLE));
			RETURN_IF_FAILED(m_script_engine->AddNamedItem(L"window", SCRIPTITEM_ISVISIBLE));
			RETURN_IF_FAILED(m_script_engine->SetScriptState(SCRIPTSTATE_CONNECTED));
			RETURN_IF_FAILED(m_script_engine->GetScriptDispatch(nullptr, &m_script_root));
			RETURN_IF_FAILED(ParseImports());
			RETURN_IF_FAILED(ParseScript(m_panel->m_config.m_code, "<main>"));
			RETURN_IF_FAILED(InitCallbackMap());
			return S_OK;
		}();

	if FAILED(hr)
	{
		Reset();
		return false;
	}

	return true;
}

bool ScriptHost::InvokeMouseRbtnUp(WPARAM wp, LPARAM lp)
{
	if (IsKeyPressed(VK_LSHIFT) && IsKeyPressed(VK_LWIN)) return false;

	const auto dispId = GetDISPID(CallbackID::on_mouse_rbtn_up);
	if (!dispId) return false;

	VariantArgs args = { wp, GET_Y_LPARAM(lp), GET_X_LPARAM(lp) }; // reversed
	_variant_t result;

	DISPPARAMS params{};
	params.rgvarg = args.data();
	params.cArgs = js::sizeu(args);

	if FAILED(m_script_root->Invoke(*dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &result, nullptr, nullptr)) return false;
	if FAILED(VariantChangeType(&result, &result, 0, VT_BOOL)) return false;
	return js::to_bool(result.boolVal);
}

std::optional<DISPID> ScriptHost::GetDISPID(CallbackID id)
{
	if (m_script_root && Connected())
	{
		const auto it = m_callback_map.find(id);
		if (it != m_callback_map.end())
		{
			return it->second;
		}
	}
	return std::nullopt;
}

std::string ScriptHost::ExtractValue(std::string_view str)
{
	static constexpr char q = '"';
	const size_t first = str.find_first_of(q);
	const size_t last = str.find_last_of(q);
	if (first < last && last < str.length())
	{
		return std::string(str.substr(first + 1, last - first - 1));
	}
	return std::string();
}

std::string ScriptHost::GetErrorText(IActiveScriptError* err)
{
	DWORD ctx{}, line{};
	EXCEPINFO excep{};
	LONG charpos{};
	Strings errors;
	wil::unique_bstr sourceline;

	errors.emplace_back(m_build_string);

	if SUCCEEDED(err->GetExceptionInfo(&excep))
	{
		if (excep.pfnDeferredFillIn)
		{
			(*excep.pfnDeferredFillIn)(&excep);
		}

		if (excep.bstrSource)
		{
			errors.emplace_back(js::from_wide(excep.bstrSource));
			SysFreeString(excep.bstrSource);
		}

		if (excep.bstrDescription)
		{
			errors.emplace_back(js::from_wide(excep.bstrDescription));
			SysFreeString(excep.bstrDescription);
		}

		if (excep.bstrHelpFile)
		{
			SysFreeString(excep.bstrHelpFile);
		}
	}

	if SUCCEEDED(err->GetSourcePosition(&ctx, &line, &charpos))
	{
		const auto it = m_context_to_path_map.find(ctx);
		if (it != m_context_to_path_map.end())
		{
			errors.emplace_back(fmt::format("File: {}", it->second));
		}
		errors.emplace_back(fmt::format("Line: {}, Col: {}", line + 1, charpos + 1));
	}

	if SUCCEEDED(err->GetSourceLineText(&sourceline))
	{
		errors.emplace_back(js::from_wide(sourceline.get()));
	}

	const std::string error_text = fmt::format("{}", fmt::join(errors, CRLF.data()));
	return error_text;
}

void ScriptHost::AddImport(std::string_view str)
{
	static const std::vector<StringPair> replacements =
	{
		{ "%fb2k_profile_path%", js::from_wide(Path::profile()) },
		{ "%fb2k_component_path%", js::from_wide(Path::component()) },
	};

	std::string path = ExtractValue(str);
	if (path.empty()) return;

	for (const auto& [what, with] : replacements)
	{
		if (path.starts_with(what))
		{
			path = with + path.substr(what.length());
			break;
		}
	}
	m_imports.emplace_back(path);
}

void ScriptHost::InvokeCallback(CallbackID id, VariantArgs args)
{
	const auto dispId = GetDISPID(id);
	if (dispId)
	{
		std::ranges::reverse(args);

		DISPPARAMS params{};
		params.rgvarg = args.data();
		params.cArgs = js::sizeu(args);

		m_script_root->Invoke(*dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
	}
}

void ScriptHost::InvokeMouseCallback(uint32_t msg, WPARAM wp, LPARAM lp)
{
	const auto id = g_msg_callback_map.at(msg);
	InvokeCallback(id, { GET_X_LPARAM(lp), GET_Y_LPARAM(lp), wp });
}

void ScriptHost::Reset()
{
	if (m_script_engine && Connected())
	{
		wil::com_ptr_t<IActiveScriptGarbageCollector> gc;
		if SUCCEEDED(m_script_engine->QueryInterface(&gc))
		{
			gc->CollectGarbage(SCRIPTGCTYPE_EXHAUSTIVE);
		}

		m_script_engine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
		m_script_engine->SetScriptState(SCRIPTSTATE_CLOSED);
		m_script_engine->Close();
	}

	m_context_to_path_map.clear();
	m_callback_map.clear();
	m_script_engine.reset();
	m_script_root.reset();
}

void ScriptHost::ParsePreprocessor()
{
	m_imports.clear();
	m_name.clear();

	static constexpr std::string_view pre_start = "// ==PREPROCESSOR==";
	static constexpr std::string_view pre_end = "// ==/PREPROCESSOR==";

	const std::string code = m_panel->m_config.m_code.get_ptr();
	std::string author, version;
	const size_t start_pos = code.find(pre_start) + pre_start.length();
	const size_t end_pos = code.find(pre_end);

	if (start_pos < end_pos && end_pos < code.length())
	{
		const std::string pre_body = code.substr(start_pos, end_pos - start_pos);

		for (auto&& line : js::split_string(pre_body, CRLF))
		{
			if (line.contains("@import"))
			{
				AddImport(line);
			}
			else if (line.contains("@name"))
			{
				m_name = ExtractValue(line);
			}
			else if (line.contains("@author"))
			{
				author = ExtractValue(line);
			}
			else if (line.contains("@version"))
			{
				version = ExtractValue(line);
			}
		}
	}

	if (m_name.empty()) m_name = fmt::format("id:{}", m_panel->m_id);
	if (version.length()) version = fmt::format(" v{}", version);
	if (author.length()) author = fmt::format(" by {}", author);

	m_build_string = fmt::format("{} ({}{}{})", Component::name_version, m_name, version, author);
}
