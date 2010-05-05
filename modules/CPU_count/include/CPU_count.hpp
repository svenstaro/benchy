/*
 * This benchmark mainly tests raw CPU speed in a threaded/non-threaded
 * task. It assumes a scenario where a few hundred data containers have
 * to be modified independently of each other using simple arithmetic.
 * It uses OpenMP for doing multiprocessing.
 */

#include <boost/asio.hpp>
#include <boost/cstdint.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <string>
#include <vector>

// number of entries to be processed
#define ENTRIES	150 // needs to be even or threads will fail
// time to run each task in seconds
#define TIME 5.0

// mode can be either "single" or "multi" to indicate multiprocessing off/on
uint32_t CPU_count_run(std::string mode);
