/*
 * This benchmark mainly tests raw CPU speed in a threaded/non-threaded
 * environment. It assumes a scenario where a few hundred data containers have
 * to be modified independently of each other.
 */

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <vector>

// number of entries to be processed
#define ENTRIES	150 // needs to be even or threads will fail
// time to run each task in seconds
#define TIME 5.0

uint64_t calcStats(const std::vector<uint64_t>& int_vec, bool in_megaops = false) {
	uint64_t result = 0;;
	BOOST_FOREACH(uint64_t i, int_vec) {
		result += i;
	}
	if(in_megaops)
		result = result/(TIME*1000000);
	return result;
}

uint64_t calcStats(const std::vector<double>& double_vec, bool in_megaops = false) {
	double result = 0;
	BOOST_FOREACH(double i, double_vec) {
		result += i;
	}
	if(in_megaops)
		result = result/(TIME*1000000);
	return (uint64_t)result;
}

void calcInt(std::vector<uint64_t>& int_vec, const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	while(!boost::this_thread::interruption_requested()) {
		for(uint32_t i = real_offset; i < numloops; ++i) {
			int_vec[i] += 1;
		}
	}
}

void calcFloat(std::vector<double>& double_vec, const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	while(!boost::this_thread::interruption_requested()) {
		for(uint32_t i = real_offset; i < numloops; ++i) {
			double_vec[i] += 1.0f;
		}
	}
}

int main () {
	uint64_t score = 0;
	// get the amount of hardware cores in the system
	uint16_t cores = boost::thread::hardware_concurrency();
	if (cores == 0)
		cores = 2;

	// containers for our calculations
	std::vector<uint64_t> int_vec;
	std::vector<double> double_vec;

	// sadly we cannot trust boost::timer so we need to do it in a more
	// complicated mannger
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);

	srand(42); // we always want the same seed!

	// thread pool object
	boost::thread_group threads;

	// fill up our lists
	for(uint32_t i = 0; i < ENTRIES; ++i) {
		int_vec.push_back(0);
		double_vec.push_back(static_cast<double>(0));
	}

	int16_t n_threads = 1;
	while(n_threads != -1) {
		std::cout << "sample length: " << TIME << " seconds" << std::endl;
		std::cout << "using " << n_threads << " threads" << std::endl;

		// reset timer and lists
		BOOST_FOREACH(uint64_t i, int_vec) {
			i = 0;
		}
		BOOST_FOREACH(double i, double_vec) {
			i = 0;
		}

		timer.expires_from_now(boost::posix_time::seconds(TIME));

		// count up ints
		for(uint16_t i = 0; i < n_threads ; ++i) {
			threads.create_thread(boost::bind(calcInt, boost::ref(int_vec), i, n_threads));
		}
		timer.wait();
		threads.interrupt_all();
		threads.join_all();

		// output performance report
		std::cout << std::setw(30) << "  int performance: " << calcStats(int_vec)
			<< " (" << calcStats(int_vec, true) << " MegaOps/s)" << std::endl;

		timer.expires_from_now(boost::posix_time::seconds(TIME));

		// count up doubles
		for(uint16_t i = 0; i < n_threads; ++i) {
			threads.create_thread(boost::bind(calcFloat, boost::ref(double_vec), i, n_threads));
		}
		timer.wait();
		threads.interrupt_all();
		threads.join_all();

		std::cout << std::setw(30) << "  float performance: " << calcStats(double_vec)
			<< " (" << calcStats(double_vec, true) << " MegaOps/s)" << std::endl;

		// calculate intermediate score
		score += calcStats(double_vec)+calcStats(int_vec);

		if (n_threads == 1)
			n_threads = cores;
		else if (n_threads == cores)
			n_threads = ENTRIES;
		else if (n_threads == ENTRIES)
			n_threads = -1;
	}
	// calculate final score 
	// 3 thread runs * 2 tests each = 6
	// divide by 1000000 (1E6) for a sane value
	score = (score/6)/1000000;
	std::cout << "score: " << score << std::endl;

	return 0;
}
// vim:set ts=4 sw=4 noet:
