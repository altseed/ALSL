#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/variant/variant.hpp>
#include "../AST/ast.hpp"
namespace ALSL{
namespace qi = boost::spirit::qi;
namespace ascii = qi::ascii;


void emplaceStk(std::list<std::shared_ptr<ALSL::Node>>* stk, bool const isConst, Tokens const token) {
	stk->push_back(std::make_shared<ALSL::Node>(isConst, token));

}

void emplaceStk(std::list<std::shared_ptr<ALSL::Node>>* stk, bool const isConst, Tokens const token, NodeContent const content) {
	stk->push_back(std::make_shared<ALSL::Node>(isConst, token));
	stk->back()->contents.push_back(content);

}

void reduceBinOp(std::list<std::shared_ptr<ALSL::Node>>* stk, Tokens const token){
	if (stk->size() < 2){ return; } // TODO: ‚Ü‚Æ‚à‚È—áŠOˆ—‚ðl‚¦‚é
	auto opd2 = stk->back();
	stk->pop_back();
	auto opd1 = stk->back();
	stk->pop_back();


}

template <typename itrT>
struct Grammar : qi::grammar<itrT, ascii::space_type> {
	qi::rule<itrT, ascii::space_type> intLtr, ltr;
	qi::rule<itrT, ascii::space_type, qi::locals<Tokens>> opMulDiv, opAddSub
	Grammar() : Grammar::base_type(ltr) {
		using qi::omit; using qi::lit; using qi::raw; using qi::labels::_a;
		using boost::bind; using boost::ref;

		intLtr.name("Integer Literal");
		intLtr = qi::int_[
			bind(&emplaceStk, &stk, true, Tokens::intLit, _1)
		];

		ltr.name("Literals")
		ltr = intLtr;

		opMulDiv.name("Multiplication, division and modular operator");
		opMulDiv =
			ltr > (
				lit('*')[_a = Tokens::opMul] |
				lit('/')[_a = Tokens::opDiv] |
				lit('%')[_a = Tokens::opMod]
			) >
			ltr[]
		//ltr % "**" % (lit('*') | '/' | '%') | (lit('+') | '-');

	}

	std::list<std::shared_ptr<ALSL::Node>> stk;
	
	

};

}

