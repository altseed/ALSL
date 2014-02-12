#pragma once

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
		virtual void print(std::ostream&) = 0;
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
		identif,

		call, // function call (exp funcName, exp arg, exp arg,...)
		arrayMember, // [] (exp arrayName, exp index)
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
		opAddAsign, // +=
		opSubAssign, // -=
		opMultAssign, // *=
		opDivAssign, // /=
		opModAssign, // %=
		opLshAssign, // <<=
		opRshAssigh, // >>=
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
		static const std::string tokens2Str[] = { "none", "intLit", "floatLit", "identif", "call", "arrayMember", "swizzleOp", "dataMember", "opPos", "opNeg", "opInv", "opNot", "opMult", "opDiv", "opMod", "opAdd", "opSub", "opLsh", "opRsh", "opGt", "opLt", "opGte", "opLte", "opEq", "opNeq", "opBitAnd", "opBitXor", "opBitOr", "opLogicAnd", "opLogicOr", "opSelect", "opAssign", "opAddAsign", "opSubAssign", "opMultAssign", "opDivAssign", "opModAssign", "opLshAssign", "opRshAssigh", "opBitAndAssign", "opBitOrAssign", "opBitXorAssign", "opSeq", "seq", "stxIf", "stxWhile", "stxFor", "stxDoWhile", "stxFunc" };

	}

	struct Node: public Printable {

	public:
		bool isConst = false;
		Tokens token = Tokens::none;
		std::list<NodeContent> contents;

		Node(bool isConst_, Tokens token_) : isConst(isConst_), token(token_){}

		void print(std::ostream& os) override{
			os << std::boolalpha <<
				"{\"Node\": {\"isConst\": " <<
				isConst <<
				", \"token\": \"" <<
				Internal::tokens2Str[(int)token] <<
				"\", \"contents\": [";
			for (auto& e : contents){
				bool isFirst = true;
				if (isFirst){ isFirst = false; }
				else { os << ", "; }
				if (e.which() == NodeContentTypes::NextNode) { (*boost::get<std::shared_ptr<Node>>(e)).print(os); }
				else { os << e; }
			}
			os << "]}}";
		}

		std::shared_ptr<Node> getPtr() { return std::shared_ptr<Node>(this); }

	};

}

std::ostream& operator<<(std::ostream& os, ALSL::Printable& self) {
	self.print(os);
	return os;
}

