#include "vfs.h"
#include "native_fs.h"
#include <iostream>

void VFS::mount(const std::string& alias, FSRef filesystem)
{
}

FSRef VFS::get_filesystem(const std::string& alias)
{
	return std::make_shared<NativeFS>(alias);
}

FileRef VFS::open_file(const std::string& path, FileMode mode)
{
	auto alias = get_alias(path);
	auto fs_path = path.substr(alias.size() + (alias.empty() ? 0 : 1));
	
	std::cout << "alias: " << alias << std::endl;
	std::cout << "fs_path: " << fs_path << std::endl;

	/*
		//(aliases "" and "." refer to default_fs)
		for fs in filesystems with name "alias"
		{
			if(fs.has_file(fs_path))
			{
				return fs.open_file(fs_path);
			}
		}
		return default_fs.open_file(path); //(not a fs_path but path)
	*/

	return get_filesystem(alias)->open_file(fs_path, mode);
}

std::string VFS::get_alias(const std::string& text) const
{
	auto slash_it = text.find_first_of("/");

	if (slash_it != std::string::npos)
	{
		return text.substr(0, slash_it);
	}

	return "";
}
