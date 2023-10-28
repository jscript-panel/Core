#pragma once

template <typename T>
class JSDispatch : public JSDispatchBase<T>
{
protected:
	JSDispatch() {}
	~JSDispatch() {}

public:
	COM_QI_BEGIN()
		COM_QI_ENTRY(IUnknown)
		COM_QI_ENTRY(IDispatch)
		COM_QI_ENTRY(T)
	COM_QI_END()
};
