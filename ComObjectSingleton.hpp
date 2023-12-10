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
	inline static wil::com_ptr_t<IDispatch> instance;
};
