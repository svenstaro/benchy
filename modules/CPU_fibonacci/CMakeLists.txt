set(CPU_fibonacci_sources
	src/CPU_fibonacci.cpp
)

add_library(CPU_fibonacci STATIC
	${CPU_fibonacci_sources}
)
