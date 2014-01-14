#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include "../../src/AST/ast.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ASTTest, ASTTestMain){
	auto e = std::make_shared<ALSL::Node>(false, ALSL::Tokens::intLit);
	e->contents.emplace_back(42);
	std::cout << *e << std::endl;
	ASSERT_EQ(1, 1);

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}