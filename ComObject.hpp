#pragma once

template <typename Base>
class ComObject : public Base
{
protected:
	virtual ~ComObject() {}

public:
	ComObject(auto&&... args) : Base(std::forward<decltype(args)>(args)...) {}

	ULONG STDMETHODCALLTYPE AddRef() final
	{
		return ++m_counter;
	}

	ULONG STDMETHODCALLTYPE Release() final
	{
		const long n = --m_counter;
		if (n == 0)
		{
			if constexpr (requires { this->FinalRelease(); })
			{
				this->FinalRelease();
			}
			delete this;
		}
		return n;
	}

private:
	pfc::refcounter m_counter = 1;
};
