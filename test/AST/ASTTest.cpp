#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include "../../src/AST/ast.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ASTTest, ASTTestMain){
	auto op = std::make_shared<ALSL::Node>(false, ALSL::Tokens::opAdd);
	auto e1 = std::make_shared<ALSL::Node>(true, ALSL::Tokens::intLit);
	auto e2 = std::make_shared<ALSL::Node>(true, ALSL::Tokens::intLit);
	e1->contents.emplace_back(42);
	e2->contents.emplace_back(31337);
	op->contents.emplace_back(e1);
	op->contents.emplace_back(e2);
	std::cout << *op << std::endl;
	ASSERT_EQ(1, 1);

}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c = '\0';
	std::cin >> c;
	return ret;
}
