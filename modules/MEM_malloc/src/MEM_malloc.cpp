#include <iostream>

#include "MEM_malloc.hpp"

uint32_t MEM_malloc_run() {
	uint32_t score = 0;
	
	// Status messages.
	std::cout << "Start MEM_malloc" << std::endl;
	std::cout << "Using " << (BYTES/1024)/1024 << " MiB of memory." << std::endl;

	// 8 bit are 1 byte, thus we conveniently use uint8_t (which is char actually).
	uint8_t* memory_blob;
	memory_blob = (uint8_t*)malloc(BYTES);

	// Initialize memory first.
	for(uint64_t i = 0; i < BYTES; ++i) {
		memory_blob[i] = (uint8_t)0;
	}

	// Declare timers to use later on for time comparison.
	boost::posix_time::ptime date_before;
	boost::posix_time::ptime date_after;
	boost::posix_time::time_duration running_time;

	// Get current date to compare with afterwards to check running time.
	date_before = boost::posix_time::microsec_clock::local_time();

	// Perform write performance test.
	for(uint16_t j = 0; j < REPEAT; ++j) {
		for(uint64_t i = 0; i < BYTES; ++i) {
			memory_blob[i] = (uint8_t)RANDOM_NO;
		}
	}

	// Compare dates to get running time.
	date_after = boost::posix_time::microsec_clock::local_time();
	running_time = date_after - date_before;

	uint64_t result;
	result = uint64_t(BYTES / (float((running_time.total_milliseconds()) / REPEAT) / 1000.f));
	std::cout << "    Write result: " << (result/1024)/1024 << " MiB/s" << std::endl;

	// Get current date to compare with afterwards to check running time.
	date_before = boost::posix_time::microsec_clock::local_time();

	// Perform read performance test.
	for(uint16_t j = 0; j < REPEAT; ++j) {
		for(uint64_t i = 0; i < BYTES; ++i) {
			assert(memory_blob[i] == (uint8_t)RANDOM_NO);
		}
	}

	// Compare dates to get running time.
	date_after = boost::posix_time::microsec_clock::local_time();
	running_time = date_after - date_before;

	result = uint64_t(BYTES / (float((running_time.total_milliseconds()) / REPEAT) / 1000.f));
	std::cout << "    Read result: " << (result/1024)/1024 << " MiB/s" << std::endl;

	free(memory_blob);

	// Calculate scores here.
	/*uint64_t result = 0;
	BOOST_FOREACH(uint64_t i, int_vec)
		result += i;

	// Make result sane and output it as score.
	std::cout << "    Result: " << result << std::endl;
	score = result/100000;
	std::cout << "    Score: " << score << std::endl;
	*/

	return score;
}

int main() {
	MEM_malloc_run();
	return 0;
}
