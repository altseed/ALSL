#include "main.hpp"
#include <iostream>
#include <string>
// #include <boost/program_options.hpp>
int main(int argc, char* argv[]) {
	/*
	namespace po = boost::program_options;
	boost::program_options::options_description opt("Options");
	opt.add_options()("input-file", boost::program_options::value<std::string>(), "file name to convert");
	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).
			  options(opt).positional(p).run(), vm);
	po::notify(vm);


	if(!vm.count("input-file")) {
		std::cout << opt << std::endl;
		return 0;
	}
	*/


	// ‰¼
	if(argc < 2) { std::cout << "Usage: alsl filename" << std::endl; return 0; }
	

	decltype(ALSL::parseFile(std::string(argv[1]))) ret;
	try {
		ret = ALSL::parseFile(std::string(argv[1]));
//		ret = ALSL::parseFile(std::string("test.alsl"));

	} catch(...){
		std::cout << "unknown error was occured while parsing." << std::endl;
	}
		ALSL::Generator gen;

		// auto const ret = ALSL::parseFile(vm["input-file"].as<std::string>());
		if(ret) {
			std::cout << "Succeessfully parsed." << std::endl;
			gen.generate(std::cout, *ret);
		} else {
			std::cout << "Failed to parse." << std::endl;

		}

	//} catch(...) {
	//	std::cout << "unknown error was occured while parsing." << std::endl;
	//}

	return 0;
}