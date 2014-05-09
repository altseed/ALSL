
#include<gtest/gtest.h>
#include <string>
#include <boost/optional.hpp>
#include "../../src/AST/ast.hpp"
#include "../../src/parser/parser.hpp"

#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )


TEST(ParserTest, ParserTestIdentif) {

	std::string src =
		R"(
a b(){}
_ _a_b(){}
)";
	try {
		auto res = ALSL::parse("file.alsl", src);
		EXPECT_TRUE(res);
		auto a = **res;

		std::cout << **res << std::endl;
	} catch(std::exception e) { std::cout << e.what() << std::endl; }
}



TEST(ParserTest, ParserTestFor) {

	std::string src =
		R"(

int main(){
	for(int i = 0; i < 10; i += 1) {
		1+1;
	}

	for(;;) 1;
}
)";
	try {
		auto res = ALSL::parse("file.alsl", src);
		EXPECT_TRUE(res);
		//std::cout << **res << std::endl;
	} catch(std::exception e) {std::cout << e.what() << std::endl;}
}

TEST(ParserTest, ParserTestIf) {

	std::string src =
		R"(

int main(){
	if(i <= 3) {
		i = 1;
	} else {
		2;
	}

	if(0)1;
	1;
	if(t <= ex){} else {}
}
)";
	try {
		auto res = ALSL::parse("file.alsl", src);
		EXPECT_TRUE(res);
		//std::cout << **res << std::endl;
	} catch(std::exception e) { std::cout << e.what() << std::endl; }
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	
	std::cin >> c;
	return ret;
}

