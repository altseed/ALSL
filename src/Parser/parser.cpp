#include "parser.hpp"


template <typename itrT>
struct Grammar :qi::grammar<itrT> {
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
	std::stack<
};

