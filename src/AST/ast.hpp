#pragma once

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

namespace ALSL{
	// todo: helper�n�����܂�����W�߂đ��̃w�b�_�Ɉڂ�
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
		identif,

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

		stxIf, // if
		stxWhile, // while
		stxFor, // for
		stxDoWhile, // do-while
		stxFunc, // function declaration

	};
	namespace Internal{
		static const std::string tokens2Str[] = { "none", "intLit", "floatLit", "doubleLit", "identif", "call", "arraySubscript", "swizzleOp", "dataMember", "opPos", "opNeg", "opInv", "opNot", "opMult", "opDiv", "opMod", "opAdd", "opSub", "opLsh", "opRsh", "opGt", "opLt", "opGte", "opLte", "opEq", "opNeq", "opBitAnd", "opBitXor", "opBitOr", "opLogicAnd", "opLogicOr", "opSelect", "opAssign", "opAddAssign", "opSubAssign", "opMultAssign", "opDivAssign", "opModAssign", "opLshAssign", "opRshAssign", "opBitAndAssign", "opBitOrAssign", "opBitXorAssign", "opSeq", "seq", "stxIf", "stxWhile", "stxFor", "stxDoWhile", "stxFunc" };

	}

	struct Node: public Printable {
	protected:

	public:
		bool isConst = false;
		Tokens token = Tokens::none;
		std::list<NodeContent> contents;

		Node(bool isConst_, Tokens token_) : isConst(isConst_), token(token_){}

		void print(std::ostream& os) override{
			os << std::boolalpha;
			os << "{\n";
			incIndent();
			printIndent(os);
			os << "\"Node\": {\n";
			incIndent();
			printIndent(os);
			os << "\"isConst\": " << isConst << ",\n";
			printIndent(os);
			os << "\"token\": \"" << Internal::tokens2Str[(int) token] << "\",\n";
			printIndent(os);
			os << "\"contents\": [\n";
			incIndent();
			bool isFirst = true;
			for (auto& e : contents){
				if (isFirst){ isFirst = false; }
				else { printIndent(os); os << ", // " << Internal::tokens2Str[(int) token] << "\n"; }
				printIndent(os);
				if (e.which() == NodeContentTypes::NextNode) { (*boost::get<std::shared_ptr<Node>>(e)).print(os); }
				else if (e.which() == NodeContentTypes::Identifier) { os << "\"" << e << "\"";}
				else { os << e; }
			}
			os << "\n";
			decIndent();
			printIndent(os);
			os << "]\n";
			decIndent();
			printIndent(os);
			os << "} // " << Internal::tokens2Str[(int) token] << "\n";
			decIndent();
			printIndent(os);
			os << "}\n";
		}

		std::shared_ptr<Node> getPtr() { return std::shared_ptr<Node>(this); }

	};

}

std::ostream& operator<<(std::ostream& os, ALSL::Printable& self) {
	self.print(os);
	return os;
}

