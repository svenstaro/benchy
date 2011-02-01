aux_source_directory(src sources)

find_package(OGRE)
find_package(Noise)

add_executable(tunnel-adventure ${sources})

target_link_libraries(tunnel-adventure 
	OGRE
	Noise
	)
