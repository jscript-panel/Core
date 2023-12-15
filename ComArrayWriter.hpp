#pragma once

class ComArrayWriter
{
public:
	static HRESULT write_ints(const std::vector<int>& ints, VARIANT* out);
	static HRESULT write_strings(const WStrings& strings, VARIANT* out);
	static HRESULT write_strings(const pfc::array_t<string8>& strings, VARIANT* out);

private:
	ComArrayWriter(size_t count);

	HRESULT add_item(_variant_t& var);
	HRESULT add_item(wil::zwstring_view str);
	void finalise(VARIANT* out);

	SAFEARRAY* m_psa{};
	LONG m_idx{};
};
