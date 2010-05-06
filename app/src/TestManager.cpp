#include "TestManager.hpp"

TestManager::TestManager() {}
TestManager::~TestManager() {}

void TestManager::RunTests() {
	// Reset scores.
	m_score_map.insert(std::make_pair("benchy_score", 0)); // average of other scores
	m_score_map.insert(std::make_pair("cpu_single_core_score", 0));
	m_score_map.insert(std::make_pair("cpu_multi_core_score", 0));
	m_score_map.insert(std::make_pair("mem_score", 0));
	m_score_map.insert(std::make_pair("hdd_score", 0));
	m_score_map.insert(std::make_pair("2d_score", 0));
	m_score_map.insert(std::make_pair("3d_score", 0));

#ifdef BUILD_CPU_TESTS
	// CPU single core tests.
	m_score_map["cpu_single_core_score"] += CPU_count_run("single");
	m_score_map["cpu_single_core_score"] += CPU_smallpt_run("single");
	// Get average.
	m_score_map["cpu_single_core_score"] /= 2;

	// CPU multi core tests.
	m_score_map["cpu_multi_core_score"] += CPU_count_run("multi");
	m_score_map["cpu_multi_core_score"] += CPU_smallpt_run("multi");
	// Get average.
	m_score_map["cpu_multi_core_score"] /= 2;
#endif


#ifdef BUILD_MEM_TESTS
	//m_score_map["mem_score"] += MEM_array_run();
	m_score_map["mem_score"] += MEM_malloc_run();
	// Get average.
	m_score_map["mem_score"] /= 1;
#endif

/*
#ifdef BUILD_HDD_TESTS
	m_score_map["hdd_score"] += HDD_readwrite_run();
	// Get average.
	m_score_map["hdd_score"] /= 1;
#endif

#ifdef BUILD_2D_TESTS
	m_score_map["2d_score"] += 2D_infinerator_run();
	m_score_map["2d_score"] += 2D_sprites_run();
	// Get average.
	m_score_map["2d_score"] /= 2;
#endif

#ifdef BUILD_3D_TESTS
	m_score_map["3d_score"] += 3D_asteroids_run();
	m_score_map["3d_score"] += 3D_dancers_run();
	m_score_map["3d_score"] += 3D_lunar_run();
	m_score_map["3d_score"] += 3D_tunnel_run();
	m_score_map["3d_score"] += 3D_woods_run();
	// Get average.
	m_score_map["3d_score"] /= 5;
#endif
*/

}

score_map TestManager::GetScores() {
	return m_score_map;
}
