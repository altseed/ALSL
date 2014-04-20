#include<gtest/gtest.h>
#include <string>
#include <boost/optional.hpp>
#include "../../src/AST/ast.hpp"
#include "../../src/parser/parser.hpp"
#include "../../src/generator/generator.hpp"

#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(Generator, GeneratorTestMain) {

	std::string src =
R"(
	struct RGBA{
		int r;
		int g;
		int b;
		int a;
	};
	int main(){
		if(1+1==2) {
			test();
		} else if(1+2==3){
			test2();
		} else {}

		while(1){
			test3();
		}
	
	return 0;
	}
)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	std::cout << **res << std::endl << "------------" << std::endl;

	gen.generate(std::cout, *res);


}




int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}

