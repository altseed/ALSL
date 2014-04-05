#include <string>
#include <iostream>
#include <memory>

#include "../AST/ast.hpp"
#include "../parser/parser.hpp"

int main() {
	ALSL::Grammar<boost::spirit::line_pos_iterator<std::string::iterator>> grammar;
	ALSL::Skipper< boost::spirit::line_pos_iterator<std::string::iterator>> skipper;
	std::shared_ptr<ALSL::Node> result;
	std::string src;
	std::cout << "Input a test code.(Ctrl-Z to quit)" << std::endl;
	for(std::string s; std::getline(std::cin, s);) {
		src.append("\n").append(s);
	}

	auto itr = boost::spirit::line_pos_iterator<std::string::iterator>(src.begin());

	auto const end = boost::spirit::line_pos_iterator<std::string::iterator>(src.end());
	bool res = boost::spirit::qi::phrase_parse(itr, end, grammar, skipper, result);
	std::cout << "Is parsing succeeded?: " << std::boolalpha << res << std::endl;
	if(res) {
		std::cout << "result:" << std::endl << *result << std::endl;
	}
	std::cin.clear();
	char c;
	std::cin >> c;
	return 0;
}