#include "generatorGLSL.hpp"

namespace ALSL {
	using boost::get;

	// GLSLのfloatにはfサフィクスがつかないので、とる
	void GeneratorGLSL::gen_floatLit(std::ostream& os, std::shared_ptr<Node> const node) { os << std::showpoint << get<float>(node->contents.back()); }

	// 型名を変換する
	void GeneratorGLSL::gen_type(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		auto const typeName = boost::get<std::shared_ptr<Node> const>(*itr);
		if(typeName->token != Tokens::identif) {/*err*/return;}
		auto const typeNameStr = get<std::string>(typeName->contents.back());
		if(typedict.find(typeNameStr) == typedict.end()) {
			os << typeNameStr;
		} else {
			os << typedict.find(typeNameStr)->second;
		}
		itr++;
		for(; itr != node->contents.cend(); itr++) {
			os << "[";
			genNextNode(os, *itr); // index of array type
			os << "]";
		}
	}

	void GeneratorGLSL::gen_call(std::ostream& os, std::shared_ptr<Node> const node) {
		auto nodeCont = *(node->contents.cbegin());
		if(nodeCont.which() != NodeContentTypes::NextNode) {
			// err
			return;
		}
		auto funcNameNode = boost::get<std::shared_ptr<Node> const>(nodeCont);

		auto funcNameCont = *(funcNameNode->contents.cbegin());

		if(funcNameCont.which() != NodeContentTypes::Identifier) {
			// err
			return;
		}
		auto funcName = boost::get<std::string>(funcNameCont);

		if(funcdict.find(funcName) == funcdict.end()) {
			Generator::gen_call(os, node);
		} else {
			(funcdict.find(funcName)->second)(os, node);
		}
	}
}