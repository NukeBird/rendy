#include "buffer_es3.h"
#include <optick.h>

void Rendy::ES3::Buffer::bind(uint32_t index)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("index", index);

	glBindBufferBase(get_gl_target(), index, id);
}