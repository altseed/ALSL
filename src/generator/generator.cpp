#include "generator.hpp"
namespace ALSL {

	using boost::get;
	void Generator::generate(std::ostream& os, std::shared_ptr<Node> node) {

		switch(node->token) {
		case Tokens::none:
			gen_none(os, node);
			break;

		case Tokens::intLit:
			gen_intLit(os, node);
			break;

		case Tokens::floatLit:
			gen_floatLit(os, node);
			break;

		case Tokens::doubleLit:
			gen_doubleLit(os, node);
			break;

		case Tokens::strLit:
			gen_strLit(os, node);
			break;

		case Tokens::identif:
			gen_identif(os, node);
			break;

		case Tokens::paren:
			gen_paren(os, node);
			break;


		case Tokens::call:
			gen_call(os, node);
			break;

		case Tokens::arraySubscript:
			gen_arraySubscript(os, node);
			break;

		case Tokens::swizzleOp:
			gen_swizzleOp(os, node);
			break;

		case Tokens::dataMember:
			gen_dataMember(os, node);
			break;



		case Tokens::opPos:
			gen_opPos(os, node);
			break;

		case Tokens::opNeg:
			gen_opNeg(os, node);
			break;

		case Tokens::opInv:
			gen_opInv(os, node);
			break;

		case Tokens::opNot:
			gen_opNot(os, node);
			break;


		case Tokens::opMult:
			gen_opMult(os, node);
			break;

		case Tokens::opDiv:
			gen_opDiv(os, node);
			break;

		case Tokens::opMod:
			gen_opMod(os, node);
			break;


		case Tokens::opAdd:
			gen_opAdd(os, node);
			break;

		case Tokens::opSub:
			gen_opSub(os, node);
			break;


		case Tokens::opLsh:
			gen_opLsh(os, node);
			break;

		case Tokens::opRsh:
			gen_opRsh(os, node);
			break;


		case Tokens::opGt:
			gen_opGt(os, node);
			break;

		case Tokens::opLt:
			gen_opLt(os, node);
			break;

		case Tokens::opGte:
			gen_opGte(os, node);
			break;

		case Tokens::opLte:
			gen_opLte(os, node);
			break;


		case Tokens::opEq:
			gen_opEq(os, node);
			break;

		case Tokens::opNeq:
			gen_opNeq(os, node);
			break;


		case Tokens::opBitAnd:
			gen_opBitAnd(os, node);
			break;


		case Tokens::opBitXor:
			gen_opBitXor(os, node);
			break;


		case Tokens::opBitOr:
			gen_opBitOr(os, node);
			break;


		case Tokens::opLogicAnd:
			gen_opLogicAnd(os, node);
			break;

		case Tokens::opLogicOr:
			gen_opLogicOr(os, node);
			break;


		case Tokens::opSelect:
			gen_opSelect(os, node);
			break;


		case Tokens::opAssign:
			gen_opAssign(os, node);
			break;

		case Tokens::opAddAssign:
			gen_opAddAssign(os, node);
			break;

		case Tokens::opSubAssign:
			gen_opSubAssign(os, node);
			break;

		case Tokens::opMultAssign:
			gen_opMultAssign(os, node);
			break;

		case Tokens::opDivAssign:
			gen_opDivAssign(os, node);
			break;

		case Tokens::opModAssign:
			gen_opModAssign(os, node);
			break;

		case Tokens::opLshAssign:
			gen_opLshAssign(os, node);
			break;

		case Tokens::opRshAssign:
			gen_opRshAssign(os, node);
			break;

		case Tokens::opBitAndAssign:
			gen_opBitAndAssign(os, node);
			break;

		case Tokens::opBitOrAssign:
			gen_opBitOrAssign(os, node);
			break;

		case Tokens::opBitXorAssign:
			gen_opBitXorAssign(os, node);
			break;


		case Tokens::opSeq:
			gen_opSeq(os, node);
			break;


		case Tokens::seq:
			gen_seq(os, node);
			break;


		case Tokens::stxIf:
			gen_stxIf(os, node);
			break;

		case Tokens::stxWhile:
			gen_stxWhile(os, node);
			break;

		case Tokens::stxFor:
			gen_stxFor(os, node);
			break;

		case Tokens::stxDoWhile:
			gen_stxDoWhile(os, node);
			break;

		case Tokens::stxFunc:
			gen_stxFunc(os, node);
			break;

		case Tokens::stxStruct:
			gen_stxStruct(os, node);
			break;

		case Tokens::type:
			gen_type(os, node);
			break;

		case Tokens::declVar:
			gen_declVar(os, node);
			break;

		case Tokens::declConst:
			gen_declConst(os, node);
			break;

		case Tokens::declProp:
			gen_declProp(os, node);
			break;


		case Tokens::kwdReturn:
			gen_kwdReturn(os, node);
			break;

		case Tokens::kwdBreak:
			gen_kwdBreak(os, node);
			break;

		case Tokens::kwdContinue:
			gen_kwdContinue(os, node);
			break;

		default:
			break;
		}
	}


