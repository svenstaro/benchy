/*
 * This benchmark mainly tests memory speed in a threaded/non-threaded
 * environment. It assumes a scenario where a few hundred data containers have
 * to be modified independently of each other.
 *
 */

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/thread.hpp>
#include <boost/multi_array.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <list>

// number of entries to be processed
#define ENTRIES	150 // needs to be even
// amount of data per entry
#define DATA_SIZE 200000
// iterations through the read/write loops
#define ITERATIONS 50

typedef boost::multi_array<double, 2> array_double;
typedef array_double::index index_double;

typedef boost::multi_array<uint32_t, 2> array_uint32_t;
typedef array_uint32_t::index index_uint32_t;

void readArray(array_double& entries_double, array_uint32_t& entries_uint32_t, 
			   const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	for(uint16_t k = 0; k < ITERATIONS; ++k) {
		for(index_double i = real_offset; i < numloops; ++i) {
			for(index_double j = 0; j < DATA_SIZE; ++j) {
				entries_double[i][j];
			}
			for(index_uint32_t j = 0; j < DATA_SIZE; ++j) {
				entries_uint32_t[i][j];
			}
		}
	}
}

void writeArray(array_double& entries_double, array_uint32_t& entries_uint32_t, 
				const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	for(uint16_t k = 0; k < ITERATIONS; ++k) {
		for(index_double i = real_offset; i < numloops; ++i) {
			for(index_double j = 0; j < DATA_SIZE; ++j) {
				entries_double[i][j] *= 0.513;
				entries_double[i][j] -= 6.1;
			}
			for(index_uint32_t j = 0; j < DATA_SIZE; ++j) {
				entries_uint32_t[i][j] *= 3;
				entries_uint32_t[i][j] += 13;
			}
		}
	}
}

int main () {
	uint16_t CORES = boost::thread::hardware_concurrency();

	boost::timer total_time;
	boost::timer worker_time;

	srand(42); // we always want the same seed!

	boost::thread_group threads;

	// create a 2D array of double that is ENTRIES x DATA_SIZE
	array_double entries_double(boost::extents[ENTRIES][DATA_SIZE]);

	// create a 2D array of uint32_t that is ENTRIES x DATA_SIZE
	array_uint32_t entries_uint32_t(boost::extents[ENTRIES][DATA_SIZE]);

	worker_time.restart();
	// fill up our lists
	for(index_double i = 0; i < ENTRIES; ++i) {
		for(index_double j = 0; j < DATA_SIZE; ++j) {
			entries_double[i][j] = static_cast<double>(rand());
		}
	}

	for(index_uint32_t i = 0; i < ENTRIES; ++i) {
		for(index_uint32_t j = 0; j < DATA_SIZE; ++j) {
			entries_uint32_t[i][j] = rand();
		}
	}
	std::cout << "init time: " << worker_time.elapsed() << std::endl;

	int16_t n_threads = 1;
	while(n_threads != -1) {
		std::cout << "using " << n_threads << " threads" << std::endl;

		worker_time.restart();
		// read our lists a bunch of times and don't do anything
		for(uint16_t i = 0; i < n_threads ; ++i) {
			threads.create_thread(boost::bind(readArray, boost::ref(entries_double), 
						boost::ref(entries_uint32_t), i, n_threads));
		}
		threads.join_all();
		std::cout << "  read time: " << worker_time.elapsed() << std::endl;

		worker_time.restart();
		// write our lists a few times
		for(uint16_t i = 0; i < n_threads; ++i) {
			threads.create_thread(boost::bind(writeArray, boost::ref(entries_double), 
						boost::ref(entries_uint32_t), i, n_threads));
		}
		threads.join_all();
		std::cout << "  write time: " << worker_time.elapsed() << std::endl;

		if (n_threads == 1)
			n_threads = CORES;
		else if (n_threads == CORES)
			n_threads = ENTRIES;
		else if (n_threads == ENTRIES)
			n_threads = -1;
	}
	std::cout << "total time: " << total_time.elapsed() << std::endl;

	return 0;
}
// vim:set ts=4 sw=4 noet:
