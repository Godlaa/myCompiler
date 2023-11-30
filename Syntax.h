#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "�Token.h"
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
	// ������ ������ � try catch � �������� ������� �� [then else] � ����� if 
	void getNext() 
	{
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
		if (curToken == NULL || curToken->type != ttSpecialSymbols || get_spec() != ss) throw exception("expected spec symbol op\n");
		else getNext();
	}
	void accept(eKeyWords kw)
	{
		if (curToken == NULL || curToken->type != ttKeywords || get_keyword() != kw) throw exception("expected another key word\n");
		else getNext();
	}
	void accept(string name)
	{
		if (curToken == NULL || curToken->type != ttConstants || get_constant() != name) throw exception("expected another variant type\n");
		else getNext();
	}
	void accept(eTokenType tt)
	{
		if (curToken == NULL || tt != curToken -> type) throw exception("expected another type\n");
		else getNext();
	}
	void accept_ident()  // ident 
	{
		if (curToken == NULL || curToken ->type != ttIdentifier) throw exception("ident exp");
		else getNext();
	}
	bool belong(vector<variant<eSpecialSymbols, eKeyWords>> starters) {
		for (const auto& starter : starters) 
		{
			if (curToken && (curToken->type == ttSpecialSymbols && holds_alternative<eSpecialSymbols>(starter) && get_spec() == get<eSpecialSymbols>(starter)) || (curToken->type == ttKeywords && holds_alternative<eKeyWords>(starter) && get_keyword() == get<eKeyWords>(starter))) 
			{
				return true;
			}
		}
		return false;
	}
	void skipto(vector<variant<eSpecialSymbols, eKeyWords>> starters)
	{
		while (curToken && !belong(starters))
		{
			getNext();
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
		catch (const std::exception& exp) { skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwVar, kwBegin }); cout << exp.what(); }
	}
	void var_block() /* ������ ����������� <������ ����������> */
	{ 
		try
		{
			accept(kwVar);
			do
			{
				same_var();
				accept(ssSemicolon);
			} while (curToken->type == ttIdentifier);
		}
		catch (const std::exception& exp)
		{
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwBegin }); cout << exp.what();
		}

	}
	void same_var() /* ������ ����������� <�������� ���������� ����������> */
	{ 
		try
			{
			accept_ident();
			while (curToken -> type == ttSpecialSymbols && get_spec() == ssComma)
			{
				accept(ssComma);
				accept_ident();
			}
			accept(ssColon);
			type();
		}
		catch (const std::exception& exp)
		{
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { ssSemicolon, kwBegin }); cout << exp.what();
		}
	}
	void type()
	{ 
		accept_ident();
	}
 	void op_block()
	{
		compoundstatement();
	}
	void compoundstatement() /* ������ ����������� <��������� ��������> */
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { ssDot }); cout << exp.what();
		}
	}
	void whilestatement() /* ������ ����������� <���� � ������������> */
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwWhile, kwFor, kwIf }); cout << exp.what();
		}
	}
	void variable() /* ������ ����������� <����������> */
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { ssAssigment, kwElse }); cout << exp.what();
		}
	}
	void ifstatement() /* ������ ����������� <�������� ��������> */
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwElse }); cout << exp.what();
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwEnd }); cout << exp.what();
		}
	}
	void forstatement() /* ������ ����������� <���� � ����������> */
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwEnd }); cout << exp.what();
		}
	}
	void statement() /* ������ ����������� <��������> */
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
				case kwElse: break; /* � ������ ������� ��������� */
				}
				break;
			}
		}
		catch (const std::exception& exp)
		{
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { kwEnd });
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
			skipto(vector<variant<eSpecialSymbols, eKeyWords>> { ssSemicolon });
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
			// map <string, CType> ��� ������� var - 's' , string
			// ���������� ����� ������� compatible to
			// ������� op �������� �� �������� ����� �����������
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