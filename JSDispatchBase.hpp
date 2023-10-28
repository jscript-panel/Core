#pragma once

static wil::com_ptr_t<ITypeLib> g_type_lib;

struct TypeInfoCache
{
	wil::com_ptr_t<ITypeInfo> type_info;
	std::unordered_map<ULONG, DISPID> cache;
};

template <typename T>
class JSDispatchBase : public T
{
protected:
	JSDispatchBase()
	{
		if (!g_type_lib)
		{
			const auto path = wil::GetModuleFileNameW(core_api::get_my_instance());
			LoadTypeLibEx(path.get(), REGKIND_NONE, &g_type_lib);
		}

		if (!s_type_info_cache.type_info)
		{
			g_type_lib->GetTypeInfoOfGuid(__uuidof(T), &s_type_info_cache.type_info);
		}
	}

	virtual ~JSDispatchBase() {}

public:
	STDMETHODIMP GetIDsOfNames(REFIID, OLECHAR** names, uint32_t, LCID, DISPID* dispids) final
	{
		RETURN_HR_IF_NULL(E_POINTER, dispids);

		const ULONG hash = LHashValOfName(LANG_NEUTRAL, names[0]);
		const auto it = s_type_info_cache.cache.find(hash);
		if (it != s_type_info_cache.cache.end())
		{
			dispids[0] = it->second;
		}
		else
		{
			RETURN_IF_FAILED(s_type_info_cache.type_info->GetIDsOfNames(&names[0], 1, &dispids[0]));
			s_type_info_cache.cache.emplace(hash, dispids[0]);
		}
		return S_OK;
	}

	STDMETHODIMP GetTypeInfo(uint32_t i, LCID, ITypeInfo** out) final
	{
		RETURN_HR_IF_NULL(E_POINTER, out);
		RETURN_HR_IF(DISP_E_BADINDEX, i != 0);

		s_type_info_cache.type_info->AddRef();
		*out = s_type_info_cache.type_info.get();
		return S_OK;
	}

	STDMETHODIMP GetTypeInfoCount(uint32_t* n) final
	{
		RETURN_HR_IF_NULL(E_POINTER, n);

		*n = 1;
		return S_OK;
	}

	STDMETHODIMP Invoke(DISPID dispid, REFIID, LCID, WORD flags, DISPPARAMS* params, VARIANT* result, EXCEPINFO* excep, uint32_t* err) final
	{
		return s_type_info_cache.type_info->Invoke(this, dispid, flags, params, result, excep, err);
	}

private:
	static TypeInfoCache s_type_info_cache;
};

template <typename T>
FOOGUIDDECL TypeInfoCache JSDispatchBase<T>::s_type_info_cache;
