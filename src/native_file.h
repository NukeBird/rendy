#pragma once
#include "abstract_file.h"
#include <fstream>
#include <string>

class NativeFile final: public AbstractFile
{
public:
	NativeFile(const std::string& path, FileMode mode);
	virtual uint64_t get_size() const override;
	virtual bool is_read_only() const override;
	virtual bool validate() override;
	virtual uint64_t seek(uint64_t offset, SeekOrigin origin = Begin) override;
	virtual uint64_t tell() override;
	virtual uint64_t read(void * buffer, uint64_t size) override;
	virtual uint64_t write(void * buffer, uint64_t size) override;
private:
	void calculate_size();

	std::fstream stream;
	uint64_t size;
	FileMode mode;
};