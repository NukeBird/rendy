file(GLOB STB_SOURCE
	"*.h"
	"*.c"
)

add_library(stb STATIC ${STB_SOURCE})
target_include_directories(stb INTERFACE ../)