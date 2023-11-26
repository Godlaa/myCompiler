#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "СToken.h"
#include <map>
#include <vector>
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
	std::vector<eKeyWords> starters_begpart = { kwVar, kwBegin };
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
			else curToken = lexer->getNextToken();
		}
		catch (const std::exception&)
		{
			throw exception("ident");
		}
	}
	bool belong(vector<eKeyWords> starters)
	{
		for (const auto& starter : starters) {
			if (curToken->type == ttKeywords && get_keyword() == starter) {
				return true;
			}
		}
		return false;
	}
	void skipto(std::vector<eKeyWords> starters, std::vector<eKeyWords> followers) 
	{
		while (!belong(followers))
		{
			getNext();
		}
	}
	std::vector<eKeyWords> set_disjunct(std::vector<eKeyWords> starters, std::vector<eKeyWords> followers)
	{
		std::vector<eKeyWords> new_vec;
		for (const auto& starter : starters) 
		{
			new_vec.push_back(starter);
		}
		for (const auto& follower : followers)
		{
			new_vec.push_back(follower);
		}
		return new_vec;
	}
	void program() {
		std::vector<eKeyWords> followers = { };
		getNext();
		accept(kwProgram);
		accept_ident();
		accept(ssSemicolon);
		block(followers);
		accept(ssDot);
	}
	void block(std::vector<eKeyWords> followers)  // block
	{
		if (!belong(starters_begpart) )
		{
			cout << " ошибка в разделе описаний "; /* ошибка в разделе описаний */
			skipto(starters_begpart, followers);
		}
		if (belong(starters_begpart))
		{
			var_block();
			op_block(set_disjunct(starters_begpart, followers));
		}
		
	}
	void var_block() /* анализ конструкции <раздел переменных> */
	{ 
		accept(kwVar);
		same_var();
		accept(ssSemicolon);
	}
	void same_var() /* анализ конструкции <описание однотипных переменных> */
	{ 
		accept_ident();
		while (get_spec() == ssComma)
		{
			accept(ssComma);
			accept_ident();
		}
		accept(ssColon);
		type();
	}
	void type() 
	{ 
		accept_ident();
	}
 	void op_block(std::vector<eKeyWords> followers)
	{
		compoundstatement(followers);
	}
	void compoundstatement(std::vector<eKeyWords> followers) /* анализ конструкции <составной оператор> */
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
	void whilestatement(std::vector<eKeyWords> followers) /* анализ конструкции <цикл с предусловием> */
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
	void ifstatement(std::vector<eKeyWords> followers) /* анализ конструкции <условный оператор> */
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
	void forstatement(std::vector<eKeyWords> followers) /* анализ конструкции <цикл с параметром> */
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
	void statement(std::vector<eKeyWords> followers) /* анализ конструкции <оператор> */
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