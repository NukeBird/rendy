#pragma once
#include "abstract_fs.h"
#include <fstream>
#include <string>

class NativeFS: public AbstractFS
{
public:
	NativeFS(const std::string& base_path);
	virtual bool validate() override;
	virtual FileRef open_file(const std::string& path, FileMode mode) override;
	virtual bool has_file(const std::string& path) override;
private:
	void normalize_base_path();

	std::string base_path;
};