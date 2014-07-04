#pragma once
#ifndef ALSL_GENERATOR_HLSL_HPP
#define ALSL_GENERATOR_HLSL_HPP
#include "generator.hpp"
#include <cstdlib>
#include <iostream>
#include <functional>
#include <unordered_map>
namespace ALSL {
	class GeneratorHLSL: public Generator {
	private:
		typedef std::function<void(std::ostream& os, std::shared_ptr<Node> const node)> GeneratorFuncType;
		std::unordered_map<std::string, GeneratorFuncType> funcdict;


		std::unordered_map<std::string, std::string> typedict;
	protected:
		virtual void gen_call(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_type(std::ostream& os, std::shared_ptr<Node> const node);

	public:
		GeneratorHLSL() {
			typedict.emplace("void", "void");
			typedict.emplace("bool", "bool");
			typedict.emplace("bool2", "bool2");
			typedict.emplace("bool3", "bool3");
			typedict.emplace("bool4", "bool4");
			typedict.emplace("int", "int");
			typedict.emplace("int2", "int2");
			typedict.emplace("int3", "int3");
			typedict.emplace("int4", "int4");
			typedict.emplace("uint", "uint");
			typedict.emplace("uint2", "uint2");
			typedict.emplace("uint3", "uint3");
			typedict.emplace("uint4", "uint4");
			typedict.emplace("float", "float");
			typedict.emplace("float2", "float2");
			typedict.emplace("float3", "float3");
			typedict.emplace("float4", "float4");
			typedict.emplace("float22", "row_major float2x2");
			typedict.emplace("float33", "row_major float3x3");
			typedict.emplace("float44", "row_major float4x4");
			typedict.emplace("Texture2D", "Texture2D");
			typedict.emplace("Sampler", "SamplerState");

			
			funcdict.emplace("mul", [this](std::ostream& os, std::shared_ptr<Node> const node) {
				auto itr = node->contents.cbegin();
				itr++;
				os << "mul(";
				genNextNode(os, *itr);
				os << ", ";
				itr++;
				genNextNode(os, *itr);
				os << ")";
			});
			

			funcdict.emplace(
				"float22",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("(row_major float2x2)float2x2", os, node); }
			);
			funcdict.emplace(
				"float33",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("(row_major float3x3)float3x3", os, node); }
			);
			funcdict.emplace(
				"float44",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {extractFunc("(row_major float4x4)float4x4", os, node); }
			);

			funcdict.emplace(
				"GetTextureSize",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {
				this->addPredefinedFunc("GetTextureSize");
				auto itr = node->contents.cbegin();
				itr++;
				if(itr == node->contents.cend()) {
					std::cerr << "Error: GetTextureSize takes 2 arguments." << std::endl; std::exit(1); // err
				}
				os << "GetTextureSize(";
				genNextNode(os, *itr);
				itr++;
				if(itr == node->contents.cend()) { assert(!"GetTextureSize takes 2 arguments."); /* err */ }
				os << ", ";
				genNextNode(os, *itr);
				itr++;
				if(itr != node->contents.cend()) { assert(!"GetTextureSize takes 2 arguments."); /* err */ }
				os << ")";
			}
			);

			funcdict.emplace(
				"SampleTexture",
				[this](std::ostream& os, std::shared_ptr<Node> const node) {
				this->addPredefinedFunc("SampleTexture");
				auto itr = node->contents.cbegin();
				itr++;
				if(itr == node->contents.cend()) {
					std::cerr << "GetTextureSize takes 3 arguments." << std::endl; std::exit(1); // err
				}
				os << "SampleTexture(";
				genNextNode(os, *itr);
				itr++;
				if(itr == node->contents.cend()) { assert(!"GetTextureSize takes 3 arguments."); /* err */ }
				os << ", ";
				genNextNode(os, *itr);
				itr++;
				os << ", ";
				if(itr == node->contents.cend()) { assert(!"GetTextureSize takes 3 arguments."); /* err */ }
				genNextNode(os, *itr);
				itr++;
				if(itr != node->contents.cend()) { assert(!"GetTextureSize takes 3 arguments."); /* err */ }
				os << ")";
			}
			);

			registerPredefinedFunc("GetTextureSize",
R"(uint2 GetTextureSize(Texture2D texture_, SamplerState sampler_){
	uint width, height;
	texture_.GetDimension(width, height);
	return uint2(width, height);
})");
			registerPredefinedFunc("SampleTexture",
R"(float4 SampleTexture(Texture2D texture_, SamplerState sampler_, float2 uv_) {
	return texture_.Sample(sampler_, uv_);
})");

		}
	};

}

#endif