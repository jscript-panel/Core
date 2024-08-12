#include "stdafx.hpp"
#include "ComArrayWriter.hpp"

ComArrayWriter::ComArrayWriter(size_t count) : m_psa(SafeArrayCreateVector(VT_VARIANT, 0L, static_cast<ULONG>(count))) {}

#pragma region static
HRESULT ComArrayWriter::write_audio_chunk_data(const audio_chunk_impl& chunk, VARIANT* out)
{
	const auto size = chunk.get_used_size();
	const auto data = chunk.get_data();
	ComArrayWriter writer(size);

	for (const size_t i : std::views::iota(size_t{}, size))
	{
		auto var = _variant_t(data[i]);
		RETURN_IF_FAILED(writer.add_item(var));
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

HRESULT ComArrayWriter::write_ints(const std::vector<int>& ints, VARIANT* out)
{
	ComArrayWriter writer(ints.size());

	for (const int i : ints)
	{
		auto var = _variant_t(i);
		RETURN_IF_FAILED(writer.add_item(var));
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

HRESULT ComArrayWriter::write_uints(const std::vector<size_t>& uints, VARIANT* out)
{
	ComArrayWriter writer(uints.size());

	for (const size_t i : uints)
	{
		auto var = _variant_t(i);
		RETURN_IF_FAILED(writer.add_item(var));
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
