#pragma once

class ComArrayWriter
{
public:
	ComArrayWriter(size_t count);

	static HRESULT write_strings(const WStrings& strings, VARIANT* out);
	static HRESULT write_strings(const pfc::array_t<string8>& strings, VARIANT* out);

	HRESULT add_item(_variant_t& var);
	HRESULT add_item(wil::zstring_view str);
	void finalise(VARIANT* out);

private:
	HRESULT add_item(wil::zwstring_view str);

	SAFEARRAY* m_psa{};
	LONG m_idx{};
};
