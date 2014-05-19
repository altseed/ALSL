#pragma once
#ifndef ALSL_PARSERCORE_HPP
#define ALSL_PARSERCORE_HPP

#define _VARIADIC_MAX  10
#define BOOST_SPIRIT_USE_PHOENIX_V3 1

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
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


template<typename RetT, typename> struct ConceptChecker {
	using result_type = RetT;
};

struct lzAddNodeContentImpl {
	typedef SpNode result_type;
	template<typename T>
	auto operator()(SpNode node, bool const isConstProp, T const t) const
	->typename ConceptChecker<result_type, decltype(t->isConst)>::result_type {
		node->isConst = isConstProp ? (node->isConst && t->isConst) : node->isConst;
		node->contents.push_back(t);
		return node;
	}
	template<typename T, typename... Args>
	auto operator()(SpNode node, bool const isConstProp, T const t, Args const... args) const
	->typename ConceptChecker<result_type, decltype(t->isConst)>::result_type
	{
		node->isConst = isConstProp? (node->isConst && t->isConst): node->isConst;
		node->contents.push_back(t);
		return (*this)(node, isConstProp, args...);
	}

};
boost::phoenix::function<lzAddNodeContentImpl> lzAddNodeContent;


struct lzMakeNodeImpl {
	typedef SpNode result_type;
	
	// ノードの定数伝搬計算用
	template<typename T> static auto getConstPropVal(bool const b, T const t)
	-> typename ConceptChecker<bool, decltype(t->isConst)>::result_type
	{
		return b && t->isConst;
	}

	template<typename T, typename... Args>
	static auto getConstPropVal(bool const b, T const t, Args const... args)
	-> typename ConceptChecker<bool, decltype(t->isConst)>::result_type
	{
		return  getConstPropVal(b && t->isConst, args...);
	}


	template<typename... Args> result_type operator()(Tokens const tokens, Args const... args) const {
		return Node::makeNode(getConstPropVal(true, args...), tokens, args...);
	}

	template<typename... Args> result_type operator()(bool const isConst, Tokens const tokens, Args const... args) const{
		return Node::makeNode(isConst, tokens, args...);
	}
};
boost::phoenix::function<lzMakeNodeImpl> lzMakeNode;


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
struct Grammar: qi::grammar<itrT, SpNode(), skpT> {


	typedef boost::fusion::vector<itrT&, itrT const&, itrT const&, qi::info const&> ErrorInfo;


	template<class itrT> std::string getContainedLine(boost::spirit::line_pos_iterator<itrT> begin, boost::spirit::line_pos_iterator<itrT> pos, boost::spirit::line_pos_iterator<itrT> end) {
		itrT first = pos.base();
		itrT last = pos.base();
		while(begin.base() <= first && *first != '\r' && *first != '\n') { --first; }
		while(last < end.base() && *last != '\r' && *last != '\n') { ++last; }
		auto ret = std::string(first, last) + '\n';
		for(auto it = first; it < last; ++it) {
			if(it == pos.base()) {
				ret += '^';
			} else if(*it == '\t') {
				ret += '\t';
			} else {
				ret += ' '; // TODO: consider multibyte char
			}
		}
		return ret;
	}


	void printErrorMsg(itrT const& first, itrT const& last, itrT const& pos, std::string const& what, std::string const& additional) {

		std::cout << "Error in " << boost::spirit::get_line(pos) << ", here: " << boost::spirit::get_current_line(srcFirst, pos, srcEnd) << "\n\t(" << additional << ")" << std::endl;
	}

	void printErrorMsg(qi::expectation_failure<itrT> const& inf, std::string const& additional) {

		std::cout << srcName << "(" << boost::spirit::get_line(inf.first) << "): " << additional;
	}

