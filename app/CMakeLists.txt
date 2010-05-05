include_directories(include)

set(benchy_app_common_sources
	src/TestManager.cpp
)

add_library(benchy_app_common STATIC
	${benchy_app_common_sources}
)

target_link_libraries(benchy_app_common
	${Boost_LIBRARIES}
	${OpenMP_CXX_FLAGS}
)

set(benchy_app_cli_sources
	src/cli_frontend.cpp
)

set(benchy_app_qt_sources
	src/qt_frontend.cpp
)

if(BENCHY_BUILD_MODULES_CPU)
	target_link_libraries(benchy_app_common
		CPU_count
		CPU_fibonacci
		CPU_smallpt
	)
endif(BENCHY_BUILD_MODULES_CPU)

if(BENCHY_BUILD_MODULES_MEM)
	target_link_libraries(benchy_app_common
		MEM_array
		MEM_malloc
	)
endif(BENCHY_BUILD_MODULES_MEM)

if(BENCHY_BUILD_MODULES_HDD)
	target_link_libraries(benchy_app_common
		HDD_readwrite
	)
endif(BENCHY_BUILD_MODULES_HDD)

if(BENCHY_BUILD_MODULES_2D)
	target_link_libraries(benchy_app_common
		2D_infinerator
		2D_sprites
	)
endif(BENCHY_BUILD_MODULES_2D)

if(BENCHY_BUILD_MODULES_3D)
	target_link_libraries(benchy_app_common
		3D_asteroids
		3D_dancers
		3D_lunar
		3D_tunnel
		3D_woods
	)
endif(BENCHY_BUILD_MODULES_3D)

if(BENCHY_BUILD_APP_CLI)
	add_executable(benchy_app_cli 
		${benchy_app_cli_sources}
	)

	target_link_libraries(benchy_app_cli
		benchy_app_common
	)
endif(BENCHY_BUILD_APP_CLI)

if(BENCHY_BUILD_APP_QT)
	add_executable(benchy_app_qt 
		${benchy_app_qt_sources}
	)

	target_link_libraries(benchy_app_cli
		benchy_app_common
	)
endif(BENCHY_BUILD_APP_QT)

