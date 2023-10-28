#pragma once

class ComArrayReader
{
public:
	HRESULT convert(const VARIANT& v, pfc::bit_array_bittable& out);
	HRESULT convert(const VARIANT& v, pfc::string_list_impl& out);

private:
	HRESULT get_property(IDispatch* pdisp, wil::zwstring_view name, VARTYPE vt, VARIANT& result);
	HRESULT init(const VARIANT& v, VARTYPE vt);

	VariantArgs m_data;
};
