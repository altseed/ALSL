#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include<string>
#include "../../src/parser/parser.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ParserTest, ParserTestMain){
	ALSL::Grammar<boost::spirit::line_pos_iterator<std::string::iterator>> grammar;
	ALSL::Skipper< boost::spirit::line_pos_iterator<std::string::iterator>> skipper;
	std::shared_ptr<ALSL::Node> result;
	std::string src =
		R"(
/*

aaaa
/*bbbb*/

*/
aaa(1)
)";

	auto itr = boost::spirit::line_pos_iterator<std::string::iterator>(src.begin());
	
	auto const end = boost::spirit::line_pos_iterator<std::string::iterator>(src.end());
	bool res = boost::spirit::qi::phrase_parse(itr, end, grammar, skipper, result);
	std::cout << *result << std::endl;

	EXPECT_TRUE(res);
	EXPECT_EQ(itr, end);

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}