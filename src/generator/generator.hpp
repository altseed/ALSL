#pragma once
#ifndef ALSL_GENERATOR_HPP
#define ALSL_GENERATOR_HPP

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include "../AST/ast.hpp"
#include "../parser/parser.hpp"

namespace ALSL {

	class Generator {
	private:
		std::unordered_map<std::string, std::string> predefinedFuncDict;
		std::unordered_set<std::string> registeredPredefinedFuncSet;


	protected:
		int indent = 0;

		virtual void generateImpl(std::ostream& os, std::shared_ptr<Node> node);
		void genNextNode(std::ostream& os, NodeContent const& node) {
			generateImpl(os, boost::get<std::shared_ptr<Node> const>(node));
		}
		bool isNode(NodeContent const& node) const {return node.which() == NodeContentTypes::NextNode;}
		Node const& getNode(NodeContent const& node) const{return *(boost::get<std::shared_ptr<Node> const>(node));}

		void genNL(std::ostream& os) const {
			os << "\n";
		}
		void genIndent(std::ostream& os) const {
			for(int i = 0; i < indent; i++) {
				os << '\t';

			}
		}

		void registerPredefinedFunc(std::string const&& name, std::string const&& body) {
			if(predefinedFuncDict.find(name) != predefinedFuncDict.cend()) { return; }
			predefinedFuncDict.insert(std::make_pair(name, body));
		}
		void addPredefinedFunc(std::string const&& name) {
			if(registeredPredefinedFuncSet.find(name) != registeredPredefinedFuncSet.cend()) {return;}
			registeredPredefinedFuncSet.insert(name);
		}

		void printPredefinedFunc(std::ostream& os) {
			for(auto const& e : registeredPredefinedFuncSet) {
				if(predefinedFuncDict.find(e) == predefinedFuncDict.cend()) { continue; } //err
				os << predefinedFuncDict.find(e)->second << "\n";
			}

		}

		virtual void gen_none(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_intLit(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_floatLit(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_doubleLit(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_strLit(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_identif(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_paren(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_call(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_arraySubscript(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_swizzleOp(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_dataMember(std::ostream& os, std::shared_ptr<Node> const node);


		virtual void gen_opPos(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opNeg(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opInv(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opNot(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opMult(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opDiv(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opMod(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opAdd(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opSub(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opLsh(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opRsh(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opGt(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opLt(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opGte(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opLte(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opEq(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opNeq(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opBitAnd(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opBitXor(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opBitOr(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opLogicAnd(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opLogicOr(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opSelect(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opAddAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opSubAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opMultAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opDivAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opModAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opLshAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opRshAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opBitAndAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opBitOrAssign(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_opBitXorAssign(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_opSeq(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_seq(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_stxIf(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_stxWhile(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_stxFor(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_stxDoWhile(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_stxFunc(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_stxStruct(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_type(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_declVar(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_declConst(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_declProp(std::ostream& os, std::shared_ptr<Node> const node);

		virtual void gen_kwdReturn(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_kwdBreak(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_kwdContinue(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_kwdIn(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_kwdOut(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_kwdInout(std::ostream& os, std::shared_ptr<Node> const node);
		virtual void gen_preprocessor(std::ostream& os, std::shared_ptr<Node> const node);


		virtual void extractFunc(std::string const&& name, std::ostream& os, std::shared_ptr<Node> const node) {
			auto itr = node->contents.cbegin();
			auto end = node->contents.cend();
			itr++; // discard function name
			os << name << "(";
			if(itr != end) {
				genNextNode(os, *(itr++));

				while(itr != end) {
					os << ", ";
					genNextNode(os, *(itr++));
				}
			}
			os << ")";

		}


	public:
		virtual void generate(std::ostream& os, std::shared_ptr<Node> node);
		// virtual void 
		virtual ~Generator(){}
	};
}


#endif
