#pragma once
#ifndef ALSL_GENERATOR_GLSL_HPP
#define ALSL_GENERATOR_GLSL_HPP
#include "generator.hpp"
#include <functional>
#include <unordered_map>
namespace ALSL {
	class GeneratorGLSL: public Generator {
	private:
		typedef std::function<void(std::ostream& os, std::shared_ptr<Node> const node)> GeneratorFuncType;
		std::unordered_map<std::string, GeneratorFuncType> funcdict;


		std::unordered_map<std::string, std::string> typedict;
	protected:
		virtual void gen_call(std::ostream& os, std::shared_ptr<Node> const node) override;

		virtual void gen_floatLit(std::ostream& os, std::shared_ptr<Node> const node) override;


		virtual void gen_type(std::ostream& os, std::shared_ptr<Node> const node) override;

	public:
		GeneratorGLSL() {
			typedict.emplace("void", "void");
			typedict.emplace("bool", "bool");
			typedict.emplace("bool2", "bvec2");
			typedict.emplace("bool3", "bvec3");
			typedict.emplace("bool4", "bvec4");
			typedict.emplace("int", "int");
			typedict.emplace("int2", "ivec2");
			typedict.emplace("int3", "ivec3");
			typedict.emplace("int4", "ivec4");
			typedict.emplace("uint", "uint");
			typedict.emplace("uint2", "uvec2");
			typedict.emplace("uint3", "uvec3");
			typedict.emplace("uint4", "uvec4");
			typedict.emplace("float", "float");
			typedict.emplace("float2", "vec2");
			typedict.emplace("float3", "vec3");
			typedict.emplace("float4", "vec4");
			typedict.emplace("float22", "mat2");
			typedict.emplace("float33", "mat3");
			typedict.emplace("float44", "mat4");
			typedict.emplace("Texture2D", "sampler2D");


			funcdict.emplace("mul", [this](std::ostream& os, std::shared_ptr<Node> const node){
				auto itr = node->contents.cbegin();
				itr++;
				genNextNode(os, *itr);
				os << " * ";
				itr++;
				genNextNode(os, *itr);
			});

			funcdict.emplace(
				"bool2",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("bvec2", os, node);}
			);
			funcdict.emplace(
				"bool3",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("bvec3", os, node); }
			);
			funcdict.emplace(
				"bool4",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("bvec4", os, node); }
			);

			funcdict.emplace(
				"int2",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("ivec2", os, node); }
			);
			funcdict.emplace(
				"int3",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("ivec3", os, node); }
			);
			funcdict.emplace(
				"int4",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("ivec4", os, node); }
			);

			funcdict.emplace(
				"uint2",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("uvec2", os, node); }
			);
			funcdict.emplace(
				"uint3",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("uvec3", os, node); }
			);
			funcdict.emplace(
				"uint4",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("uvec4", os, node); }
			);

			funcdict.emplace(
				"float2",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("vec2", os, node); }
			);
			funcdict.emplace(
				"float3",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("vec3", os, node); }
			);
			funcdict.emplace(
				"float4",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("vec4", os, node); }
			);

			funcdict.emplace(
				"float22",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("mat2", os, node); }
			);
			funcdict.emplace(
				"float33",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("mat3", os, node); }
			);
			funcdict.emplace(
				"float44",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("mat4", os, node); }
			);

			funcdict.emplace(
				"GetTextureSize",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {
					this->addPredefinedFunc("GetTextureSize");
					auto itr = node->contents.cbegin();
					itr++;
					os << "GetTextureSize(";
					genNextNode(os, *itr);
					itr++;
					if(itr != node->contents.cend()) {assert(!"GetTextureSize takes only 1 argument."); /* err */}
					os << ")";
				}
			);


			registerPredefinedFunc("GetTextureSize",
R"(uvec2 GetTextureSize(sampler2D texture_){
	return (uvec2)textureSize(texture_, 0);
})");
		}
	};

}

#endif