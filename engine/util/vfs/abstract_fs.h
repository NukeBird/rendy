#pragma once
#include "abstract_file.h"
#include <memory>
#include <string>

namespace Rendy
{
	struct AbstractFS
	{
		virtual bool validate() = 0;
		virtual FileRef open_file(const std::string& path, FileMode mode) = 0;
		virtual bool has_file(const std::string& path) = 0;
		//create_file
		//remove_file
		//copy_file
		//rename_file
		//is_file
		//is_directory
	};

	using FSRef = std::shared_ptr<AbstractFS>;
};