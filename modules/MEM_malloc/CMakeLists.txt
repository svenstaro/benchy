set(MEM_malloc_sources
	src/MEM_malloc.cpp
)

add_library(MEM_malloc STATIC
	${MEM_malloc_sources}
)