	void printErrorMsg(ErrorInfo params, std::string const& additional) {
		auto const first = boost::fusion::at_c<0>(params);
		auto const last = boost::fusion::at_c<1>(params);
		auto const pos = boost::fusion::at_c<2>(params);
		auto const what = boost::fusion::at_c<3>(params);
		std::cout << "Error in " << srcName << "(" << boost::spirit::get_line(pos) << "), here: " << getContainedLine(srcFirst, pos, srcEnd) << "\n\t(" << additional << ")" << std::endl;
	}

	template<typename localT = qi::unused_type, typename attrT = SpNode()> using rule = qi::rule<itrT, attrT, skpT, localT>;
	rule<> intLtr, floatLit, doubleLit, paren, ltr, lhs, expr, functionCall, statement, identif, entry, stxIf, stxWhile, stxFor, stxDoWhile, stxStruct, declVar, declConst, declProp, type, kwd, preprocessor;
	rule<qi::locals<Tokens>> opUnary, opMulDiv, opAddSub, opBitShift, opLtGt, opEqNeq, opBitAnd, opBitXor, opBitOr, opLogicAnd, opLogicOr, opSelectSub, opAssign, opSeq;
	rule<qi::locals<SpNode, SpNode>> block, stxFunc;



	itrT srcFirst, srcEnd;
	std::string srcName;



