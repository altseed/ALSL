#include "../AST/ast.hpp"
#include <memory>
#include <string>
#include <boost/optional.hpp>

#include "parser.hpp"
#include "parserCore.hpp"


namespace ALSL {
	boost::optional<std::shared_ptr<Node>> parse(std::string::const_iterator beg, std::string::const_iterator end) {
		Grammar<boost::spirit::line_pos_iterator<std::string::const_iterator>> grammar;
		Skipper< boost::spirit::line_pos_iterator<std::string::const_iterator>> skipper;
		std::shared_ptr<ALSL::Node> result;

		auto itr = boost::spirit::line_pos_iterator<std::string::const_iterator>(beg);

		auto const itrEnd = boost::spirit::line_pos_iterator<std::string::const_iterator>(end);
		if(boost::spirit::qi::phrase_parse(itr, itrEnd, grammar, skipper, result)) {
			return boost::make_optional(result);
		} else {
			return boost::optional<std::shared_ptr<Node>>();
		}

	}
	boost::optional<std::shared_ptr<Node>> parse(std::string const& src) {
		return parse(src.cbegin(), src.cend());
	}

}