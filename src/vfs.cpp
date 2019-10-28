#include "vfs.h"
#include <iostream>

VFS::VFS(FSRef default_fs)
{
	this->default_fs = default_fs;
	mount("", default_fs);
	mount(".", default_fs);
}

void VFS::mount(const std::string& alias, FSRef filesystem)
{
	if (filesystem->validate())
	{
		filesystem_map[alias].push_back(filesystem);
	}
}

FSRef VFS::get_filesystem(const std::string& alias)
{
	auto fs_list = get_filesystem_list(alias);

	if (!fs_list.empty())
	{
		return fs_list[0];
	}

	return default_fs;
}

std::vector<FSRef> VFS::get_filesystem_list(const std::string& alias)
{
	auto it = filesystem_map.find(alias);

	if (it != filesystem_map.end())
	{
		return it->second; //TODO: unnecessary copy here?
	}

	return std::vector<FSRef>();
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

	return default_fs->open_file(path, mode);
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
