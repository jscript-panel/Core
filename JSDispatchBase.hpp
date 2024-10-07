#pragma once

template <typename T>
class JSDispatchBase : public T
{
protected:
	JSDispatchBase() = default;
	virtual ~JSDispatchBase() = default;

public:
	STDMETHODIMP GetIDsOfNames(REFIID, OLECHAR** names, uint32_t, LCID, DISPID* dispids) final
	{
		RETURN_HR_IF_NULL(E_POINTER, dispids);
		RETURN_IF_FAILED(InitTypeInfo());

		const ULONG hash = LHashValOfName(LANG_NEUTRAL, names[0]);
		const auto it = s_dispid_map.find(hash);

		if (it != s_dispid_map.end())
		{
			dispids[0] = it->second;
		}
		else
		{
			RETURN_IF_FAILED_EXPECTED(s_type_info->GetIDsOfNames(&names[0], 1, &dispids[0]));

			s_dispid_map.emplace(hash, dispids[0]);
		}

		return S_OK;
	}

	STDMETHODIMP GetTypeInfo(uint32_t i, LCID, ITypeInfo** out) final
	{
		RETURN_HR_IF_NULL(E_POINTER, out);
		RETURN_HR_IF(DISP_E_BADINDEX, i != 0);

		s_type_info->AddRef();
		*out = s_type_info.get();
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
		return s_type_info->Invoke(this, dispid, flags, params, result, excep, err);
	}

private:
	HRESULT InitTypeInfo()
	{
		if (!s_type_info)
		{
			RETURN_IF_FAILED(factory::type_lib->GetTypeInfoOfGuid(__uuidof(T), &s_type_info));
		}

		return S_OK;
	}

	inline static std::unordered_map<ULONG, DISPID> s_dispid_map;
	inline static wil::com_ptr_t<ITypeInfo> s_type_info;
};
