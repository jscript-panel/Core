#pragma once

template <typename T>
class ComObjectSingleton
{
public:
	static T* get()
	{
		if (!instance)
		{
			instance = new ImplementCOMRefCounter<T>();
		}

		return reinterpret_cast<T*>(instance.get());
	}

private:
	static wil::com_ptr_t<IDispatch> instance;
};

template <typename T>
FOOGUIDDECL wil::com_ptr_t<IDispatch> ComObjectSingleton<T>::instance;
