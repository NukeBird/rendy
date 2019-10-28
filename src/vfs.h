#pragma once
#include "abstract_vfs.h"
#include "native_fs.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class VFS final: public AbstractVFS
{
public:
	VFS(FSRef default_fs = std::make_shared<NativeFS>(""));
	virtual void mount(const std::string& alias, FSRef filesystem) override;
	virtual FSRef get_filesystem(const std::string& alias) override;
	virtual FileRef open_file(const std::string& path, FileMode mode = FileMode::ReadWrite) override;
private:
	std::string get_alias(const std::string& text) const;

	std::unordered_map<std::string, std::vector<FSRef>> filesystem_map;
	FSRef default_fs;
};