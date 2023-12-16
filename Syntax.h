#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "СToken.h"
#include <map>
#include <unordered_set>
#include "Errors.h"
#include "Assembly.h"

#define starters_type  vector<variant<eSpecialSymbols, eKeyWords>>
#define kw_check curToken -> type == ttKeywords && get_keyword()
#define spec_check curToken -> type == ttSpecialSymbols && get_spec()

class Syntax
{
public:
	Syntax(shared_ptr<Lexer> lexer) { this->lexer = lexer; }
	void run_syntax() {
		zero_level_variables();
		program();
	}
private:
	shared_ptr<Token> curToken;
	shared_ptr<Lexer> lexer;
	Assembly asm_file;
	typedef map<string, pair<int, eVariantType>> var;
	var variables;
	int cur_scope = 0;
	// хватит ловить в try catch и например скипать до [then else] в блоке if 
	void getNext() 
	{
		this->curToken = this->lexer->getNextToken();
	}
	bool compatible_to(eVariantType type1, eVariantType type2) // приведение типов функция compatible to
	{
		if (type1 == type2) {
			return true;
		}
		else if (type1 == vtInt && type2 == vtReal) {
			return true;
		}
		else if (type1 == vtReal && type2 == vtReal) {
			return true;
		}
		return false;
	}
	eVariantType exist_op(eVariantType type1, eVariantType type2, eSpecialSymbols operation) // возможна ли операция между переменными 
	{
		switch (operation)
		{
		case ssEqual:
		case ssNoEqual:
			if (type1 == type2) {
				return vtBool; 
			}
			else {
				return None; 
			}
			break;

		case eSpecialSymbols::ssPlus:
		case eSpecialSymbols::ssMinus:
		case eSpecialSymbols::ssMult:
			if (type1 == vtInt && type2 == vtInt) {
				return vtInt; 
			}
			else if ((type1 == vtReal || type1 == vtInt) &&
				(type2 == vtReal || type2 == vtInt)) {
				return vtReal;
			}
			return None; 

		case eSpecialSymbols::ssDiv:
			if ((type1 == vtReal || type1 == vtInt) &&
				(type2 == vtReal || type2 == vtInt)) {
				return vtReal; 
			}
			return None;

		case eSpecialSymbols::ssMod:
			if (type1 == vtInt && type2 == vtInt) {
				return vtInt; 
			}
			return None;

		case eSpecialSymbols::ssGreater:
			if ((type1 == vtReal || type1 == vtInt) &&
				(type2 == vtReal || type2 == vtInt)) {
				return vtBool;
			}
			else if (type1 == vtString && type2 == vtString) {
				return vtBool;
			}
			return None;
		case eSpecialSymbols::ssLesser:
			if ((type1 == vtReal || type1 == vtInt) &&
				(type2 == vtReal || type2 == vtInt)) {
				return vtBool;
			}
			else if (type1 == vtString && type2 == vtString) {
				return vtBool; 
			}
			return None; 
		default:
			return None;
		}
	}
	eVariantType exist_op(eVariantType type1, eVariantType type2, eKeyWords op) // возможна ли операция между переменными 
	{
		// Логические операции для типа bool
		if ((op == kwAnd || op == kwOr|| op == kwNot) &&
			(type1 == vtBool && type2 == vtBool)) {
			return vtBool;
		}
		no_op_between;
		return None;
	}
	eSpecialSymbols get_spec() {
		try
		{
			return dynamic_cast<SpecialSymbols*>(curToken.get())->ss;
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	eKeyWords get_keyword() {
		try
		{
			return dynamic_cast<KeyWordToken*>(curToken.get())->kw;
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	string get_constant() {
		try
		{
			return get<string>(dynamic_cast<ConstToken*>(curToken.get())->data);
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	string get_ident() 
	{
		try
		{
			return (dynamic_cast<IdentToken*>(curToken.get())->ident);
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void accept(eSpecialSymbols ss)
	{
		if (curToken == NULL || curToken->type != ttSpecialSymbols || get_spec() != ss) throw exception(spec_symbol_err);
		else getNext();
	}
	void accept(eKeyWords kw) 
	{
		if (curToken == NULL || curToken->type != ttKeywords || get_keyword() != kw) throw exception(keyword_err);
		else getNext();
	}
	void accept(eTokenType tt)
	{
		if (curToken == NULL || tt != curToken -> type) throw exception(token_type_err);
		else getNext();
	}
	void accept_ident()  // ident 
	{
		if (curToken == NULL || curToken ->type != ttIdentifier) throw exception(ident_err);
		else getNext();
	}
	bool belong(starters_type starters) {
		for (const auto& starter : starters) 
		{
			if (curToken && (curToken->type == ttSpecialSymbols && holds_alternative<eSpecialSymbols>(starter) && get_spec() == get<eSpecialSymbols>(starter)) || (curToken->type == ttKeywords && holds_alternative<eKeyWords>(starter) && get_keyword() == get<eKeyWords>(starter))) 
			{
				return true;
			}
		}
		return false;
	}
	void skipto(starters_type starters)
	{
		while (curToken && !belong(starters))
		{
			getNext();
		}
	}
	void zero_level_variables() {
		variables["INT_MAX"] = pair(0, vtInt);
		variables["INT_MIN"] = pair(0, vtInt);
		variables["PI"] = pair(0, vtReal);
		variables["EXP"] = pair(0, vtReal);
	}
	var variable_level()
	{
		var level_map;
		for (const auto& element : variables) {
			if (element.second.first <= cur_scope) {
				level_map[element.first] = (element.first, pair(cur_scope, element.second.second));
			}
		}
		return level_map;
	}
	void check_variables() 
	{
		var cur_level_variables = variable_level();
		if (curToken == NULL || curToken->type != ttIdentifier) throw exception(ident_err);
		else if (cur_level_variables.find(get_ident()) != cur_level_variables.end()) throw exception("variable already named\n");
		else { variables[get_ident()] = pair(cur_scope, None); }// map <string, CType> при разборе var - 's' , string
	}
	void check_type()
	{
		eVariantType type;
		if (curToken == NULL || curToken->type != ttIdentifier) throw exception(ident_err);
		else 
		{
			if (get_ident() == "integer") type = vtInt;
			else if (get_ident() == "bool")  type = vtBool;
			else if (get_ident() == "string")  type = vtString;
			else if (get_ident() == "real")  type = vtReal;
			else throw exception("unknown type");
			for (auto& var : variables)
			{
				if (var.second.second == None)
				{
					var.second.second = type;
					asm_file.write_variable(var.first, type);
				}
			}
		}
	}
	void program() {
		getNext();
		try
		{
			accept(kwProgram);
			accept_ident();
			accept(ssSemicolon);
			block();
			accept(ssDot);
		}
		catch (const std::exception& exp) { cout << exp.what(); }
		asm_file.ending();
	}
	void block( )  // block
	{
		try
		{
			var_block();
			asm_file.prossed();
			op_block();
		}
		catch (const std::exception& exp) { skipto(starters_type { kwVar, kwBegin }); cout << exp.what(); }
	}
	void var_block() /* анализ конструкции <раздел переменных> */
	{ 
		try
		{
			if (kw_check == kwVar)
			{
				accept(kwVar);
				cur_scope++;
				do
				{
					same_var();
					accept(ssSemicolon);
				} while (curToken->type == ttIdentifier);
			}
			else
			{
				skipto(starters_type{ kwBegin });
			}
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type{ kwBegin }); cout << exp.what();
		}

	}
	void same_var() /* анализ конструкции <описание однотипных переменных> */
	{ 
		try
			{
			check_variables();
			accept_ident();
			while (spec_check == ssComma)
			{
				accept(ssComma);
				check_variables();
				accept_ident();
			}
			accept(ssColon);
			type();
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type{ ssSemicolon, kwBegin }); cout << exp.what();
		}
	}
	void type()
	{ 
		check_type();
		accept_ident();
	}
 	void op_block()
	{
		compoundstatement();
	}
	void compoundstatement() /* анализ конструкции <составной оператор> */
	{ 
		try
		{
			accept(kwBegin);
			statement();
			while (curToken->type == ttSpecialSymbols && get_spec() == ssSemicolon)
			{
				accept(ssSemicolon);
				statement();
			}
			accept(kwEnd);
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type{ ssDot }); cout << exp.what();
		}
	}
	void whilestatement() /* анализ конструкции <цикл с предусловием> */
	{
		try
		{
			accept(kwWhile);
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
		expression();
		try
		{
			accept(kwDo);
			statement();
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { kwWhile, kwFor, kwIf }); cout << exp.what();
		}
	}
	eVariantType get_variable_type()
	{
		eVariantType extype = None;
		var cur_level_variables = variable_level();
		if (curToken->type == ttIdentifier) {
			if (cur_level_variables.find(get_ident()) != cur_level_variables.end())
			{
				extype = cur_level_variables.find(get_ident())->second.second;
			}
		}
		return extype;
	}
	string variable() /* анализ конструкции <переменная> */
	{
		try
		{		
			string var;
			var = string(dynamic_cast<IdentToken*>(curToken.get())->ident);
			accept_ident();	
			if (curToken->type == ttSpecialSymbols)
			{
				while (get_spec() == ssLeftCurveBrascet || get_spec() == ssDot)
				{
					switch (get_spec())
					{
					case ssLeftCurveBrascet:
						accept(ssLeftCurveBrascet);
						expression();
						while (get_spec() == ssComma)
						{
							accept(ssComma);
							expression();
						}
						accept(ssRightCurveBrascet);
						break;
					case ssDot:
						accept(ssDot);
						accept_ident();
						break;
					}
				}
			}
			return var;
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type{ ssAssigment, kwElse }); cout << exp.what();
		}
	}
	void ifstatement() /* анализ конструкции <условный оператор> */
	{
		try
		{
			accept(kwIf);
			auto exptype = expression();
			if (!compatible_to(exptype, vtBool)) compatible_error;
			accept(kwThen);
			statement();
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { kwElse }); cout << exp.what();
		}
		try
		{
			if (curToken->type == ttKeywords && get_keyword() == kwElse)
			{
				accept(kwElse);
				statement();
			}
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type{ kwEnd }); cout << exp.what();
		}
	}
	void forstatement() /* анализ конструкции <цикл с параметром> */
	{
		try
		{
			accept(kwFor);
			accept_ident(); // на самом деле может быть здесь тип не для цикла 
			accept(ssAssigment);
			auto exptype = expression();
			if (!compatible_to(vtInt, exptype)) compatible_error;
			if (curToken->type == ttKeywords && get_keyword() == kwTo || get_keyword() == kwToDownTo)
			{
				getNext();
				auto exptype = expression();
				if (!compatible_to(vtInt, exptype)) compatible_error;
				accept(kwDo);
				statement();
			}
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { kwEnd }); cout << exp.what();
		}
	}
	void statement() /* анализ конструкции <оператор> */
	{
		eVariantType vt;
		try
		{
			switch (curToken->type)
			{
			case ttIdentifier:
				assignment();
				break;
			case ttKeywords:
				switch (get_keyword())
				{
				case kwBegin:
					compoundstatement(); break;
				case kwIf:
					ifstatement(); break;
				case kwWhile:
					whilestatement(); break;
				case kwFor:
					forstatement(); break;
				case kwEnd:
					break;
				case kwWriteln:
					accept(kwWriteln);
					vt = get_variable_type();
					asm_file.print_to_console(get_ident(), vt);
					accept_ident();

					break;
				case kwUntil:
					break;
				case kwElse: break; /* в случае пустого оператора */
				}
				break;
			}
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { kwEnd });
			cout << exp.what();
		}
	}
	void assignment()
	{
		try
		{
			auto vartype = get_variable_type();
			auto var = variable();
			accept(ssAssigment);
			auto exptype = expression(); 	
			asm_file.assigment(var, vartype, exptype);
			if (!compatible_to(vartype, exptype)) compatible_error;
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { ssSemicolon });
			cout << exp.what();
		}
	}
	eVariantType expression()
	{
		eSpecialSymbols res = ssNone;
		auto ex1type = simple_expression();
		if (curToken->type == ttSpecialSymbols)
		{
			res = realation_operator();
			if (res != ssNone)
			{
				auto ex2type = simple_expression();
				ex1type = exist_op(ex1type, ex2type, res);
				if (ex1type == None) no_op_between;
			}
		}
		return ex1type;
	}
	eVariantType simple_expression()
	{
		eSpecialSymbols res = ssNone;
		auto ex1type = term();
		if (curToken->type == ttSpecialSymbols)
		{
			if (curToken->type == ttSpecialSymbols) res = additive_op();
			else res = ssNone;
		}
		while (res != ssNone)
		{
			auto ex2type = term();
			eVariantType temp = ex1type;
			ex1type = exist_op(ex1type, ex2type, res);
			if (ex1type == None) no_op_between;
			if (curToken->type == ttSpecialSymbols) 
			{ 
				switch (res)
				{
				case ssPlus:
					asm_file.add(temp, ex2type);
					break;
				case ssMinus:
					asm_file.substract(temp, ex2type);
					break;
				default:
					break;
				}
				res = additive_op(); 
			}
			else res = ssNone;
		}
		return ex1type;
	}
	eVariantType term() // slogaemoe
	{
		eSpecialSymbols res = ssNone;
		auto ex1type = factor();
		if (curToken->type == ttSpecialSymbols) res = mult_op();
		while (res != ssNone)
		{
			auto ex2type = factor();
			eVariantType temp = ex1type;
			ex1type = exist_op(ex1type, ex2type, res);
			if (ex1type == None) no_op_between;
			if (curToken->type == ttSpecialSymbols) 
			{
				switch (res)
				{
				case ssDiv:
					asm_file.division(temp, ex2type);
					break;
				case ssMult:
					asm_file.mult(temp, ex2type);
					break;
				default:
					break;
				}
				 res = mult_op();
			}
			else res = ssNone;
		}
		return ex1type;
	}
	eVariantType factor() // mnozitel
	{ 
		eVariantType exptype = None;
		if (curToken->type == ttSpecialSymbols)
		{
			accept(ssLeftCurveBrascet);
			exptype = expression();
			accept(ssRightCurveBrascet);
		}
		else
		{
			switch (curToken->type) {
			case ttConstants:
				switch (dynamic_cast<ConstToken*>(curToken.get())->data.index())
				{
					case 0:
						exptype = vtInt;
						asm_file.value_to_int_stack(to_string(dynamic_cast<ConstToken*>(curToken.get())->Get_Int()));
						break;
					case 1:
						exptype = vtReal;
						asm_file.value_to_float_stack_const(dynamic_cast<ConstToken*>(curToken.get())->Get_Float());
						break;
					case 2:
						exptype = vtString;
						break;
					case 3:
						exptype = vtBool;
						break;
					default: /* ошибка */
						exptype = None;
				}
				accept(ttConstants);
				break;
			case ttIdentifier:
				// check that it is variable
				exptype = get_variable_type();
				switch (exptype)
				{
				case vtInt:
					asm_file.value_to_int_stack(dynamic_cast<IdentToken*>(curToken.get())->ident);
					break;
				case vtReal:
					asm_file.value_to_float_stack_ident(dynamic_cast<IdentToken*>(curToken.get())->ident);
					break;
				default:
					break;
				}
				
				accept(ttIdentifier);
				break;
			}
		}
		return exptype;
	}
	eSpecialSymbols mult_op() // mult op 
	{
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssDiv:
			accept(ssDiv);
			return op;
			break;
		case ssMult:
			accept(ssMult);
			return op;
			break;
		}
		return ssNone;
	}
	eSpecialSymbols additive_op() {
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssMinus:
			accept(ssMinus);
			return op;
			break;
		case ssPlus:
			accept(ssPlus);
			return op;
			break;
		}
		return ssNone;
	}
    eSpecialSymbols realation_operator() {
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssEqual:
			accept(ssEqual);
			return op;
			break;
		case ssNoEqual:
			accept(ssNoEqual);
			return op;
			break;
		case ssGreater:
			accept(ssGreater);
			return op;
			break;
		case ssLesser:
			accept(ssLesser);
			return op;
			break;
		}
		return ssNone;
	}
};