/*
 * This benchmark mainly tests raw CPU speed in a threaded/non-threaded
 * environment. It assumes a scenario where a few hundred data containers have
 * to be modified independently of each other.
 *
 */

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/progress.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <vector>

// number of entries to be processed
#define ENTRIES	150 // needs to be even
// time to run each task in seconds
#define TIME 5.0

uint64_t calcStats(const std::vector<uint64_t>& intvec) {
	uint64_t result = 0;;
	BOOST_FOREACH(uint64_t i, intvec) {
		result += i;
	}
	return result;
}

uint64_t calcStats(const std::vector<double>& doublevec) {
	double result = 0;
	BOOST_FOREACH(double i, doublevec) {
		result += i;
	}
	return (uint64_t)result;
}

void calcInt(std::vector<uint64_t>& intvec, const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	while(!boost::this_thread::interruption_requested()) {
		for(uint32_t i = real_offset; i < numloops; ++i) {
			intvec[i] += 1;
		}
	}
}

void calcFloat(std::vector<double>& doublevec, const uint16_t offset, const uint16_t n_threads) {
	uint16_t numloops = (ENTRIES / n_threads) * (offset + 1);
	uint16_t real_offset = ENTRIES / n_threads * offset;
	while(!boost::this_thread::interruption_requested()) {
		for(uint32_t i = real_offset; i < numloops; ++i) {
			doublevec[i] += 1.0f;
		}
	}
}

int main () {
	uint16_t CORES = boost::thread::hardware_concurrency();
	std::vector<uint64_t> intvec;
	std::vector<double> doublevec;

	// sadly we cannot trust boost::timer so we need to do it in a more
	// complicated mannger
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io);

	srand(42); // we always want the same seed!

	boost::thread_group threads;

	// fill up our lists
	for(uint32_t i = 0; i < ENTRIES; ++i) {
		intvec.push_back(0);
		doublevec.push_back(static_cast<double>(0));
	}

	boost::xtime xt;
	boost::xtime_get(&xt, boost::TIME_UTC);
	xt.sec += 0.1;

	int16_t n_threads = 1;
	while(n_threads != -1) {
		std::cout << "using " << n_threads << " threads" << std::endl;

		// reset timer and lists
		BOOST_FOREACH(uint64_t i, intvec) {
			i = 0;
		}
		BOOST_FOREACH(double i, doublevec) {
			i = 0;
		}

		timer.expires_from_now(boost::posix_time::seconds(5));

		// count up ints
		for(uint16_t i = 0; i < n_threads ; ++i) {
			threads.create_thread(boost::bind(calcInt, boost::ref(intvec), i, n_threads));
		}
		timer.wait();
		threads.interrupt_all();
		threads.join_all();

		std::cout << "  int performance: " << calcStats(intvec) << std::endl;

		timer.expires_from_now(boost::posix_time::seconds(5));

		// count up doubles
		for(uint16_t i = 0; i < n_threads; ++i) {
			threads.create_thread(boost::bind(calcFloat, boost::ref(doublevec), i, n_threads));
		}
		timer.wait();
		threads.interrupt_all();
		threads.join_all();

		std::cout << "  float performance: " << calcStats(doublevec) << std::endl;

		if (n_threads == 1)
			n_threads = CORES;
		else if (n_threads == CORES)
			n_threads = ENTRIES;
		else if (n_threads == ENTRIES)
			n_threads = -1;
	}
	std::cout << "total time: " << "BLAH!" << std::endl;

	return 0;
}
// vim:set ts=4 sw=4 noet:
