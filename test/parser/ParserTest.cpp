#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include "../../src/parser/parser.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ParserTest, ParserTestMain){
	ASSERT_EQ(1, 1);

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}