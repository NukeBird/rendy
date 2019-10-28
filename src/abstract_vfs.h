#pragma once
#include "abstract_fs.h"
#include <string>
#include <memory>

struct AbstractVFS
{
	virtual void mount(const std::string& alias, FSRef filesystem) = 0;
	virtual FSRef get_filesystem(const std::string& alias) = 0;
	virtual FileRef open_file(const std::string& path, FileMode mode = FileMode::ReadWrite) = 0;
};

using VFSRef = std::shared_ptr<AbstractVFS>;