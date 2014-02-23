#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/container/vector.hpp>
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


// TODO: reduceはもはやreduce時にしていないことが多いので名前を変える
// TODO: BinOp, UnOp系は演算子に限らないので名前を変える

// トークン記号を一つ取り、スタックの後ろから2つとり、それらををオペランドとした式ノードをスタックに積む
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

// トークン記号を一つ取り、スタックの後ろから1つとり、それらををオペランドとした式ノードをスタックに積む

void reduceUnOp(std::list<std::shared_ptr<ALSL::Node>>* stk, std::list<Tokens>* ops, bool& pass) {
	pass = true;
	if (ops->size() < 1){ pass = false; return; }
	Tokens token = ops->back();
	ops->pop_back();
	if (stk->size() < 1){ pass = false; return; }
	auto opd1 = stk->back();
	stk->pop_back();
	emplaceStk(stk, (opd1->isConst), token);
	stk->back()->contents.push_back(opd1);
}


// トークン記号を一つ取り、スタックの後ろから3つとり、それらををオペランドとした式ノードをスタックに積む

void reduceTriOp(std::list<std::shared_ptr<ALSL::Node>>* stk, std::list<Tokens>* ops, bool& pass) {
	pass = true;
	if (ops->size() < 1){ pass = false; return; }
	Tokens token = ops->back();
	ops->pop_back();
	if (stk->size() < 3){ pass = false; return; }
	auto opd3 = stk->back();
	stk->pop_back();
	auto opd2 = stk->back();
	stk->pop_back();
	auto opd1 = stk->back();
	stk->pop_back();
	emplaceStk(stk, (opd1->isConst && opd2->isConst && opd3->isConst), token);
	stk->back()->contents.push_back(opd1);
	stk->back()->contents.push_back(opd2);
	stk->back()->contents.push_back(opd3);
}


// スタックの後ろをとり、その下にあるノードのcontentにする
void reduceTopByNext(std::list<std::shared_ptr< ALSL::Node >> *stk, bool isConstPropagation, bool& pass) {
	pass = true;
	if (stk->size() < 2){ pass = false; return; }
	auto cont = stk->back();
	
	stk->pop_back();
	stk->back()->contents.push_back(cont);
	if (isConstPropagation){ stk->back()->isConst = cont->isConst; }
}

// スタックからひとつ取り、それを識別子とした関数呼び出しノードを積む
void functionCallImpl(std::list<std::shared_ptr<ALSL::Node>>* stk,  bool& pass){
	pass = true;
	if (stk->size() < 1 || stk->back()->token != Tokens::identif){ pass = false; return; }
	auto id = stk->back();
	stk->pop_back();
	emplaceStk(stk, true, Tokens::call, id);
	
}


template <typename itrT>
struct Skipper : qi::grammar<itrT> {
	qi::rule<itrT> entry, comment, multiLineComment, singleLineComment;
	Skipper() : Skipper::base_type(entry, "ALSL comment") {
		multiLineComment = qi::lit("/*") >> *(multiLineComment.alias() | (qi::char_ - qi::lit('*'))) >> qi::lit("*/");
		singleLineComment = (qi::lit("//") >> *(qi::char_ - qi::lit('\r') - qi::lit('\n')));
		comment = singleLineComment | multiLineComment;
		entry = qi::ascii::space | comment;

	}
};

template <typename itrT, typename skpT = Skipper<itrT>>
struct Grammar : qi::grammar<itrT, skpT> {

	void printErrorMsg(itrT const& first, itrT const& last, itrT const& pos, std::string const& what, std::string const& additional) {

		std::cout << "Error in Line " << boost::spirit::get_line(pos) << ": " << std::string(pos, last) << "("<< additional << ")" << std::endl;
	}

	qi::rule<itrT, skpT> intLtr, floatLit, doubleLit, ltr, opUnary1, opUnary2, opMulDiv, opAddSub, opBitShift, opLtGt, opEqNeq, opBitAnd, opBitXor, opBitOr, opLogicAnd, opLogicOr, opSelect, opSelectSub, opAssign, opSeq, expr, functionCall, sentence, block, identif, entry;

