#include<gtest/gtest.h>
#include <string>
#include <boost/optional.hpp>
#include "../../src/AST/ast.hpp"
#include "../../src/parser/parser.hpp"
#include "../../src/generator/generator.hpp"
#include "../../src/generator/generatorGLSL.hpp"
#include "../../src/generator/generatorHLSL.hpp"
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )


TEST(Generator, GeneratorTestIf) {

	std::string src =
		R"(
int main(){
	if(true) 0;
	if(true){1;}
	if(true){2;} else 3;
	if(true){4;} else {5;}
	if(true){6;} else if(false){7;}

}
)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	std::cout << "Input: " << std::endl << src << std::endl;
	std::cout << "\n----------\nGLSL: " << std::endl;
	genGL.generate(std::cout, *res);
	std::cout << "\n----------\nHLSL: " << std::endl;
	genHL.generate(std::cout, *res);


}


TEST(Generator, GeneratorTestCall) {

	std::string src =
		R"(
int main(){
	mat4 a_;
	mat4 b;
	mul(a_, b);
	test(a, b);
}
)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	std::cout << "Input: " << std::endl << src << std::endl;
	std::cout << "\n----------\nGLSL: " << std::endl;
	genGL.generate(std::cout, *res);
	std::cout << "\n----------\nHLSL: " << std::endl;
	genHL.generate(std::cout, *res);


}

TEST(Generator, GeneratorTestSimple) {

	std::string src =
R"(
float VSM(float2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;

	float p = 0.0;
	if(t <= ex){ p = 1.0; }

	float variance = ex2 - ex * ex;
	variance = max(variance, 0.4 / (depthParams_.x * depthParams_.x));

	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}
)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	std::cout << "Input: " << std::endl << src << std::endl;
	std::cout << "\n----------\nGLSL: " << std::endl;
	genGL.generate(std::cout, *res);
	std::cout << "\n----------\nHLSL: " << std::endl;
	genHL.generate(std::cout, *res);


}

TEST(Generator, Issue3) {

	std::string src =
		R"(
mat4 calcMatrix(float4 weights, int4 indexes)
{
return matM[indexes.x] * weights.x +
matM[indexes.y] * weights.y +
matM[indexes.z] * weights.z +
matM[indexes.w] * weights.w;
}
)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	std::cout << "Input: " << std::endl << src << std::endl;
	std::cout << "\n----------\nGLSL: " << std::endl;
	genGL.generate(std::cout, *res);
	std::cout << "\n----------\nHLSL: " << std::endl;
	genHL.generate(std::cout, *res);


}

TEST(Generator, Preprocessor) {

	std::string src =
		R"(
float Test()
{
#ifdef ONE
return 1.0;
#endif
return 0.0;
}

)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	std::cout << "Input: " << std::endl << src << std::endl;
	std::cout << "\n----------\nGLSL: " << std::endl;
	genGL.generate(std::cout, *res);
	std::cout << "\n----------\nHLSL: " << std::endl;
	genHL.generate(std::cout, *res);


}



int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}

