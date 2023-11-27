#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "СToken.h"
#include <map>
#include <unordered_set>
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
	unordered_set<eKeyWords> st_block = { kwBegin, kwVar };
	unordered_set<eKeyWords> st_var = { kwBegin, kwVar };
	unordered_set<eSpecialSymbols> after_var = { ssSemicolon };
	unordered_set<eTokenType> id_starters = { ttIdentifier };
	void error(int index) {
		switch (index)
		{
		case 1:
			cout << "ошибка в разделе описаний\n"; /* ошибка в разделе описаний */
			break;
		default:
			cout << "XZ\n";
			break;
		}
	}
	void getNext() {
		this->curToken = this->lexer->getNextToken();
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
	void accept(eSpecialSymbols ss)
	{
		// For SpecialSumbols
		try
		{
			if (curToken == NULL || curToken->type != ttSpecialSymbols || get_spec() != ss) throw exception("expected spec symbol op\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void accept(eKeyWords kw)
	{
		// For KeyWords
		try
		{
			if (curToken == NULL || curToken->type != ttKeywords || get_keyword() != kw) throw exception("expected another key word\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void accept(string name)
	{
		// For VariantType
		try
		{
			if (curToken == NULL || curToken->type != ttConstants || get_constant() != name) throw exception("expected another variant type\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void accept_ident()  // ident 
	{
		try
		{
			if (curToken == NULL || curToken ->type != ttIdentifier) throw exception("ident exp");
			else getNext();
		}
		catch (const std::exception&)
		{
			cout << "exception ident";
			getNext();
		}
	}
	bool belong(unordered_set<eKeyWords> starters)
	{
		for (const auto& starter : starters) {
			if (curToken != NULL && curToken->type == ttKeywords && get_keyword() == starter) {
				return true;
			}
		}
		return false;
	}
	void skipto2(unordered_set<eKeyWords> starters, std::unordered_set<eKeyWords> followers)
	{
		while (!belong(starters) && !belong(followers))
		{
			getNext();
		}
	}
	void skipto(unordered_set<eKeyWords> followers)
	{
		while (!belong(followers))
		{
			getNext();
		}
	}
	unordered_set<eKeyWords> set_disjunct(unordered_set<eKeyWords> starters, unordered_set<eKeyWords> followers)
	{
		unordered_set<eKeyWords>new_vec;
		for (const auto& starter : starters) 
		{
			new_vec.insert(starter);
		}
		for (const auto& follower : followers)
		{
			new_vec.insert(follower);
		}
		return new_vec;
	}
	void program() {
		unordered_set<eKeyWords>followers = { };
		getNext();
		accept(kwProgram);
		accept_ident();
		accept(ssSemicolon);
		block(followers);
		accept(ssDot);
	}
	void block(unordered_set<eKeyWords> followers)  // block
	{
		var_block(followers);
		op_block(followers);
		
	}
	void var_block(unordered_set<eKeyWords> followers) /* анализ конструкции <раздел переменных> */
	{ 
		if (!belong(st_var))
		{
			error(1);
			skipto2(st_var, followers);
		}
		if (curToken ->	type == ttKeywords && get_keyword() == kwVar && belong(st_var))
		{
			accept(kwVar);
			same_var(followers);
			accept(ssSemicolon);
		}
	}
	void same_var(unordered_set<eKeyWords>followers) /* анализ конструкции <описание однотипных переменных> */
	{ 
		accept_ident();
		while (get_spec() == ssComma)
		{
			accept(ssComma);
			accept_ident();
		}
		accept(ssColon);
		type(followers);
	}
	void type(unordered_set<eKeyWords>followers)
	{ 
		accept_ident();
	}
 	void op_block(unordered_set<eKeyWords>followers)
	{
		compoundstatement(followers);
	}
	void compoundstatement(unordered_set<eKeyWords>followers) /* анализ конструкции <составной оператор> */
	{ 
		accept(kwBegin); 
		statement(followers);
		while (curToken->type == ttSpecialSymbols && get_spec() == ssSemicolon)
		{
			accept(ssSemicolon); 
			statement(followers);
		}
		accept(kwEnd);
	}
	void whilestatement(unordered_set<eKeyWords>followers) /* анализ конструкции <цикл с предусловием> */
	{
		accept(kwWhile); 
		expression();
		accept(kwDo); 
		statement(followers);
	}
	void variable() /* анализ конструкции <переменная> */
	{
		accept_ident();
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
	void ifstatement(unordered_set<eKeyWords>followers) /* анализ конструкции <условный оператор> */
	{
		accept(kwIf);
		expression();
		accept(kwThen); 
		statement(followers);
		if (curToken->type == ttKeywords && get_keyword() == kwElse)
		{
			accept(kwElse);
			statement(followers);
		}
	}
	void forstatement(unordered_set<eKeyWords>followers) /* анализ конструкции <цикл с параметром> */
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
			statement(followers);
		}
	}
	void statement(unordered_set<eKeyWords>followers) /* анализ конструкции <оператор> */
	{
		switch (curToken -> type)
		{
		case ttIdentifier:
			assignment();
			break;
		case ttKeywords:
			switch (get_keyword())
			{
			case kwBegin:
				compoundstatement(followers); break;
			case kwIf:
				ifstatement(followers); break;
			case kwWhile:
				whilestatement(followers); break;
			case kwFor:
				forstatement(followers); break;
			case kwEnd:
				break;
			case kwUntil:
				break;
			case kwElse: break; /* в случае пустого оператора */
			}
			break;
		}
	}
	void assignment()
	{
		variable();
		accept(ssAssigment);
		expression();
	}
	void expression() 
	{
		eSpecialSymbols oper;
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
				getNext(); // ПОЛУЧИЛИ Константу 
				break;
			case ttIdentifier:
				getNext(); // ПОЛУЧИЛИ ПЕРЕМЕННУЮ ТИПО ПРОСТО 5 ИЛИ 'СТРОКА' И ЧЕ С ЭТИМ ДЕЛАТЬ
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
			return 1;
			break;
		case ssNoEqual:
			accept(ssNoEqual);
			return 1;
			break;
		case ssGreater:
			accept(ssGreater);
			return 1;
			break;
		case ssLesser:
			accept(ssLesser);
			return 1;
			break;
		}
		return 0;
	}
};