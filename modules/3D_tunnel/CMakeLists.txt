aux_source_directory(src sources)

find_package(OGRE)

add_executable(tunnel-adventure ${sources})

target_link_libraries(tunnel-adventure OGRE)
