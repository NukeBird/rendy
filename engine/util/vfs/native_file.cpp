#include "native_file.h"
#include <optick.h>

Rendy::NativeFile::NativeFile(const std::string& path, FileMode mode)
{
	OPTICK_EVENT();

	this->mode = mode;
	this->size = 0;

	unsigned stream_mode = 0;
	
	switch (mode)
	{
		case FileMode::Read:
		{
			stream_mode = std::ios::in;
			break;
		}
		case FileMode::Write:
		{
			stream_mode = std::ios::out;
			break;
		}
		case FileMode::ReadWrite:
		{
			stream_mode = std::ios::in | std::ios::out;
			break;
		}
	}

	stream_mode |= std::ios::binary;
	
	stream.open(path, stream_mode);

	if (validate())
	{
		calculate_size();
	}
}

uint64_t Rendy::NativeFile::get_size() const
{
	OPTICK_EVENT();
	return size;
}

bool Rendy::NativeFile::is_read_only() const
{
	OPTICK_EVENT();
	return mode == FileMode::Read;
}

bool Rendy::NativeFile::is_open() const
{
	OPTICK_EVENT();
	return stream.is_open();
}

bool Rendy::NativeFile::validate()
{
	OPTICK_EVENT();
	return stream.good();
}

uint64_t Rendy::NativeFile::seek(uint64_t offset, SeekOrigin origin)
{
	OPTICK_EVENT();

	std::ios_base::seekdir way = std::ios_base::beg;

	switch (origin)
	{
		case AbstractFile::Begin:
		{
			way = std::ios_base::beg;
			break;
		}
		case AbstractFile::End:
		{
			way = std::ios_base::end;
			break;
		}
		case AbstractFile::Set:
		{
			way = std::ios_base::cur;
			break;
		}
	}
	
	stream.seekg(offset, way);
	stream.seekp(offset, way);
	
	return tell();
}

uint64_t Rendy::NativeFile::tell()
{
	OPTICK_EVENT();
	return static_cast<uint64_t>(stream.tellg());
}

uint64_t Rendy::NativeFile::read(void* buffer, uint64_t size)
{
	OPTICK_EVENT();

	if (!validate())
	{
		return 0;
	}

	stream.read(reinterpret_cast<char*>(buffer), 
		static_cast<std::streamsize>(size));

	if (stream)
	{
		return size;
	}

	return static_cast<uint64_t>(stream.gcount());
}

uint64_t Rendy::NativeFile::write(void* buffer, uint64_t size)
{
	OPTICK_EVENT();

	if (!validate())
	{
		return 0;
	}

	stream.write(reinterpret_cast<char*>(buffer),
		static_cast<std::streamsize>(size));

	if (stream)
	{
		return size;
	}

	uint64_t result = static_cast<uint64_t>(stream.gcount());

	calculate_size(); //TODO

	return result;
}

void Rendy::NativeFile::calculate_size()
{
	OPTICK_EVENT();

	seek(0, SeekOrigin::End);
	size = tell();
	seek(0, SeekOrigin::Begin);
}
