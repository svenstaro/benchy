#include <boost/cstdint.hpp>

#include <map>
#include <string>

typedef std::map<std::string, uint32_t> score_map;

// This is what you call a compile-time plugin system. (YEAAAAAAAAH!)
#ifdef BUILD_CPU_TESTS
#include "CPU_count.hpp"
#include "CPU_smallpt.hpp"
#endif

/*
#ifdef BUILD_MEM_TESTS
#include "MEM_array.hpp"
#include "MEM_malloc.hpp"
#endif

#ifdef BUILD_HDD_TESTS
#include "HDD_readwrite.hpp"
#endif

#ifdef BUILD_2D_TESTS
#include "2D_infinerator.hpp"
#include "2D_sprites.hpp"
#endif

#ifdef BUILD_3D_TESTS
#include "3D_asteroids.hpp"
#include "3D_dancers.hpp"
#include "3D_lunar.hpp"
#include "3D_tunnel.hpp"
#include "3D_woods.hpp"
#endif
*/


class TestManager {
public:
	TestManager();
	~TestManager();

	void RunTests();
	score_map GetScores();

private:
	// map in which to store score pais in the format: [testname, associated score]
	score_map m_score_map;
};