	Grammar() : Grammar::base_type(entry) {
		using qi::omit; using qi::lit; using qi::raw;
		using boost::phoenix::construct; using boost::phoenix::val;
		using boost::bind;
// msvcのバグ回避
#ifdef _MSC_VER
#define $(code) .operator[](code)
#else
#define $(code) [code]
// ラムダ式単独で書く場合、左ブラケットの連続が規約違反になるので括弧で囲む必要がある
#endif

		intLtr.name("Integer Literal");
		intLtr = qi::int_ $(
			// bind(&emplaceStk, &nodeStk, true, Tokens::intLit, _1)
			([this](int i, qi::unused_type, bool& pass){emplaceStk(&(this->nodeStk), true, Tokens::intLit); })
		);

		floatLit.name("Float Literal");
		floatLit = (qi::real_parser<float, qi::strict_real_policies<float>>() >> lit('f'))[bind(&emplaceStk, &nodeStk, true, Tokens::floatLit, _1)];

		doubleLit.name("Double Literal");
		doubleLit = (qi::real_parser<double, qi::strict_real_policies<double>>())[bind(&emplaceStk, &nodeStk, true, Tokens::doubleLit, _1)];

		identif.name("Identifier");
		identif = qi::as_string[(qi::alpha | '_') >> *(qi::alnum | '_')][bind(&emplaceStk, &nodeStk, true, Tokens::identif, _1)];


		ltr.name("Literals or expressions in parenthesis");
		ltr =
			floatLit |
			doubleLit |
			intLtr |
			lit('(') > expr.alias() > lit(')') |
			(identif >> -(lit('(')[bind(&functionCallImpl, &nodeStk, _3)] >> -(expr[bind(&reduceTopByNext, &nodeStk, true, _3)] % lit(',')) >> lit(')')))
			;

		opUnary1.name("member selectors or array subscript");
		opUnary1 = ltr > *(
			(lit('.') > (
				qi::as_string[qi::repeat(1, 4)[qi::char_('w', 'z')]]$(
					([this](std::string s, qi::unused_type, bool& pass){
						pushToken(&(this->opStk), Tokens::swizzleOp);
						emplaceStk(&(this->nodeStk), true, Tokens::identif, s);
						reduceBinOp(&(this->nodeStk), &(this->opStk), pass);
					})
				) |
				ltr $(
					([this](qi::unused_type, qi::unused_type, bool& pass){
								pushToken(&(this->opStk), Tokens::dataMember);
								reduceBinOp(&(this->nodeStk), &(this->opStk), pass);
						})
					)
			)) |
			(lit('[')[bind(&pushToken, &opStk, Tokens::arraySubscript)] > expr.alias() > lit(']'))[bind(&reduceBinOp, &nodeStk, &opStk, _3)]
		);

		opUnary2.name("unary operator");
		opUnary2 =
				(
					(
						lit('-')[bind(&pushToken, &opStk, Tokens::opNeg)] |
						lit('!')[bind(&pushToken, &opStk, Tokens::opNot)] |
						lit('~')[bind(&pushToken, &opStk, Tokens::opInv)]
					) >> opUnary2.alias()
				)[bind(&reduceUnOp, &nodeStk, &opStk, _3)] |
				opUnary1;

		opMulDiv.name("Multiplication, division or modular operator");
		// this以下ではphoenixのstl対応が使いにくい、bindと組み合わせるとplaceholderの_a等と連携できない
		opMulDiv =
			opUnary2 > *(((
			lit('*')[bind(&pushToken, &opStk, Tokens::opMult)] |
			lit('/')[bind(&pushToken, &opStk, Tokens::opDiv)] |
			lit('%')[bind(&pushToken, &opStk, Tokens::opMod)]
			) >
			opUnary2)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opAddSub.name("Addition or Subtraction operator");
		opAddSub =
			opMulDiv > *(((
			lit('+')[bind(&pushToken, &opStk, Tokens::opAdd)] |
			lit('-')[bind(&pushToken, &opStk, Tokens::opSub)]
			) >
			opMulDiv)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opBitShift.name("bit shift operator");
		opBitShift =
			opAddSub > *(((
			lit("<<")[bind(&pushToken, &opStk, Tokens::opLsh)] |
			lit(">>")[bind(&pushToken, &opStk, Tokens::opRsh)]
			) >
			opAddSub)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opLtGt.name("less than (equal) or greater than (equal)");
		opLtGt =
			opBitShift > *(((
			lit('<')[bind(&pushToken, &opStk, Tokens::opLt)] |
			lit("<=")[bind(&pushToken, &opStk, Tokens::opLte)] |
			lit('>')[bind(&pushToken, &opStk, Tokens::opGt)] |
			lit(">=")[bind(&pushToken, &opStk, Tokens::opGte)]
			) >
			opBitShift)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opEqNeq.name("(not) equal");
		opEqNeq =
			opLtGt > *(((
			lit("==")[bind(&pushToken, &opStk, Tokens::opEq)] |
			lit("!=")[bind(&pushToken, &opStk, Tokens::opNeq)]
			) >
			opLtGt)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);



		opBitAnd.name("bitwise and");
		opBitAnd =
			opEqNeq > *(((
			lit('&')[bind(&pushToken, &opStk, Tokens::opBitAnd)]
			) >
			opEqNeq)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opBitXor.name("bitwise xor");
		opBitXor =
			opBitAnd > *(((
			lit('^')[bind(&pushToken, &opStk, Tokens::opBitXor)]
			) >
			opBitAnd)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opBitOr.name("bitwise or");
		opBitOr =
			opBitXor > *(((
			lit('|')[bind(&pushToken, &opStk, Tokens::opBitOr)]
			) >
			opBitXor)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);



		opLogicAnd.name("logical and");
		opLogicAnd =
			opBitOr > *(((
			lit("&&")[bind(&pushToken, &opStk, Tokens::opLogicAnd)]
			) >
			opBitOr)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opLogicOr.name("logical or");
		opLogicOr =
			opLogicAnd > *(((
			lit("||")[bind(&pushToken, &opStk, Tokens::opLogicOr)]
			) >
			opLogicAnd)[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		opAssign.name("unary operator");
		opAssign = opLogicOr >>
				-(((
					lit('=')[bind(&pushToken, &opStk, Tokens::opAssign)] |
					lit("*=")[bind(&pushToken, &opStk, Tokens::opMultAssign)] |
					lit("/=")[bind(&pushToken, &opStk, Tokens::opDivAssign)] |
					lit("%=")[bind(&pushToken, &opStk, Tokens::opModAssign)] |
					lit("+=")[bind(&pushToken, &opStk, Tokens::opAddAssign)] |
					lit("-=")[bind(&pushToken, &opStk, Tokens::opSubAssign)] |
					lit("<<=")[bind(&pushToken, &opStk, Tokens::opLshAssign)] |
					lit(">>=")[bind(&pushToken, &opStk, Tokens::opRshAssign)] |
					lit("&=")[bind(&pushToken, &opStk, Tokens::opBitAndAssign)] |
					lit("^=")[bind(&pushToken, &opStk, Tokens::opBitXorAssign)] |
					lit("|=")[bind(&pushToken, &opStk, Tokens::opBitOrAssign)]
				)
			> (opAssign.alias() | opLogicOr))[bind(&reduceBinOp, &nodeStk, &opStk, _3)]);


		expr.name("Expression");
		expr = opAssign;
		entry = expr;
		qi::on_error<qi::fail>(
			entry,
			[this](boost::fusion::vector<itrT&, itrT const&, itrT const&, qi::info const&> params, qi::unused_type, qi::error_handler_result){
				printErrorMsg(
					boost::fusion::at_c<0>(params),
					boost::fusion::at_c<1>(params),
					boost::fusion::at_c<2>(params),
					boost::fusion::at_c<3>(params).tag,
					"Missing operators or operands?"
				);
			}
			);
#undef $

	}

	std::list<std::shared_ptr<Node>> nodeStk;
	std::list<Tokens> opStk;
	
	

};

}

