#pragma once

class ScriptHost : public ImplementCOMRefCounter<IActiveScriptSite>
{
public:
	ScriptHost(PanelBase* panel);

	COM_QI_SIMPLE(IActiveScriptSite)

	STDMETHODIMP GetDocVersionString(BSTR*) final;
	STDMETHODIMP GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** ppunk, ITypeInfo** ppti) final;
	STDMETHODIMP GetLCID(LCID*) final;
	STDMETHODIMP OnEnterScript() final;
	STDMETHODIMP OnLeaveScript() final;
	STDMETHODIMP OnScriptError(IActiveScriptError* err) final;
	STDMETHODIMP OnScriptTerminate(const VARIANT*, const EXCEPINFO*) final;
	STDMETHODIMP OnStateChange(SCRIPTSTATE state) final;
	bool Initialise();
	bool InvokeMouseRbtnUp(WPARAM wp, LPARAM lp);
	void InvokeCallback(CallbackID id, VariantArgs args = {});
	void Reset();

	SCRIPTSTATE m_state{};
	std::string m_build_string, m_name;
	std::unordered_map<CallbackID, DISPID> m_callback_map;

private:
	HRESULT InitCallbackMap();
	HRESULT ParseImports();
	HRESULT ParseScript(wil::zstring_view code, wil::zstring_view path);
	std::optional<DISPID> GetDISPID(CallbackID id);
	std::string ExtractValue(wil::zstring_view str);
	std::string GetErrorText(IActiveScriptError* err);
	void AddImport(wil::zstring_view str);
	void ParsePreprocessor();

	DWORD m_last_source_context{};
	PanelBase* m_panel;
	Strings m_imports;
	std::unordered_map<DWORD, std::string> m_context_to_path_map;
	wil::com_ptr_nothrow<IActiveScript> m_script_engine;
	wil::com_ptr_t<IActiveScriptProperty> m_script_property;
	wil::com_ptr_t<IActiveScriptParse> m_parser;
	wil::com_ptr_t<IDispatch> m_script_root;
	wil::com_ptr_t<Console> m_console;
	wil::com_ptr_t<Fb> m_fb;
	wil::com_ptr_t<Plman> m_plman;
	wil::com_ptr_t<Utils> m_utils;
	wil::com_ptr_t<Window> m_window;
};
