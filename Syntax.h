#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ÑToken.h"
#include <map>
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

	void getNext() {
		this->curToken = this->lexer->getNextToken();
	}
	bool accept(eSpecialSymbols ss)
	{
		// For SpecialSumbols
		try
		{
			if (curToken == NULL || curToken->type != ttSpecialSymbols || dynamic_cast<SpecialSymbols*>(curToken.get())->ss != ss) throw exception("expected spec symbol op\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
			return false;
		}
		return true;
	}
	bool accept(eKeyWords kw)
	{
		// For KeyWords
		try
		{
			if (curToken == NULL || curToken->type != ttKeywords || dynamic_cast<KeyWordToken*>(curToken.get())->kw != kw) throw exception("expected another key word\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
			return false;
		}
		return true;
	}
	bool accept(string name)
	{
		// For VariantType
		try
		{
			if (curToken == NULL || curToken->type != ttConstants || get<string>(dynamic_cast<ConstToken*>(curToken.get())->data) != name) throw exception("expected another variant type\n");
			else getNext();
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
			return false;
		}
		return true;
	}
	bool accept_ident() { // name
		try
		{
			if (curToken == NULL || curToken ->type != ttIdentifier) throw exception("ident exp");
			else curToken = lexer->getNextToken();
		}
		catch (const std::exception&)
		{
			throw exception("expected another key word");
			return false;
		}
		return true;
	}
	void program() {
		getNext();
		accept(kwProgram);
		accept_ident();
		accept(ssSemicolon);
		block();
		accept(ssDot);
	}
	void block() { // block
		accept(kwBegin);
		var_block();
		op_block();
	}
	void var_block() { // variables block
		accept(kwVar);
		same_var();
		accept(ssSemicolon);
	}
	void same_var() { // same variables
		accept_ident();
		while (accept(ssComma))
		{
			accept_ident();
		}
		accept(ssColon);
		type();
	}
	void type() { // types
		accept_ident();
	}
	void op_block() { // operators block
		comp_op();
	}
	void comp_op() { // sostavnoi operator

	}

};