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
	bool CheckCallbackID(CallbackID id);
	bool Connected();
	bool Error();
	bool Initialise();
	bool InvokeMouseRbtnUp(WPARAM wp, LPARAM lp);
	void InvokeCallback(CallbackID id, VariantArgs args = {});
	void InvokeMouseCallback(uint32_t msg, WPARAM wp, LPARAM lp);
	void Reset();

	std::string m_build_string, m_name;

private:
	struct Preprocessor
	{
		static constexpr std::string_view Start = "// ==PREPROCESSOR==";
		static constexpr std::string_view End = "// ==/PREPROCESSOR==";
	};

	HRESULT InitCallbackMap();
	HRESULT InitScriptEngine();
	HRESULT InitVersion();
	HRESULT ParseImports();
	HRESULT ParseScript(std::string_view path);
	std::optional<DISPID> GetDISPID(CallbackID id);
	std::string ExtractValue(std::string_view str);
	std::string GetErrorText(IActiveScriptError* err);
	void AddImport(std::string_view str);
	void ParsePreprocessor();

	DWORD m_last_source_context{};
	PanelBase* m_panel;
	SCRIPTSTATE m_state{};
	Strings m_imports;
	std::unordered_map<CallbackID, DISPID> m_callback_map;
	std::unordered_map<DWORD, std::string> m_context_to_path_map;
	wil::com_ptr_t<IActiveScript> m_script_engine;
	wil::com_ptr_t<IActiveScriptProperty> m_script_property;
	wil::com_ptr_t<IActiveScriptParse> m_parser;
	wil::com_ptr_t<IDispatch> m_script_root;
	wil::com_ptr_t<Window> m_window;
};
