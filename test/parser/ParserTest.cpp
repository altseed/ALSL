
#include<gtest/gtest.h>
#include <string>
#include <boost/optional.hpp>
#include "../../src/AST/ast.hpp"
#include "../../src/parser/parser.hpp"

#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ParserTest, ParserTestMain){

	std::string src =
		R"(
	struct S{
		int a;
		int b;
	}
	int main(){return 0;}
)";

	auto res = ALSL::parse(src);
	EXPECT_TRUE(res);
	std::cout << **res << std::endl;
	

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}

