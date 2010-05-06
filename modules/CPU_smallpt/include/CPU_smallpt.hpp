/*
 * This is a benchmark based on Kevin Beason's smallpt, a raytracer in 99 lines
 * of C++.
 * The score is determined by the time taken to render the scene.
 * It uses OpenMP for doing multiprocessing.
 */

#include <boost/cstdint.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

// mode can be either "single" or "multi" to indicate multiprocessing off/on
uint32_t CPU_smallpt_run(std::string mode);
