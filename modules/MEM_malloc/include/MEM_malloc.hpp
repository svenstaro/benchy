/*
 * This benchmark tests memory speed using low-level C.
 * It runs a few repeated read/write commands to approximate the memory speed.
 * The score is determined by the approximated memory speed.
 */

#include <boost/cstdint.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <cstdlib>

// Amount of memory to use in bytes.
#define BYTES 157286400 // 150 MiB
// How often to repeat each test.
#define REPEAT 10
// Totally randomly chosen number to fill memory blob with during write tests.
#define RANDOM_NO 42

// Mode can be either "single" or "multi" to indicate multiprocessing off/on.
uint32_t MEM_malloc_run();
