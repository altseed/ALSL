#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/variant/variant.hpp>
#include "../AST/ast.hpp"
namespace ALSL{
namespace qi = boost::spirit::qi;
namespace ascii = qi::ascii;

void pushToken(std::list<Tokens>* opStk, Tokens const token){
	opStk->push_back(token);
}

void emplaceStk(std::list<std::shared_ptr<ALSL::Node>>* stk, bool const isConst, Tokens const token) {
	stk->push_back(std::make_shared<ALSL::Node>(isConst, token));

}

void emplaceStk(std::list<std::shared_ptr<ALSL::Node>>* stk, bool const isConst, Tokens const token, NodeContent const content) {
	stk->push_back(std::make_shared<ALSL::Node>(isConst, token));
	stk->back()->contents.push_back(content);

}

void reduceBinOp(std::list<std::shared_ptr<ALSL::Node>>* stk, std::list<Tokens>* ops, bool& pass){
	pass = true;
	if (ops->size() < 1){ pass = false; return; }
	Tokens token = ops->back();
	ops->pop_back();
	if (stk->size() < 2){ pass = false; return; }
	auto opd2 = stk->back();
	stk->pop_back();
	auto opd1 = stk->back();
	stk->pop_back();
	emplaceStk(stk, (opd1->isConst && opd2->isConst), token);
	stk->back()->contents.push_back(opd1);
	stk->back()->contents.push_back(opd2);

}

template <typename itrT>
struct Grammar : qi::grammar<itrT, ascii::space_type> {
	qi::rule<itrT, ascii::space_type> intLtr, ltr, opMulDiv, opAddSub, expr, entry;
	Grammar() : Grammar::base_type(entry) {
		using qi::omit; using qi::lit; using qi::raw;
		using boost::phoenix::construct; using boost::phoenix::val;
		using boost::bind;
		// msvcでラムダ式のパースに失敗するのでbindで頑張る
		intLtr.name("Integer Literal");
		intLtr = qi::int_[
			bind(&emplaceStk, &nodeStk, true, Tokens::intLit, _1)
		];

		ltr.name("Literals or expressions in parenthesis");
		ltr =
			intLtr |
			lit('(') > expr.alias() > lit(')');

		opMulDiv.name("Multiplication, division or modular operator");
		// this以下ではphoenixのstl対応が使いにくい、bindと組み合わせるとplaceholderの_a等と連携できない
		opMulDiv =
			ltr > *(((
			lit('*')[bind(&pushToken, &opStk, Tokens::opMult)] |
			lit('/')[bind(&pushToken, &opStk, Tokens::opDiv)] |
			lit('%')[bind(&pushToken, &opStk, Tokens::opMod)]
			) >
			ltr)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opAddSub.name("Addition or Subtraction operator");
		opAddSub =
			opMulDiv > *(((
			lit('+')[bind(&pushToken, &opStk, Tokens::opPlus)] |
			lit('-')[bind(&pushToken, &opStk, Tokens::opMinus)]
			) >
			opMulDiv)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);
		expr.name("Expression");
		expr = opAddSub;
		entry = expr;
		qi::on_error<qi::fail>
			(
			entry
			, std::cout
			<< val("An error has been occured: Expecting \"")
			<< qi::labels::_4                               // what failed?
			<< val("\"  but accutually got \"")
			<< construct<std::string>(qi::labels::_1, qi::labels::_2)   // iterators to error-pos, end
			<< val("\"")
			<< std::endl

			);

	}

	std::list<std::shared_ptr<Node>> nodeStk;
	std::list<Tokens> opStk;
	
	

};

}

