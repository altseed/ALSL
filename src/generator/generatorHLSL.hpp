#pragma once
#ifndef ALSL_GENERATOR_HLSL_HPP
#define ALSL_GENERATOR_HLSL_HPP
#include "generator.hpp"
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

		virtual void gen_floatLit(std::ostream& os, std::shared_ptr<Node> const node);


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
			typedict.emplace("mat2", "row_major float2x2");
			typedict.emplace("mat3", "row_major float3x3");
			typedict.emplace("mat4", "row_major float4x4");

			funcdict.emplace("mul", [this](std::ostream& os, std::shared_ptr<Node> const node) {
				auto itr = node->contents.cbegin();
				itr++;
				os << "mul(";
				this->generate(os, boost::get<std::shared_ptr<Node> const>(*itr));
				os << ", ";
				itr++;
				this->generate(os, boost::get<std::shared_ptr<Node> const>(*itr));
				os << ")";
			});

		}
	};

}

#endif