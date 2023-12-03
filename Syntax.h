#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "СToken.h"
#include <map>
#include <unordered_set>
#include "Errors.h"

#define starters_type  vector<variant<eSpecialSymbols, eKeyWords>>
#define kw_check curToken -> type == ttKeywords && get_keyword()
#define spec_check curToken -> type == ttSpecialSymbols && get_spec()

class Syntax
{
public:
	Syntax(shared_ptr<Lexer> lexer) { this->lexer = lexer; }
	void run_syntax() {
		program();
	}
private:
	shared_ptr<Token> curToken;
	shared_ptr<Lexer> lexer;
	typedef map<string, eVariantType> var;
	var variables;
	// хватит ловить в try catch и например скипать до [then else] в блоке if 
	void getNext() 
	{
		this->curToken = this->lexer->getNextToken();
	}
	void compatible_to() // приведение типов функция compatible to
	{

	}
	void exist_op() // возможна ли операция между переменными 
	{

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
	void check_variables() 
	{
		if (curToken == NULL || curToken->type != ttIdentifier) throw exception(ident_err);
		else variables.insert(var :: value_type(get_ident(), None)); // map <string, CType> при разборе var - 's' , string
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
				if (var.second == None) var.second = type;
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

	}
	void block( )  // block
	{
		try
		{
			var_block();
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
	void variable() /* анализ конструкции <переменная> */
	{
		try
		{
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
			expression();
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
			accept_ident();
			accept(ssAssigment);
			expression();
			if (curToken->type == ttKeywords && get_keyword() == kwTo || get_keyword() == kwToDownTo)
			{
				getNext();
				expression();
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
			variable();
			accept(ssAssigment);
			expression();
		}
		catch (const std::exception& exp)
		{
			skipto(starters_type { ssSemicolon });
			cout << exp.what();
		}
	}
	void expression() 
	{
		simple_expression();
		if (curToken->type == ttSpecialSymbols && realation_operator())
		{
			simple_expression();
		}
	}
	void simple_expression() 
	{
		term();
		while (curToken->type == ttSpecialSymbols && additive_op())
		{
			term();
		}
	}
	void term() // slogaemoe
	{
		factor();
		if (curToken -> type == ttSpecialSymbols)
		{
			while (mult_op())
			{
				factor();
			}
		}
	}
	void factor() // mnozitel
	{ 
		if (curToken->type == ttSpecialSymbols)
		{
			accept(ssLeftCurveBrascet);
			expression();
			accept(ssRightCurveBrascet);
		}
		else
		{
			switch (curToken->type) {
			case ttConstants:
				accept(ttConstants);
				break;
			case ttIdentifier:
				accept(ttIdentifier);
				break;
			}
		}
	}
	bool mult_op() // mult op 
	{
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssDiv:
			accept(ssDiv);
			return true;
			break;
		case ssMult:
			accept(ssMult);
			return true;
			break;
		}
		return false;
	}
	bool additive_op() {
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssMinus:
			accept(ssMinus);
			return true;
			break;
		case ssPlus:
			accept(ssPlus);
			return true;
			break;
		}
		return false;
	}
	bool realation_operator() {
		eSpecialSymbols op;
		op = get_spec();
		switch (op)
		{
		case ssEqual:
			accept(ssEqual);
			return true;
			break;
		case ssNoEqual:
			accept(ssNoEqual);
			return true;
			break;
		case ssGreater:
			accept(ssGreater);
			return true;
			break;
		case ssLesser:
			accept(ssLesser);
			return true;
			break;
		}
		return false;
	}
};