	void Generator::gen_none(std::ostream& os, std::shared_ptr<Node> const node) {os << "/* none */";}
	void Generator::gen_intLit(std::ostream& os, std::shared_ptr<Node> const node) {os << get<int>(node->contents.back());}
	void Generator::gen_floatLit(std::ostream& os, std::shared_ptr<Node> const node) { os << std::showpoint << get<float>(node->contents.back()) << "f"; }
	void Generator::gen_doubleLit(std::ostream& os, std::shared_ptr<Node> const node) { os << std::showpoint << get<double>(node->contents.back()); }
	void Generator::gen_strLit(std::ostream& os, std::shared_ptr<Node> const node) { os << '"' << get<std::string>(node->contents.back()) << '"'; }
	void Generator::gen_identif(std::ostream& os, std::shared_ptr<Node> const node) {
		auto const name = get<std::string>(node->contents.back());
		os << name;
	}
	void Generator::gen_paren(std::ostream& os, std::shared_ptr<Node> const node) { os << '('; genNextNode(os, node->contents.back()); os << ')'; }


	void Generator::gen_call(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++));
		os << '(';
		if(itr != node->contents.cend()) {
			genNextNode(os, *(itr++));
			for(;itr != node->contents.cend(); itr++) {
				os << ", ";
				genNextNode(os, *itr);
			}
		}
		os << ")";
	}
	void Generator::gen_arraySubscript(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++)); // name
		os << '[';
		if(itr == node->contents.cend()) {return;} // TODO:error handling
		genNextNode(os, *(itr++));
		os << "]";
	}
	void Generator::gen_swizzleOp(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++)); // name
		os << '.';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_dataMember(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++)); // name
		os << '.';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));

	}


	void Generator::gen_opPos(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << '+';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opNeg(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << '-';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opInv(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << '~';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opNot(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << '!';
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));

	}

	void Generator::gen_opMult(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " * ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opDiv(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " / ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opMod(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " % ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opAdd(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " + ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opSub(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " - ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opLsh(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " << ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opRsh(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " >> ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opGt(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " > ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opLt(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " < ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opGte(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " >= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opLte(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " <= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opEq(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " == ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opNeq(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " != ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opBitAnd(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " & ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opBitXor(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " ^ ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opBitOr(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " | ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opLogicAnd(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " && ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opLogicOr(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " || ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opSelect(std::ostream& os, std::shared_ptr<Node> const node) {} // Not supported for now

	void Generator::gen_opAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " = ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opAddAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " += ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opSubAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " -= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opMultAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " *= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opDivAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " /= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opModAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " %= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opLshAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " <<= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opRshAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " >>= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opBitAndAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " &= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opBitOrAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " |= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}
	void Generator::gen_opBitXorAssign(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << " ^= ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_opSeq(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
		os << ", ";
		if(itr == node->contents.cend()) { return; }
		genNextNode(os, *(itr++));
	}

	void Generator::gen_seq(std::ostream& os, std::shared_ptr<Node> const node) {
		for(auto const& cont : node->contents) {
			genNextNode(os, cont);
			auto const tok = getNode(cont).token;
			if(
				tok != Tokens::stxFunc &&
				tok != Tokens::stxIf &&
				tok != Tokens::stxWhile &&
				tok != Tokens::stxDoWhile &&
				tok != Tokens::stxFor &&
				tok != Tokens::stxStruct
			) { os << ';'; }
			genNL(os);
		}

	
	}

	void Generator::gen_stxIf(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		bool isToAddSemiColon = false;
		os << "if(";
		if(itr != node->contents.cend() && isNode(*itr)) {
			genNextNode(os, *(itr++));
		} else {} // err
		os << ")";
		if(itr == node->contents.cend() || !isNode(*itr)) { return; } // err
		if(getNode(*itr).token == Tokens::none) {
			os << " {}";
		} else if(getNode(*itr).token == Tokens::seq) {
			os << " {";
			indent++;
			genNL(os);
			genNextNode(os, *itr);
			indent--;
			genNL(os);
			os << "}";
		} else {
			os << " ";
			genNextNode(os, *itr);
			isToAddSemiColon = true;
		}
		itr++;
		if(itr == node->contents.cend()) {
			if(isToAddSemiColon) {
				os << ";";
			}
			genNL(os);
			return;
		} // normal exit

		os << " else";
		if(!isNode(*itr)) { return; } // err

		if(getNode(*itr).token == Tokens::none) {
			os << " {}";
			genNL(os);
		} else if(getNode(*itr).token == Tokens::seq) {
			os << " {";
			indent++;
			genNL(os);
			genNextNode(os, *itr);
			indent--;
			genNL(os);
			os << "}";
		} else {
			os << " ";
			isToAddSemiColon = true;
			genNextNode(os, *itr);
		}
		if(isToAddSemiColon) {
			os << ";";
			genNL(os);
		}
	}
	void Generator::gen_stxWhile(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << "while(";
		if(itr != node->contents.cend() && isNode(*itr)) {
			genNextNode(os, *(itr++));
		} else {} // err
		os << ")";
		if(itr == node->contents.cend() || !isNode(*itr)) { return; } // err
		if(getNode(*itr).token == Tokens::none) {
			os << " {}";
			genNL(os);
		} else if(getNode(*itr).token == Tokens::seq) {
			os << " {";
			indent++;
			genNL(os);
			genNextNode(os, *itr);
			indent--;
			genNL(os);
			os << "}";
		} else {
			os << " ";
			genNextNode(os, *itr);
			os << ";";
			genNL(os);
		}
	}
	void Generator::gen_stxFor(std::ostream& os, std::shared_ptr<Node> const node) {
	
		auto itr = node->contents.cbegin();
		os << "for(";
		if(itr != node->contents.cend() && isNode(*itr)) {
			if(getNode(*itr).token != Tokens::none){genNextNode(os, *(itr++));}
		} else {} // err
		os << "; ";
		if(itr != node->contents.cend() && isNode(*itr)) {
			if(getNode(*itr).token != Tokens::none) { genNextNode(os, *(itr++)); }
		} else {} // err
		os << "; ";
		if(itr != node->contents.cend() && isNode(*itr)) {
			if(getNode(*itr).token != Tokens::none) { genNextNode(os, *(itr++)); }
		} else {} // err
		os << ")";
		if(itr == node->contents.cend() || !isNode(*itr)) { return; } // err
		if(getNode(*itr).token == Tokens::none) {
			os << " {}";
			genNL(os);
		} else if(getNode(*itr).token == Tokens::seq) {
			os << " {";
			indent++;
			genNL(os);
			genNextNode(os, *itr);
			indent--;
			genNL(os);
			os << "}";
		} else {
			os << " ";
			genNextNode(os, *itr);
			os << ";";
			genNL(os);
		}
	}
	void Generator::gen_stxDoWhile(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << "do";
		genNL(os);
		os << "{";
		indent++;
		genNL(os);
		genNextNode(os, *(itr++));
		indent--;
		os << "} while(";
		genNextNode(os, *(itr++));
		os << ");";
		genNL(os);
		
	}
	void Generator::gen_stxFunc(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++));// type
		os << " ";
		genNextNode(os, *(itr++));// name

		os << '(';
		if(itr != node->contents.cend() && isNode(*itr) && getNode(*itr).token == Tokens::declVar) {
			genNextNode(os, *(itr++));
			for(; itr != node->contents.cend() && isNode(*itr) && getNode(*itr).token == Tokens::declVar; itr++) {
				os << ", ";
				genNextNode(os, *itr);
			}
		}
		os << ")";
		if(itr == node->contents.cend() || !isNode(*itr)) {  return;} // TODO: error handling
		if(getNode(*itr).token == Tokens::none) {os << " {}";}
		else {
			genNL(os);
			os << "{";
			indent++;
			genNL(os);
			genNextNode(os, *itr);
			indent--;
			genNL(os);
			os << "}";
			genNL(os);
		}
	
	}
	void Generator::gen_stxStruct(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << "struct ";
		genNextNode(os, *(itr++)); // name
		genNL(os);
		os << "{";
		indent++;
		for(; itr != node->contents.cend(); itr++) {
			genNL(os);
			genNextNode(os, *itr);
		}
		indent--;
		genNL(os);
		os << "};";
		genNL(os);

	}
	void Generator::gen_type(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++)); // type name
		for(; itr != node->contents.cend(); itr++) {
			os << "[";
			genNextNode(os, *itr); // index of array type
			os << "]";
		}
	}
	void Generator::gen_declVar(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		genNextNode(os, *(itr++));// type
		os << " ";
		genNextNode(os, *(itr++));// name
		if(itr == node->contents.cend()) {
			// os << ";";
		} else {
			os << " = ";
			genNextNode(os, *itr);// init
			// os << ";";
		}
	}
	void Generator::gen_declConst(std::ostream& os, std::shared_ptr<Node> const node) {
		auto itr = node->contents.cbegin();
		os << "const ";
		genNextNode(os, *(itr++));// type
		os << " ";
		genNextNode(os, *(itr++));// name
		if(itr == node->contents.cend()) {
			//err
		} else {
			os << " = ";
			genNextNode(os, *itr);// init
			// os << ";";
		}
	
	}
	void Generator::gen_declProp(std::ostream& os, std::shared_ptr<Node> const node) {}

	void Generator::gen_kwdReturn(std::ostream& os, std::shared_ptr<Node> const node) {
		os << "return";
		if(!node->contents.empty()) {
			os << " ";
			genNextNode(os, node->contents.back());
		}
	}
	void Generator::gen_kwdBreak(std::ostream& os, std::shared_ptr<Node> const node) {
		os << "break";
	}
	void Generator::gen_kwdContinue(std::ostream& os, std::shared_ptr<Node> const node) {
		os << "continue";
	}
}