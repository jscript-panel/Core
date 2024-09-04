#include "stdafx.hpp"
#include "ComArrayWriter.hpp"

ComArrayWriter::ComArrayWriter(size_t count) : m_psa(SafeArrayCreateVector(VT_VARIANT, 0L, static_cast<ULONG>(count))) {}

#pragma region static
HRESULT ComArrayWriter::write_audio_samples(std::span<audio_sample> samples, VARIANT* out)
{
	ComArrayWriter writer(samples.size());

	for (auto&& sample : samples)
	{
		RETURN_IF_FAILED(writer.add_item(sample));
	}

	writer.finalise(out);
	return S_OK;
}

HRESULT ComArrayWriter::write_handles_map(const MetadbHandleList::Map& map, VARIANT* out)
{
	ComArrayWriter writer(map.size() * 2);

	for (auto&& item : map)
	{
		auto var = _variant_t(new ComObject<MetadbHandleList>(item.second));
		RETURN_IF_FAILED(writer.add_item(item.first));
		RETURN_IF_FAILED(writer.add_item(var));
	}

	writer.finalise(out);
	return S_OK;
}

HRESULT ComArrayWriter::write_ints(std::span<int> ints, VARIANT* out)
{
	ComArrayWriter writer(ints.size());

	for (const int i : ints)
	{
		RETURN_IF_FAILED(writer.add_item(i));
	}

	writer.finalise(out);
	return S_OK;
}

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

HRESULT ComArrayWriter::write_strings(const fb2k::array_typed<fb2k::string>& strings, VARIANT* out)
{
	ComArrayWriter writer(strings.get_count());

	for (auto&& string : strings)
	{
		const auto wstr = js::to_wide(string->c_str());
		RETURN_IF_FAILED(writer.add_item(wstr));
	}

	writer.finalise(out);
	return S_OK;
}

HRESULT ComArrayWriter::write_strings(const pfc::array_t<string8>& strings, VARIANT* out)
{
	ComArrayWriter writer(strings.get_count());

	for (auto&& string : strings | std::views::transform(js::to_wide))
	{
		RETURN_IF_FAILED(writer.add_item(string));
	}

	writer.finalise(out);
	return S_OK;
}

HRESULT ComArrayWriter::write_uints(std::span<size_t> uints, VARIANT* out)
{
	ComArrayWriter writer(uints.size());

	for (const size_t i : uints)
	{
		RETURN_IF_FAILED(writer.add_item(i));
	}

	writer.finalise(out);
	return S_OK;
}
#pragma endregion

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

HRESULT ComArrayWriter::add_item(js::IsNum auto num)
{
	auto var = _variant_t(num);
	return add_item(var);
}

HRESULT ComArrayWriter::add_item(std::wstring_view str)
{
	auto var = _variant_t(str.data());
	return add_item(var);
}

void ComArrayWriter::finalise(VARIANT* out)
{
	out->vt = VT_ARRAY | VT_VARIANT;
	out->parray = m_psa;
}
