#include<gtest/gtest.h>
#include <string>
#include <boost/optional.hpp>
#include <sstream>
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



TEST(Generator, GeneratorTestFor) {

	std::string src =
		R"(
int main(){
	for(1;2;3) 4;
	for(;;) {5;}
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


TEST(Generator, GeneratorTestWhile) {

	std::string src =
		R"(
int main(){
	while(true)1;
	while(false){2;3;}
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


TEST(Generator, GeneratorTestDoWhile) {

	std::string src =
		R"(
int main(){
	do{
		1;
		2;
	} while(true);
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
TEST(Generator, GeneratorTestStruct) {

	std::string src =
		R"(
struct S {
	int i;
	vec2 v;
}

int main(){
	S s;
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

TEST(Generator, GeneratorTestCompound) {

	std::string src =
		R"(
struct S {
	int i;
	vec2 v;
}

int main(){
	S s;
	int i = 0;
	while(i < 10) {
		for(int j = 0; j <= 10; j += 1){s.i += i * j;}
		i += 1;
		if(false){break;}
	}
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
	float44 a_;
	float44 b;
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
float44 calcMatrix(float4 weights, int4 indexes)
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


TEST(Generator, TypeConvert) {

	std::string src =
		R"(
void Test()
{
	Texture2D texture;
	float44 mat;
}

)";
	std::stringstream resActualHL;
	std::stringstream resActualGL;
	std::string resExpectHL = R"(void Test()
{
	Texture2D texture;
	row_major float4x4 mat;
}

)";
	std::string resExpectGL = R"(void Test()
{
	sampler2D texture;
	mat4 mat;
}

)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	genGL.generate(resActualGL, *res);
	EXPECT_EQ(resExpectGL, resActualGL.str());

	genHL.generate(resActualHL, *res);
	EXPECT_EQ(resExpectHL, resActualHL.str());

}



TEST(Generator, ParamQualify) {

	std::string src =
		R"(
int Test(in int a, in int b, out int c, inout int d, int e)
{
	c = a + b;
	d += a + b + c;
	return a + b + c + d;
}

)";
	std::stringstream resActualHL;
	std::stringstream resActualGL;
	std::string resExpectHL = R"(int Test(in int a, in int b, out int c, inout int d, int e)
{
	c = a + b;
	d += a + b + c;
	return a + b + c + d;
}

)";
	std::string resExpectGL = R"(int Test(in int a, in int b, out int c, inout int d, int e)
{
	c = a + b;
	d += a + b + c;
	return a + b + c + d;
}

)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	genGL.generate(resActualGL, *res);
	EXPECT_EQ(resExpectGL, resActualGL.str());

	genHL.generate(resActualHL, *res);
	EXPECT_EQ(resExpectHL, resActualHL.str());

}


TEST(Generator, TypeConstructor) {

	std::string src =
		R"(
int Test()
{
	bool2 b2 = bool2(true, false);
	bool3 b3 = bool3(true, false, true);
	bool4 b4 = bool4(true, false, true, true);
	int2 i2 = int2(1, 2);
	int2 i3 = int3(1, 2, 3);
	int4 i4 = int4(1, 2, 3, 4);
	uint2 u2 = uint2(1, 2);
	uint2 u3 = uint3(1, 2, 3);
	uint4 u4 = uint4(1, 2, 3, 4);
	float2 f2 = float2(1.0f, 1.0f);
	float3 f3 = float3(1.0f, 1.0f, 0.0f);
	float3 f4 = float4(1.0f, 1.0f, 0.0f, 2.0f);
	float22 m2 = float22(f2, f2);
	float33 m3 = float33(f3, f3, f3);
	float44 m4 = float44(f4, f4, f4, f4);
}

)";
	std::stringstream resActualHL;
	std::stringstream resActualGL;
	std::string resExpectHL = R"(int Test()
{
	bool2 b2 = bool2(true, false);
	bool3 b3 = bool3(true, false, true);
	bool4 b4 = bool4(true, false, true, true);
	int2 i2 = int2(1, 2);
	int2 i3 = int3(1, 2, 3);
	int4 i4 = int4(1, 2, 3, 4);
	uint2 u2 = uint2(1, 2);
	uint2 u3 = uint3(1, 2, 3);
	uint4 u4 = uint4(1, 2, 3, 4);
	float2 f2 = float2(1.00000f, 1.00000f);
	float3 f3 = float3(1.00000f, 1.00000f, 0.000000f);
	float3 f4 = float4(1.00000f, 1.00000f, 0.000000f, 2.00000f);
	row_major float2x2 m2 = (row_major float2x2)float2x2(f2, f2);
	row_major float3x3 m3 = (row_major float3x3)float3x3(f3, f3, f3);
	row_major float4x4 m4 = (row_major float4x4)float4x4(f4, f4, f4, f4);
}

)";
	std::string resExpectGL = R"(int Test()
{
	bvec2 b2 = bvec2(true, false);
	bvec3 b3 = bvec3(true, false, true);
	bvec4 b4 = bvec4(true, false, true, true);
	ivec2 i2 = ivec2(1, 2);
	ivec2 i3 = ivec3(1, 2, 3);
	ivec4 i4 = ivec4(1, 2, 3, 4);
	uvec2 u2 = uvec2(1, 2);
	uvec2 u3 = uvec3(1, 2, 3);
	uvec4 u4 = uvec4(1, 2, 3, 4);
	vec2 f2 = vec2(1.00000, 1.00000);
	vec3 f3 = vec3(1.00000, 1.00000, 0.000000);
	vec3 f4 = vec4(1.00000, 1.00000, 0.000000, 2.00000);
	mat2 m2 = mat2(f2, f2);
	mat3 m3 = mat3(f3, f3, f3);
	mat4 m4 = mat4(f4, f4, f4, f4);
}

)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	genGL.generate(resActualGL, *res);
	EXPECT_EQ(resExpectGL, resActualGL.str());

	genHL.generate(resActualHL, *res);
	EXPECT_EQ(resExpectHL, resActualHL.str());

}


TEST(Generator, GetTextureSize) {

	std::string src =
		R"(
uint Test(in Texture2D tex)
{
	uint2 size = GetTextureSize(tex);
	return size.x;
}

)";
	std::stringstream resActualHL;
	std::stringstream resActualGL;
	std::string resExpectHL = R"(uint2 GetTextureSize(Texture2D texture_){
	uint width, height;
	texture_.GetDimension(width, height);
	return uint2(width, height);
}
uint Test(in Texture2D tex)
{
	uint2 size = GetTextureSize(tex);
	return size.x;
}

)";
	std::string resExpectGL = R"(uvec2 GetTextureSize(sampler2D texture_){
	return (uvec2)textureSize(texture_, 0);
}
uint Test(in sampler2D tex)
{
	uvec2 size = GetTextureSize(tex);
	return size.x;
}

)";

	auto res = ALSL::parse("file.alsl", src);
	EXPECT_TRUE(res);
	ALSL::Generator gen;
	ALSL::GeneratorGLSL genGL;
	ALSL::GeneratorHLSL genHL;
	// std::cout << **res << std::endl << "------------" << std::endl;
	genGL.generate(resActualGL, *res);
	EXPECT_EQ(resExpectGL, resActualGL.str());

	genHL.generate(resActualHL, *res);
	EXPECT_EQ(resExpectHL, resActualHL.str());

}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	auto ret = RUN_ALL_TESTS();
	char c;
	std::cin >> c;
	return ret;
}

