#pragma once

template <typename T>
class JSDisposable : public JSDispatchBase<T>
{
protected:
	JSDisposable() {}
	~JSDisposable() {}

	virtual void FinalRelease() = 0;

public:
	COM_QI_BEGIN()
		COM_QI_ENTRY(IUnknown)
		COM_QI_ENTRY(IDispatch)
		COM_QI_ENTRY(IDisposable)
		COM_QI_ENTRY(T)
	COM_QI_END()

	STDMETHODIMP Dispose() final
	{
		this->FinalRelease();
		return S_OK;
	}
};
