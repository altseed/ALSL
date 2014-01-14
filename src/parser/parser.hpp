#pragma once

#include <string>
#include <vector>
#include <stack>
#include <list>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/variant.hpp>
namespace ALSL{
namespace qi = boost::spirit::qi;
namespace ascii = qi::ascii;


template <typename itrT>
struct Grammar : qi::grammar<itrT, ascii::space_type> {
Grammar:Grammer::base_type(op2) {
	using qi::omit; using qi::lit; using qi::raw;

	dec_ltr = omit[
		qi::int_[
			[&](int i, unused_type, unused_type){
				this->acmStk.push(i);
				return;
			}
		]
	];
	ltr = dec_ltr;

	ltr % "**" % (lit('*') | '/' | '%') | (lit('+') | '-');

		};

		qi::rule<itrT> dec_ltr, ltr, op1, op2;
		std::stack <
};

}

