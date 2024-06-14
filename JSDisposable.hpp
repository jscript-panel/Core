#pragma once

template <typename T>
class JSDisposable : public JSDispatchBase<T>
{
protected:
	JSDisposable() {}
	~JSDisposable() {}

	virtual void __stdcall Dispose() = 0;

public:
	COM_QI_BEGIN()
		COM_QI_ENTRY(IUnknown)
		COM_QI_ENTRY(IDispatch)
		COM_QI_ENTRY(IDisposable)
		COM_QI_ENTRY(T)
	COM_QI_END()
};
