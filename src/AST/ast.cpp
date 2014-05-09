#include "ast.hpp"
namespace ALSL {
void Node::print(std::ostream& os) {
	os << std::boolalpha;
	os << "{\n";
	incIndent();
	printIndent(os);
	os << "\"Node\": {\n";
	incIndent();
	printIndent(os);
	os << "\"isConst\": " << isConst << ",\n";
	printIndent(os);
	os << "\"token\": \"" << Internal::tokens2Str[(int)token] << "\",\n";
	printIndent(os);
	os << "\"contents\": [\n";
	incIndent();
	bool isFirst = true;
	for(auto& e : contents) {
		if(isFirst) { isFirst = false; } else { printIndent(os); os << ", // " << Internal::tokens2Str[(int)token] << "\n"; }
		printIndent(os);
		if(e.which() == NodeContentTypes::NextNode) {
			(*boost::get<std::shared_ptr<Node>>(e)).print(os); 
		} else if(e.which() == NodeContentTypes::Identifier) {
			os << "\"" << boost::get<std::string>(e) << "\""; 
		} else {
			os << e;
		}
	}
	os << "\n";
	decIndent();
	printIndent(os);
	os << "]\n";
	decIndent();
	printIndent(os);
	os << "} // " << Internal::tokens2Str[(int)token] << "\n";
	decIndent();
	printIndent(os);
	os << "}\n";
}

}

std::ostream& operator<<(std::ostream& os, ALSL::Printable& self) {
	self.print(os);
	return os;
}

