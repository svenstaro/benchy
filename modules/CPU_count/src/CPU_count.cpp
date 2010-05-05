#include <omp.h>

#include <iostream>
#include <vector>

#include "CPU_count.hpp"

uint32_t CPU_count_run(std::string mode) {
	uint32_t score = 0;
	uint16_t threads;
	if (mode == "single") {
		// Just do a single thread.
		threads = 1;
	} 
	else if (mode == "multi") {
		// Get amount of system-visible hardware cores.
		threads = omp_get_max_threads();
	}
	else {
		std::cerr << "ERROR: Wrong value given to CPU_count_run()." << std::endl;
		exit(1);
	}

	// int_vec is the container to do work on
	std::vector<uint64_t> int_vec;

	// Fill the container up with the amount of defined max entries.
	for(uint32_t i = 0; i < ENTRIES; ++i)
		int_vec.push_back(0);

	// Boost.Timer would be great here instead of using the more complicated 
	// Boost.Asio to do it. Sadly, tests show that Boost.Timer is unreliable.
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);
	timer.expires_from_now(boost::posix_time::seconds(TIME));

	// Status messages.
	std::cout << "Start CPU_count in " << mode << " mode." << std::endl;
	std::cout << "Using " << threads << " threads." << std::endl;

	// Do actual counting "work" here.
#pragma omp parallel shared(int_vec) num_threads(threads)
	while(!timer.expires_from_now().is_negative()) {
		for(uint32_t i = 0; i < ENTRIES; ++i)
			int_vec[i] += 1;
	}

	// Calculate scores here.
	uint64_t result = 0;
	BOOST_FOREACH(uint64_t i, int_vec)
		result += i;

	// Make result sane and output it as score.
	std::cout << "    Result: " << result << std::endl;
	score = result/100000;
	std::cout << "    Score: " << score << std::endl;

	return score;
}
