#pragma once

class ComArrayWriter
{
public:
	static HRESULT write_audio_samples(std::span<audio_sample> samples, VARIANT* out);
	static HRESULT write_handles_map(const MetadbHandleList::Map& map, VARIANT* out);
	static HRESULT write_ints(std::span<int> ints, VARIANT* out);
	static HRESULT write_strings(const WStrings& strings, VARIANT* out);
	static HRESULT write_strings(const fb2k::array_typed<fb2k::string>& strings, VARIANT* out);
	static HRESULT write_strings(const pfc::array_t<string8>& strings, VARIANT* out);
	static HRESULT write_uints(std::span<size_t> uints, VARIANT* out);

private:
	ComArrayWriter(size_t count);

	HRESULT add_item(_variant_t& var);
	HRESULT add_item(js::IsNum auto num);
	HRESULT add_item(std::wstring_view str);
	void finalise(VARIANT* out);

	SAFEARRAY* m_psa{};
	LONG m_idx{};
};
