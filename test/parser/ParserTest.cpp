#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include<string>
#include "../../src/parser/parser.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ParserTest, ParserTestMain){
	ALSL::Grammar<std::string::iterator> grammar;
	std::string src = "(42 + 1 +) * 31337 + 3 / 4";

	auto itr = src.begin();
	bool res = boost::spirit::qi::phrase_parse(itr, src.end(), grammar, boost::spirit::ascii::space);
	EXPECT_NE(grammar.nodeStk.size(), 0);
	for (auto e : grammar.nodeStk){
		std::cout << *e << std::endl;
	}

	EXPECT_TRUE(res);
	EXPECT_EQ(itr, src.end());

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}