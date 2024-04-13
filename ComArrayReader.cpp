#include "stdafx.hpp"
#include "ComArrayReader.hpp"

HRESULT ComArrayReader::convert(const VARIANT& v, pfc::bit_array_bittable& out)
{
	RETURN_IF_FAILED(init(v, VT_UI4));
	if (m_data.empty()) out.resize(0);

	for (auto&& item : m_data)
	{
		out.set(item.ulVal, true);
	}
	return S_OK;
}

HRESULT ComArrayReader::convert(const VARIANT& v, pfc::string_list_impl& out)
{
	RETURN_IF_FAILED(init(v, VT_BSTR));

	for (auto&& item : m_data)
	{
		const string8 str = js::from_wide(item.bstrVal);
		out.add_item(str);
	}
	return S_OK;
}

HRESULT ComArrayReader::get_property(IDispatch* pdisp, wil::zwstring_view name, VARTYPE vt, VARIANT& result)
{
	auto cname = const_cast<LPOLESTR>(name.data());
	DISPID dispId{};
	DISPPARAMS params{};

	RETURN_IF_FAILED(pdisp->GetIDsOfNames(IID_NULL, &cname, 1, LOCALE_USER_DEFAULT, &dispId));
	RETURN_IF_FAILED(pdisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params, &result, nullptr, nullptr));
	RETURN_IF_FAILED(VariantChangeType(&result, &result, 0, vt));
	return S_OK;
}

HRESULT ComArrayReader::init(const VARIANT& v, VARTYPE vt)
{
	RETURN_HR_IF(E_INVALIDARG, v.vt != VT_DISPATCH || !v.pdispVal);

	IDispatch* pdisp = v.pdispVal;
	_variant_t result;
	RETURN_IF_FAILED(get_property(pdisp, L"length", VT_UI4, result));
	const size_t count = result.ulVal;
	m_data.resize(count);

	for (const size_t i : std::views::iota(size_t{}, count))
	{
		RETURN_IF_FAILED(get_property(pdisp, std::to_wstring(i), vt, m_data[i]));
	}
	return S_OK;
}
