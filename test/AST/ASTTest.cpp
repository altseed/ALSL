#define _VARIADIC_MAX  10

#include<gtest/gtest.h>
#include "../../src/AST/ast.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )
TEST(ASTTest, ASTTestMain){
	auto opd1 = ALSL::Node::makeNode(true, ALSL::Tokens::intLit, 42);
	auto opd2 = ALSL::Node::makeNode(true, ALSL::Tokens::intLit, 31337);
	auto op = ALSL::Node::makeNode(true, ALSL::Tokens::opAdd, opd1, opd2);
	
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
