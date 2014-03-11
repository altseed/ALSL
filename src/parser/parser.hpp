#pragma once
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
#line 18

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
	
	// ÉmÅ[ÉhÇÃíËêîì`î¿åvéZóp
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

	void printErrorMsg(itrT const& first, itrT const& last, itrT const& pos, std::string const& what, std::string const& additional) {

		std::cout << "Error in Line " << boost::spirit::get_line(pos) << ", here: " << std::string(pos, last) << "("<< additional << ")" << std::endl;
	}

	template<typename localT = qi::unused_type, typename attrT = SpNode()> using rule = qi::rule<itrT, attrT, skpT, localT>;
	rule<> intLtr, floatLit, doubleLit, ltr, lhs, expr, functionCall, sentence, identif, entry, ifStc, whileStc;
	rule<qi::locals<Tokens>> opUnary, opMulDiv, opAddSub, opBitShift, opLtGt, opEqNeq, opBitAnd, opBitXor, opBitOr, opLogicAnd, opLogicOr, opSelectSub, opAssign, opSeq;
	rule<qi::locals<SpNode, SpNode>> block;

	Grammar(): Grammar::base_type(entry) {
		using qi::lit; using qi::labels::_val; using namespace qi::labels;
		using boost::phoenix::val;

		intLtr.name("Integer Literal");
		intLtr = qi::int_[_val = lzMakeNode(val(true), val(Tokens::intLit), _1)];

		floatLit.name("Float Literal");
		floatLit = (qi::real_parser<float, qi::strict_real_policies<float>>() >> lit('f'))
			[_val = lzMakeNode(val(true), val(Tokens::floatLit), _1)];

		doubleLit.name("Double Literal");
		doubleLit = (qi::real_parser<double, qi::strict_real_policies<double>>())
			[_val = lzMakeNode(val(true), val(Tokens::doubleLit), _1)];

		identif.name("Identifier");
		identif = qi::as_string[(qi::alpha | '_') >> *(qi::alnum | '_')][_val = lzMakeNode(val(true), val(Tokens::identif), _1)];


		ltr.name("Literal or expression in parenthesis");
		ltr %= floatLit | doubleLit | intLtr | (lit('(') > expr.alias() > lit(')'));

		functionCall.name("function call");
		functionCall = (identif > lit('('))[_val = lzMakeNode(val(Tokens::call), _1)] > -(expr[lzAddNodeContent(_val, val(true), _1)] % lit(',')) > lit(')');

		lhs.name("member selector or array subscript");
		lhs = identif[_val = _1] >>
			*(
				(lit('.') > (
					qi::as_string[qi::repeat(1, 4)[qi::char_('w', 'z')]]
						[_val = lzMakeNode(val(Tokens::swizzleOp), _val, lzMakeNode(val(true), val(Tokens::identif), _1))] |
					ltr
						[_val = lzMakeNode(val(Tokens::dataMember), _val, lzMakeNode(val(true), val(Tokens::identif), _1))]
				)) |
				(lit('[') > expr.alias() > lit(']'))[_val = lzMakeNode(val(Tokens::arraySubscript), _val, _1)]
			);

	opUnary.name("unary operator");
		opUnary =
				(ltr | lhs | functionCall)[_val = _1] |
				(
					(
						lit('-')[_a = val(Tokens::opNeg)] |
						lit('!')[_a = val(Tokens::opNot)] |
						lit('~')[_a = val(Tokens::opInv)]
					) >> opUnary.alias()
				)[_val = lzMakeNode(_a, _1)];

		opMulDiv.name("Multiplication, division or modular operator");
		opMulDiv =
			opUnary[_val = _1][_val = _1] > *(((
			lit('*')[_a = Tokens::opMult] |
			lit('/')[_a = Tokens::opDiv] |
			lit('%')[_a = Tokens::opMod]
			) >
			opUnary)[_val = lzMakeNode(_a, _val, _1)]);


		opAddSub.name("Addition or Subtraction operator");
		opAddSub =
			opMulDiv[_val = _1] > *(((
			lit('+')[_a = Tokens::opAdd] |
			lit('-')[_a = Tokens::opSub]
			) >
			opMulDiv)[_val = lzMakeNode(_a, _val, _1)]);


		opBitShift.name("bit shift operator");
		opBitShift =
			opAddSub[_val = _1] > *(((
			lit("<<")[_a = Tokens::opLsh] |
			lit(">>")[_a = Tokens::opRsh]
			) >
			opAddSub)[_val = lzMakeNode(_a, _val, _1)]);


		opLtGt.name("less than (equal) or greater than (equal)");
		opLtGt =
			opBitShift[_val = _1] > *(((
			lit('<')[_a = Tokens::opLt] |
			lit("<=")[_a = Tokens::opLte] |
			lit('>')[_a = Tokens::opGt] |
			lit(">=")[_a = Tokens::opGte]
			) >
			opBitShift)[_val = lzMakeNode(_a, _val, _1)]);


		opEqNeq.name("(not) equal");
		opEqNeq =
			opLtGt[_val = _1] > *(((
			lit("==")[_a = Tokens::opEq] |
			lit("!=")[_a = Tokens::opNeq]
			) >
			opLtGt)[_val = lzMakeNode(_a, _val, _1)]);



		opBitAnd.name("bitwise and");
		opBitAnd =
			opEqNeq[_val = _1] > *(((
			lit('&')[_a = Tokens::opBitAnd]
			) >
			opEqNeq)[_val = lzMakeNode(_a, _val, _1)]);


		opBitXor.name("bitwise xor");
		opBitXor =
			opBitAnd[_val = _1] > *(((
			lit('^')[_a = Tokens::opBitXor]
			) >
			opBitAnd)[_val = lzMakeNode(_a, _val, _1)]);


		opBitOr.name("bitwise or");
		opBitOr =
			opBitXor[_val = _1] > *(((
			lit('|')[_a = Tokens::opBitOr]
			) >
			opBitXor)[_val = lzMakeNode(_a, _val, _1)]);



		opLogicAnd.name("logical and");
		opLogicAnd =
			opBitOr[_val = _1] > *(((
			lit("&&")[_a = Tokens::opLogicAnd]
			) >
			opBitOr)[_val = lzMakeNode(_a, _val, _1)]);


		opLogicOr.name("logical or");
		opLogicOr =
			opLogicAnd[_val = _1] > *(((
			lit("||")[_a = Tokens::opLogicOr]
			) >
			opLogicAnd)[_val = lzMakeNode(_a, _val, _1)]);


		opAssign.name("assign operators");
		opAssign = (*(lhs[_val = _1] >> (
					lit('=')[_a = Tokens::opAssign] |
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
					) ) )|
					opLogicOr[_val = _1];

		opSeq.name("sequence operator");
		opSeq = opAssign[_val = _1] > *(lit(',')[_a = val(Tokens::opSeq)] > opAssign[_val = lzMakeNode(_a, _val, _1)]);
		expr.name("expression");
		expr %= opSeq;

		qi::on_error<qi::fail>(
			expr,
			[this](boost::fusion::vector<itrT&, itrT const&, itrT const&, qi::info const&> params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(
				boost::fusion::at_c<0>(params),
				boost::fusion::at_c<1>(params),
				boost::fusion::at_c<2>(params),
				boost::fusion::at_c<3>(params).tag,
				"Unexpected token, maybe missing operators or operands?"
				);
		}
		);


		sentence.name("sentence");
		sentence %= expr > lit(';');
		qi::on_error<qi::fail>(
			sentence,
			[this](boost::fusion::vector<itrT&, itrT const&, itrT const&, qi::info const&> params, qi::unused_type, qi::error_handler_result) {
			printErrorMsg(
				boost::fusion::at_c<0>(params),
				boost::fusion::at_c<1>(params),
				boost::fusion::at_c<2>(params),
				boost::fusion::at_c<3>(params).tag,
				"missing semi-colon?"
				);
		}
		);



		block.name("block");
		block = lit('{') > (
			lit('}')[_val = lzMakeNode(val(true), val(Tokens::none))] |
			(sentence[_a = _val = lzMakeNode(val(Tokens::seq), _1)] > 
				*(sentence
					[_b = lzMakeNode(val(Tokens::seq), _1), lzAddNodeContent(_a, val(true), _b), _b = _a]
				)
			)
		);

		ifStc.name("if");
		ifStc = lit("if") > lit('(') > expr > lit(')') > (sentence | block) > -(lit("else") > (sentence | block));
		entry %= sentence | block;
		
	}

};

}

