#pragma once

#include <iostream>
#include <memory>
#include <list>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

namespace ALSL{
	// todo: helperŒn‚ª—­‚Ü‚Á‚½‚çW‚ß‚Ä‘¼‚Ìƒwƒbƒ_‚ÉˆÚ‚·
	class Printable{
	public:
		virtual void print(std::ostream&) = 0;
	};
	struct Node;
	typedef boost::variant<std::shared_ptr<Node>, int, float, double> NodeContent;
	namespace NodeContentTypes{
		int const NextNode = 0;
		int const Int = NextNode + 1;
		int const Float = Int + 1;
		int const Double = Float + 1;
	}
	enum class Tokens{
		none,
		intLit,
		floatLit,
		opPlus,
		opMinus,
		opMult,
		opDiv,
		opMod
	};
	namespace Internal{
	static const std::string tokens2Str[] = { "none", "intLit", "floatLit", "opPlus", "opMinus", "opMult", "opDiv", "opMod" };

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

