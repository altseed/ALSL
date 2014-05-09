#include "main.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>
int main(int argc, char* argv[]) {

	namespace po = boost::program_options;
	boost::program_options::options_description opt("Options");
	opt.add_options()("input-file", boost::program_options::value<std::string>(), "file name to convert");
	opt.add_options()("output-file,o", boost::program_options::value<std::string>(), "file name of the output(without extentions)");
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



	// ‰¼
	//if(argc < 2) { std::cout << "Usage: alsl filename" << std::endl; return 0; }
	

	decltype(ALSL::parseFile(std::string(argv[1]))) ret;
	try {
		// ret = ALSL::parseFile(std::string(argv[1]));
		ret = ALSL::parseFile(vm["input-file"].as<std::string>());


	} catch(...){
		std::cout << "unknown error was occured while parsing." << std::endl;
	}
		ALSL::GeneratorGLSL genGL;
		ALSL::GeneratorHLSL genHL;

		// auto const ret = ALSL::parseFile(vm["input-file"].as<std::string>());
		if(ret) {
			std::cout << "Succeessfully translated." << std::endl;
			std::string fname = (!vm.count("output-file")) ? "a" : vm["output-file"].as<std::string>();
			std::ofstream ofsGL(fname + ".fs", std::ofstream::out);
			genGL.generate(ofsGL, *ret);
			ofsGL.close();
			std::ofstream ofsHL(fname + ".fx", std::ofstream::out);
			genHL.generate(ofsHL, *ret);
			ofsHL.close();

		} else {
			std::cout << "Failed to translate." << std::endl;

		}

	//} catch(...) {
	//	std::cout << "unknown error was occured while parsing." << std::endl;
	//}

	return 0;
}