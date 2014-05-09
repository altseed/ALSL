#include "../AST/ast.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <iterator>
#include <boost/optional.hpp>

#include "parser.hpp"
#include "parserCore.hpp"


namespace ALSL {
	boost::optional<std::shared_ptr<Node>> parse(std::string const& fileName, std::string::const_iterator beg, std::string::const_iterator end) {
		Skipper< boost::spirit::line_pos_iterator<std::string::const_iterator>> skipper;
		std::shared_ptr<ALSL::Node> result;

		auto itr = boost::spirit::line_pos_iterator<std::string::const_iterator>(beg);

		auto const itrEnd = boost::spirit::line_pos_iterator<std::string::const_iterator>(end);

		Grammar<boost::spirit::line_pos_iterator<std::string::const_iterator>> grammar(fileName, itr, itrEnd);
		if(boost::spirit::qi::phrase_parse(itr, itrEnd, grammar, skipper, result)) {
			return boost::make_optional(result);
		} else {
			return boost::optional<std::shared_ptr<Node>>();
		}

	}
	boost::optional<std::shared_ptr<Node>> parse(std::string const& fileName, std::string const& src) {
		return parse(fileName, src.cbegin(), src.cend());
	}

	/*
	// マルチバイト文字対応
	boost::optional<std::shared_ptr<Node>> parse(std::istream_iterator<char> beg, std::istream_iterator<char> end) {
		Grammar<boost::spirit::line_pos_iterator<std::istream_iterator<char>>> grammar;
		Skipper< boost::spirit::line_pos_iterator<std::istream_iterator<char>>> skipper;
		std::shared_ptr<ALSL::Node> result;

		auto itr = boost::spirit::line_pos_iterator<std::istream_iterator<char>>(beg);

		auto const itrEnd = boost::spirit::line_pos_iterator<std::istream_iterator<char>>(end);
		if(boost::spirit::qi::phrase_parse(itr, itrEnd, grammar, skipper, result)) {
			return boost::make_optional(result);
		} else {
			return boost::optional<std::shared_ptr<Node>>();
		}

	}
	*/
	boost::optional<std::shared_ptr<Node>> parseFile(std::string const& fileName) {
		std::ifstream ifs(fileName, std::ifstream::in);
		std::string str;
		while(!ifs.eof()) {
			std::string s;
			std::getline(ifs, s );
			str += s + "\n";
		}
		
		return parse(fileName, str.begin(), str.end());
	}


}