	Grammar(std::string const& fileName, itrT first, itrT end): Grammar::base_type(entry), srcName(fileName), srcFirst(first), srcEnd(end) {
		using qi::lit; using qi::labels::_val; using qi::eps; using namespace qi::labels;
		using boost::phoenix::val;

		intLtr.name("Integer Literal");
		intLtr = qi::lexeme[qi::int_][_val = lzMakeNode(val(true), val(Tokens::intLit), _1)];

		floatLit.name("Float Literal");
		floatLit = qi::lexeme[qi::real_parser<float, qi::strict_real_policies<float>>() >> lit('f')]
			[_val = lzMakeNode(val(true), val(Tokens::floatLit), _1)];

		doubleLit.name("Double Literal");
		doubleLit = qi::lexeme[qi::real_parser<double, qi::strict_real_policies<double>>()]
			[_val = lzMakeNode(val(true), val(Tokens::doubleLit), _1)];

		identif.name("Identifier");
		identif = qi::as_string[qi::lexeme[qi::char_("_a-zA-Z") >> *qi::char_("_a-zA-Z0-9")]][_val = lzMakeNode(val(true), val(Tokens::identif), _1)];
		// qi::char_('_')|qi::alnum等とやると、_のある場所でバッファオーバーランする
		


		preprocessor.name("preprocessor");
		preprocessor = qi::as_string[qi::lexeme[qi::char_("#") >> *(qi::char_ - qi::lit('\r') - qi::lit('\n'))]][_val = lzMakeNode(true, val(Tokens::preprocessor), lzMakeNode(true, val(Tokens::identif), _1))];
		// lexemeの代わりにno_skipを使うとパースできない(char_か何かと食い合ってる可能性)
		qi::on_error<qi::fail>(
			preprocessor,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);

		type.name("type name");
		type =
			identif[_val = lzMakeNode(val(true), val(Tokens::type), _1)];// >>
			//*('[' >
			//qi::int_[lzAddNodeContent(_val, val(true), lzMakeNode(val(true), val(Tokens::intLit), _1))] >
			//']');

		paren.name("parenthes");
		paren = (lit('(') >> expr >> ')')[_val = lzMakeNode(val(true), val(Tokens::paren), _1)];

		ltr.name("literal");
		ltr %= floatLit | doubleLit | intLtr;

		functionCall.name("function call");
		functionCall = (identif >> lit('('))[_val = lzMakeNode(val(Tokens::call), _1)] > -(opAssign[lzAddNodeContent(_val, val(true), _1)] % lit(',')) > lit(')');

		lhs.name("variable(with member selector or array subscript)");
		lhs = identif[_val = _1] >>
			*(
				(lit('.') > (
				qi::lexeme[qi::as_string[qi::repeat(1, 4)[qi::char_('w', 'z') | qi::char_('r') | qi::char_('g') | qi::char_('b') | qi::char_('a')]]]
						[_val = lzMakeNode(val(Tokens::swizzleOp), _val, lzMakeNode(val(true), val(Tokens::identif), _1))] |
					ltr
						[_val = lzMakeNode(val(Tokens::dataMember), _val, lzMakeNode(val(true), val(Tokens::identif), _1))]
				)) |
				(lit('[') >> expr >> lit(']'))[_val = lzMakeNode(val(Tokens::arraySubscript), _val, _1)]
			);
		opUnary.name("unary operator");
		opUnary =
			(ltr |paren | functionCall | lhs)[_val = _1] |
				(
					(
						lit('-')[_a = val(Tokens::opNeg)] |
						lit('!')[_a = val(Tokens::opNot)] |
						lit('~')[_a = val(Tokens::opInv)]
					) >> opUnary.alias()
				)[_val = lzMakeNode(_a, _1)];

		opMulDiv.name("Multiplication, division or modular operator");
		opMulDiv =
			opUnary[_val = _1][_val = _1] >> *(((
			lit('*')[_a = Tokens::opMult] |
			lit('/')[_a = Tokens::opDiv] |
			lit('%')[_a = Tokens::opMod]
			) >>
			opUnary)[_val = lzMakeNode(_a, _val, _1)]);


		opAddSub.name("Addition or Subtraction operator");
		opAddSub =
			opMulDiv[_val = _1] >> *(((
			lit('+')[_a = Tokens::opAdd] |
			lit('-')[_a = Tokens::opSub]
			) >>
			opMulDiv)[_val = lzMakeNode(_a, _val, _1)]);


		opBitShift.name("bit shift operator");
		opBitShift =
			opAddSub[_val = _1] >> *(((
			lit("<<")[_a = Tokens::opLsh] |
			lit(">>")[_a = Tokens::opRsh]
			) >>
			opAddSub)[_val = lzMakeNode(_a, _val, _1)]);


		opLtGt.name("less than (equal) or greater than (equal)");
		opLtGt =
			opBitShift[_val = _1] >> *(((
			lit("<=")[_a = Tokens::opLte] |
			lit('<')[_a = Tokens::opLt] |
			lit(">=")[_a = Tokens::opGte] |
			lit('>')[_a = Tokens::opGt]
			) >>
			opBitShift)[_val = lzMakeNode(_a, _val, _1)]);


		opEqNeq.name("(not) equal");
		opEqNeq =
			opLtGt[_val = _1] >> *(((
			lit("==")[_a = Tokens::opEq] |
			lit("!=")[_a = Tokens::opNeq]
			) >>
			opLtGt)[_val = lzMakeNode(_a, _val, _1)]);



		opBitAnd.name("bitwise and");
		opBitAnd =
			opEqNeq[_val = _1] >> *(((
			lit('&')[_a = Tokens::opBitAnd]
			) >>
			opEqNeq)[_val = lzMakeNode(_a, _val, _1)]);


		opBitXor.name("bitwise xor");
		opBitXor =
			opBitAnd[_val = _1] >> *(((
			lit('^')[_a = Tokens::opBitXor]
			) >>
			opBitAnd)[_val = lzMakeNode(_a, _val, _1)]);


		opBitOr.name("bitwise or");
		opBitOr =
			opBitXor[_val = _1] >> *(((
			lit('|')[_a = Tokens::opBitOr]
			) >>
			opBitXor)[_val = lzMakeNode(_a, _val, _1)]);



		opLogicAnd.name("logical and");
		opLogicAnd =
			opBitOr[_val = _1] >> *(((
			lit("&&")[_a = Tokens::opLogicAnd]
			) >>
			opBitOr)[_val = lzMakeNode(_a, _val, _1)]);


		opLogicOr.name("logical or");
		opLogicOr =
			opLogicAnd[_val = _1] >> *(((
			lit("||")[_a = Tokens::opLogicOr]
			) >>
			opLogicAnd)[_val = lzMakeNode(_a, _val, _1)]);


		opAssign.name("assign operators");
		opAssign = (
						lhs[_val = _1] >> (
							(lit('=') >> !lit('='))[_a = Tokens::opAssign] |
							lit("*=")[_a = Tokens::opMultAssign] |
							lit("/=")[_a = Tokens::opDivAssign] |
							lit("%=")[_a = Tokens::opModAssign] |
							lit("+=")[_a = Tokens::opAddAssign] |
							lit("-=")[_a = Tokens::opSubAssign] |
							lit("<<=")[_a = Tokens::opLshAssign] |
							lit(">>=")[_a = Tokens::opRshAssign] |
							lit("&=")[_a = Tokens::opBitAndAssign] |
							lit("^=")[_a = Tokens::opBitXorAssign] |
							lit("|=")[_a = Tokens::opBitOrAssign]
						) >
						opAssign[_val = lzMakeNode(_a, _val, _1)]
					) |
					opLogicOr[_val = _1];

		opSeq.name("sequence operator");
		opSeq = opAssign[_val = _1] >> *(lit(',')[_a = val(Tokens::opSeq)] > opAssign[_val = lzMakeNode(_a, _val, _1)]);
		qi::on_error<qi::fail>(
			opSeq,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
				printErrorMsg(params, "missing values after an operator");
			}
		);



