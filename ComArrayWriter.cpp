#include "stdafx.hpp"
#include "ComArrayWriter.hpp"

ComArrayWriter::ComArrayWriter(size_t count) : m_psa(SafeArrayCreateVector(VT_VARIANT, 0L, static_cast<ULONG>(count))) {}

HRESULT ComArrayWriter::write_strings(const WStrings& strings, VARIANT* out)
{
	ComArrayWriter writer(strings.size());

	for (auto&& string : strings)
	{
		RETURN_IF_FAILED(writer.add_item(string));
	}

	writer.finalise(out);
	return S_OK;
}

HRESULT ComArrayWriter::add_item(_variant_t& var)
{
	const HRESULT hr = SafeArrayPutElement(m_psa, &m_idx, &var);
	m_idx++;

	if FAILED(hr)
	{
		SafeArrayDestroy(m_psa);
		m_psa = nullptr;
	}
	return hr;
}

HRESULT ComArrayWriter::add_item(wil::zstring_view str)
{
	const std::wstring wstr = to_wide(str);
	return add_item(wstr);
}

HRESULT ComArrayWriter::add_item(wil::zwstring_view str)
{
	auto var = _variant_t(str.data());
	return add_item(var);
}

void ComArrayWriter::finalise(VARIANT* out)
{
	out->vt = VT_ARRAY | VT_VARIANT;
	out->parray = m_psa;
}
