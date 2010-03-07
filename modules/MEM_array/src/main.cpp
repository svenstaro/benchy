#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <list>

#define ITERATIONS 4000000

int main () {
	boost::progress_timer timer;

	std::list<uint32_t> int_list;
	std::list<long double> double_list;
	std::list<char> char_list;

	srand(42);

	for(uint32_t i = 0; i < ITERATIONS; ++i) {
		uint32_t rnd = rand();
		int_list.push_back(rnd);
	}

	for(uint32_t i = 0; i < ITERATIONS; ++i) {
		long double rnd = static_cast<long double>(rand());
		double_list.push_back(rnd);
	}

	for(uint32_t i = 0; i < ITERATIONS; ++i) {
		char rnd = static_cast<char>(rand());
		if (std::isalnum(rnd)) {
			char_list.push_back(rnd);
		}
	}

	BOOST_FOREACH(uint32_t a, int_list) {
		a *= rand();
	}
	BOOST_FOREACH(long double a, double_list) {
		a *= static_cast<long double>(rand());
		a = sqrt(a);
		a = sin(a);
	}
	BOOST_FOREACH(char a, char_list) {
		char rnd = static_cast<char>(rand());
		if (std::isalnum(rnd)) 
			a = rnd;
	}

	//int_list.sort();
	//int_list.reverse();
	//char_list.sort();
	//char_list.reverse();
	//double_list.sort();
	//double_list.reverse();
}
