#pragma once
#include <cstdint>
#include <memory>
#include <string>

enum class FileMode
{
	Read,
	Write,
	ReadWrite
};

struct AbstractFile 
{
	enum SeekOrigin
	{
		Begin,
		End,
		Set
	};

	virtual uint64_t get_size() const = 0;
	virtual bool is_read_only() const = 0;
	virtual bool is_open() const = 0;
	virtual bool validate() = 0;
	virtual uint64_t seek(uint64_t offset, SeekOrigin origin = Begin) = 0;
	virtual uint64_t tell() = 0;
	virtual uint64_t read(void* buffer, uint64_t size) = 0;
	virtual uint64_t write(void* buffer, uint64_t size) = 0;
};

using FileRef = std::shared_ptr<AbstractFile>;