		expr.name("expression");
		expr %= declConst | declVar | opSeq;
		qi::on_error<qi::fail>(
			expr,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
				printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
			}
		);

		declVar.name("variant declaration");
		declVar = (type >> identif)[_val = lzMakeNode(val(false), val(Tokens::declVar), _1, _2)] >> -('=' > expr[lzAddNodeContent(_val, val(true), _1)]);
		qi::on_error<qi::fail>(
			declVar,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);

		declConst.name("constant declaration");
		declConst = (qi::lexeme["const"] >> type >> identif)[_val = lzMakeNode(val(Tokens::declConst), _1, _2)] > '=' > expr[lzAddNodeContent(_val, val(true), _1)];
		qi::on_error<qi::fail>(
			declConst,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);



		declProp.name("property declaration");
		declProp =
			(qi::lexeme["property"] >> type >> identif)[_val = lzMakeNode(val(false), val(Tokens::declProp), _1, _2)] >
			'=' >
			'{' >
			'"' > qi::as_string[qi::no_skip[*((lit("\\") >> qi::char_) | +(qi::char_ - '"' - '\\'))]]
				[lzAddNodeContent(_val, val(true), lzMakeNode(val(true), val(Tokens::strLit), _1))] > '"' > ',' >
			(floatLit | doubleLit | intLtr)[lzAddNodeContent(_val, val(true), _1)] > ',' >
			(floatLit | doubleLit | intLtr)[lzAddNodeContent(_val, val(true), _1)] > ',' >
			(floatLit | doubleLit | intLtr)[lzAddNodeContent(_val, val(true), _1)] >
			'}';
		qi::on_error<qi::fail>(
			declVar,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);



		kwd.name("keyword");
		kwd =
			(qi::lexeme["return"][_val = lzMakeNode(val(true), val(Tokens::kwdReturn))] >>
				-(expr[lzAddNodeContent(_val, val(true), _1)]) >> ';') |
				(qi::lexeme["break"][_val = lzMakeNode(val(true), val(Tokens::kwdBreak))] >> ';') |
				(qi::lexeme["continue"][_val = lzMakeNode(val(true), val(Tokens::kwdContinue))] >> ';');
		qi::on_error<qi::fail>(
			kwd,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "parsing failed at keyword. missing semi-colon?");
		}
		);



		statement.name("statement");
		statement %= preprocessor | kwd | stxIf | stxWhile | stxFor | stxDoWhile | (expr > lit(';'));
		qi::on_error<qi::fail>(
			statement,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(
				params,
				"missing semi-colon?"
				);
		}
		);



		block.name("block");
		block = lit('{') > (
			lit('}')[_val = lzMakeNode(val(true), val(Tokens::none))] |
			(
			statement[_val = lzMakeNode(val(Tokens::seq), _1)] >> *(statement[lzAddNodeContent(_val, val(true), _1)])
			) > '}'
		);
		qi::on_error<qi::fail>(
			block,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);


		
		stxIf.name("if");
		stxIf =
			lit("if") >
			'(' >
			expr[_val = lzMakeNode(val(Tokens::stxIf), _1)] >
			')' >
			(block | statement)[lzAddNodeContent(_val, val(true), _1)] >>
			-("else" >> (block | statement)[lzAddNodeContent(_val, val(true), _1)]);
		qi::on_error<qi::fail>(
			stxIf,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);


		stxWhile.name("while");
		stxWhile =
			lit("while") >
			'(' >
			expr[_val = lzMakeNode(val(Tokens::stxWhile), _1)] >
			')' >
			(block | statement)[lzAddNodeContent(_val, val(true), _1)];
		qi::on_error<qi::fail>(
			stxWhile,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);



		stxDoWhile.name("do-while");
		stxDoWhile =
			lit("do")[_val = lzMakeNode(val(true), val(Tokens::stxDoWhile))] >
			(block)[lzAddNodeContent(_val, val(true), _1)] >
			"while" >
			'(' >
			expr[lzAddNodeContent(_val, val(true), _1)] >
			')' >
			';';
		qi::on_error<qi::fail>(
			stxDoWhile,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);




		stxFor.name("for");
		stxFor =
			lit("for")[_val = lzMakeNode(val(true), val(Tokens::stxFor))] >>
			'(' >>
			((expr >> ';')[lzAddNodeContent(_val, val(true), _1)] | lit(';')[lzAddNodeContent(_val, val(true), lzMakeNode(val(true), val(Tokens::none)))]) >>
			((expr >> ';')[lzAddNodeContent(_val, val(true), _1)] | lit(';')[lzAddNodeContent(_val, val(true), lzMakeNode(val(true), val(Tokens::none)))]) >>
			((expr >> ')')[lzAddNodeContent(_val, val(true), _1)] | lit(')')[lzAddNodeContent(_val, val(true), lzMakeNode(val(true), val(Tokens::none)))]) >>
			(block | statement)[lzAddNodeContent(_val, val(true), _1)];

		qi::on_error<qi::fail>(
			stxFor,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);

		stxFunc.name("function declaration");
		stxFunc =
			(type >> identif >> '(')[_val = lzMakeNode(val(Tokens::stxFunc), _1, _2)] >
			*(declVar[lzAddNodeContent(_val, val(true), _1)] % ',') >
			')' >
			block[lzAddNodeContent(_val, val(true), _1)];
		qi::on_error<qi::fail>(
			stxFunc,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);






		stxStruct.name("struct declaration");
		stxStruct = "struct" > identif[_val = lzMakeNode(val(Tokens::stxStruct), _1)] > '{' >
			*(preprocessor[lzAddNodeContent(_val, val(true), _1)] | (declVar[lzAddNodeContent(_val, val(true), _1)] > ';')) >
			'}' >> -lit(';');
		qi::on_error<qi::fail>(
			stxStruct,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);


		entry =
			(preprocessor | ((declConst | declProp) > ';') | stxStruct | stxFunc)[_val = lzMakeNode(val(Tokens::seq), _1)] >>
			*((preprocessor | ((declConst | declProp) > ';') | stxStruct | stxFunc)[lzAddNodeContent(_val, val(true), _1)])
			// block[_val = _1] |
			// (statement[_val = lzMakeNode(val(Tokens::seq), _1)] >> *(statement[lzAddNodeContent(_val, val(true), _1)]))
			;
		qi::on_error<qi::fail>(
			entry,
			[this](ErrorInfo params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(params, "Unexpected token, maybe missing operators or operands?");
		}
		);
	}

};

}


#endif
