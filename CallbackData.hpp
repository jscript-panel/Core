#pragma once

struct CallbackData : public pfc::refcounted_object_root
{
	CallbackData(const VariantArgs& args) : m_args(args) {}

	VariantArgs m_args;
};

struct MetadbCallbackData : public pfc::refcounted_object_root
{
	MetadbCallbackData(metadb_handle_list_cref handles, bool fromhook) : m_handles(handles), m_fromhook(fromhook) {}

	bool m_fromhook{};
	metadb_handle_list m_handles;
};

template <typename T>
class CallbackDataReleaser
{
public:
	CallbackDataReleaser(auto data)
	{
		m_data = reinterpret_cast<T*>(data);
	}

	~CallbackDataReleaser()
	{
		m_data->refcount_release();
	}

	T* operator->()
	{
		return m_data;
	}

private:
	T* m_data;
};

struct AlbumArtCallbackData
{
	AlbumArtCallbackData(IMetadbHandle* handle, size_t art_id, IJSImage* image) : m_handle(handle), m_art_id(art_id), m_image(image) {}

	~AlbumArtCallbackData()
	{
		if (m_handle) m_handle->Release();
		if (m_image) m_image->Release();
	}

	IJSImage* m_image{};
	IMetadbHandle* m_handle{};
	size_t m_art_id{};
};

struct ImageCallbackData
{
	ImageCallbackData(BSTR path, IJSImage* image) : m_path(path), m_image(image) {}

	~ImageCallbackData()
	{
		if (m_image) m_image->Release();
	}

	IJSImage* m_image{};
	_bstr_t m_path;
};
