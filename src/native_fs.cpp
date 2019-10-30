#include "native_fs.h"
#include "native_file.h"

Rendy::NativeFS::NativeFS(const std::string& base_path)
{
	this->base_path = base_path;
	normalize_base_path();
}

bool Rendy::NativeFS::validate()
{
	return true; //TODO
}

Rendy::FileRef Rendy::NativeFS::open_file(const std::string& path, FileMode mode)
{
	return std::make_shared<NativeFile>(base_path + path, mode);
}

bool Rendy::NativeFS::has_file(const std::string& path)
{
	auto file = open_file(path, FileMode::Read);
	return file->is_open();
}

void Rendy::NativeFS::normalize_base_path()
{
	if (validate())
	{
		if (!base_path.empty())
		{
			if (base_path.back() != '/')
			{
				base_path.push_back('/');
			}
		}
	}
}
