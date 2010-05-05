#include <boost/program_options.hpp>

#include <iostream>
#include <string>

#include "TestManager.hpp"

namespace po = boost::program_options;

const po::variables_map usage(int ac, char* av[]) {
	// Declare the supported options.
	po::options_description desc("benchy desktop benchmarking tool\
	\n\nAllowed options");
	desc.add_options()
		("help,h", "this message")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		exit(0);
	}

	return vm;
}

void printScores(score_map scores) {
	std::cout << "Your system received a benchy score of: " << scores["benchy_score"] << std::endl;
	std::cout << "Detail scores: " << std::endl;
	std::cout << "CPU single core score: " << scores["cpu_single_core_score"] << std::endl;
	std::cout << "CPU multi core score: " << scores["cpu_multi_core_score"] << std::endl;
	std::cout << "Memory score: " << scores["mem_score"] << std::endl;
	std::cout << "Harddrive score: " << scores["hdd_score"] << std::endl;
	std::cout << "2D score: " << scores["2d_score"] << std::endl;
	std::cout << "3D score: " << scores["3d_score"] << std::endl;
}

int main(int argc, char *argv[]) {
	// get program options
	po::variables_map opts = usage(argc, argv);

	TestManager TestMan;
	TestMan.RunTests();

	score_map scores;
	scores = TestMan.GetScores();
	printScores(scores);

	return 0;
}
