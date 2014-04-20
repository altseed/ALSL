#pragma once
#ifndef ALSL_AST_HPP
#define ALSL_AST_HPP


#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

namespace ALSL{
	// todo: helperånÇ™ó≠Ç‹Ç¡ÇΩÇÁèWÇﬂÇƒëºÇÃÉwÉbÉ_Ç…à⁄Ç∑
	class Printable{
	public:
		static int& getIndent(){ static int i; return i; }
		static void incIndent(){ getIndent()++; }
		static void decIndent(){ getIndent()--; }
		static void initIndent(){ getIndent() = 0; }
		virtual void print(std::ostream&) = 0;
		void printIndent(std::ostream& os) {
			const int ind = getIndent();
			for (int i = 0; i < ind; i++) {
				os << "    ";
			}
		}
	};
	struct Node;
	typedef boost::variant<std::shared_ptr<Node>, int, float, double, std::string> NodeContent;
	namespace NodeContentTypes{
		int const NextNode = 0;
		int const Int = NextNode + 1;
		int const Float = Int + 1;
		int const Double = Float + 1;
		int const Identifier = Double + 1;
	}
	enum class Tokens{
		none,
		intLit,
		floatLit,
		doubleLit,
		strLit,
		identif,

		paren, // paren (exp opd)

		call, // function call (exp funcName, exp arg, exp arg,...)
		arraySubscript, // [] (exp arrayName, exp index)
		swizzleOp, // swizzle operation  (exp dataName, identif swizzle)
		dataMember, // . (exp dataName, identif memberName)

		// arithm, bitwise and logical operators
		opPos, // unary + (exp opd) 
		opNeg, // unary - (exp opd)
		opInv, // ~ (exp opd)
		opNot, // ! (exp opd)
		
		opMult, // * (exp opd1, exp opd2)
		opDiv, // / (exp opd1, exp opd2)
		opMod, // %

		opAdd, // +
		opSub, // -

		opLsh, // <<
		opRsh, // >>

		opGt, // >
		opLt, // <
		opGte, // >=
		opLte, // <=

		opEq, // ==
		opNeq, // !=

		opBitAnd, // &

		opBitXor, // ^

		opBitOr, // |

		opLogicAnd, // &&
		opLogicOr, // ||

		opSelect, // ?:

		opAssign, // =
		opAddAssign, // +=
		opSubAssign, // -=
		opMultAssign, // *=
		opDivAssign, // /=
		opModAssign, // %=
		opLshAssign, // <<=
		opRshAssign, // >>=
		opBitAndAssign, // &=
		opBitOrAssign, // |=
		opBitXorAssign, // ^=

		opSeq, // ,

		seq, // ;

		stxIf, // if(exp cond, block true, [block false])
		stxWhile, // while
		stxFor, // for
		stxDoWhile, // do-while // {}Ç™è»ó™Ç≈Ç´Ç»Ç¢
		stxFunc, // function declaration
		stxStruct, // struct declaration
		type,
		declVar, // variable declaration
		declConst, // constant value declaration
		declProp, // property declaration
		
		kwdReturn, // return(exp ret)
		kwdBreak, // break
		kwdContinue, // continue

	};
	namespace Internal{
		static const std::string tokens2Str[] = {"none", "intLit", "floatLit", "doubleLit", "strLit", "identif", "paren", "call", "arraySubscript", "swizzleOp", "dataMember", "opPos", "opNeg", "opInv", "opNot", "opMult", "opDiv", "opMod", "opAdd", "opSub", "opLsh", "opRsh", "opGt", "opLt", "opGte", "opLte", "opEq", "opNeq", "opBitAnd", "opBitXor", "opBitOr", "opLogicAnd", "opLogicOr", "opSelect", "opAssign", "opAddAssign", "opSubAssign", "opMultAssign", "opDivAssign", "opModAssign", "opLshAssign", "opRshAssign", "opBitAndAssign", "opBitOrAssign", "opBitXorAssign", "opSeq", "seq", "stxIf", "stxWhile", "stxFor", "stxDoWhile", "stxFunc", "stxStruct", "type", "declVar", "declConst", "declProp", "kwdReturn", "kwdBreak", "kwdContinue"};

	}

	struct Node: public Printable {
	protected:

	public:
		bool isConst = false;
		Tokens token = Tokens::none;
		std::list<NodeContent> contents;

		Node(bool const isConst_, Tokens const token_) : isConst(isConst_), token(token_){}

		static void makeNodeImpl(std::shared_ptr<Node>){}

		template<typename T, typename... Args> static void makeNodeImpl(std::shared_ptr<Node> node, T const t, Args const... args) {
			node->contents.push_back(t);
			makeNodeImpl(node, args...);
		}
		template<typename... Args> static std::shared_ptr<Node> makeNode(bool const isConst_, Tokens const token_, Args const... args) {
			auto node = std::make_shared<Node>(isConst_, token_);
			makeNodeImpl(node, args...);
			return node;
		}
		void print(std::ostream& os) override;

		std::shared_ptr<Node> getPtr() { return std::shared_ptr<Node>(this); }

	};

	typedef std::shared_ptr<Node> SpNode;

}

std::ostream& operator<<(std::ostream& os, ALSL::Printable& self);

#endif