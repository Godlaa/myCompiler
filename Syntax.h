#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ÑToken.h"
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
	void accept(eSpecialSymbols ss)
	{
		// For SpecialSumbols
		try
		{
			if (curToken == NULL || curToken->type != ttSpecialSymbols || dynamic_cast<SpecialSymbols*>(curToken.get())->ss != ss)
			{
				throw exception("expected another op");
			}
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
			if (curToken == NULL || curToken->type != ttKeywords || dynamic_cast<KeyWordToken*>(curToken.get())->kw != kw)
			{
				throw exception("expected another key word");
			}
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void accept(string vt)
	{
		// For VariantType
		try
		{
			if (curToken == NULL || curToken->type != ttConstants || get<string>(dynamic_cast<ConstToken*>(curToken.get())->data) != vt)
			{
				throw exception("expected another key word");
			}
		}
		catch (const std::exception& exp)
		{
			cout << exp.what();
		}
	}
	void program() {
		accept("program");
		name();
		accept(ssSemicolon);
		block();
		accept(ssDot);
	}
	void name() { // name
		try
		{
			if (curToken != NULL) get<string>(dynamic_cast<ConstToken*>(curToken.get())->data);
		}
		catch (const std::exception&)
		{
			throw exception("expected another key word");
		}
	}
	void block() { // block
		var_block();
		op_block();
	}
	void var_block() { // variabels block
		accept("var");
		same_var();
		// { same_var() }
		accept(ssSemicolon);
	}
	void same_var() { // odnotipnie variables

	}
	void op_block() { // operators block
		comp_op();
	}
	void comp_op() { // sostavnoi operator
		///// ! 
	}
	//bool isOper(List<Oper> ops)
	//{
	//	if (curToken == null || curToken.tokenType != ToketType.ttOper) return false;
	//	foreach(var op in ops)
	//	{
	//		if (curToken.operation == op) return true;
	//	}
	//	return false;
	//}

	//float factor() // mnozitel
	//{
	//	if (curToken != null && curToken.tokenType == ToketType.ttOper)
	//	{
	//		accept(Oper.oLeftBracet);
	//		var left = simpleExpr();
	//		accept(Oper.oRightBracet);
	//		return left;
	//	}
	//	if (curToken == null) throw new Exception("Token excpected");
	//	var res = curToken.value;
	//	getNext();
	//	return res;
	//}

	//float term() //slagaemoe
	//{
	//	var left = factor();
	//	while (isOper(new List<Oper>() { Oper.oMul, Oper.oDiv }))
	//	{
	//		var oper = curToken.operation;
	//		getNext();
	//		var right = factor();
	//		switch (oper)
	//		{
	//		case Oper.oMul:
	//			left = left * right;
	//			break;
	//		case Oper.oDiv:
	//			left = left / right;
	//			break;
	//		default:
	//			throw new Exception("bad multipl oper");
	//		}
	//	}
	//	return left;
	//}

	//float simpleExpr()
	//{
	//	var left = term();
	//	while (isOper(new List<Oper>() { Oper.oPlus, Oper.oMinus }))
	//	{
	//		var oper = curToken.operation;
	//		getNext();
	//		var right = term();
	//		switch (oper)
	//		{
	//		case Oper.oPlus:
	//			left = left + right;
	//			break;
	//		case Oper.oMinus:
	//			left = left - right;
	//			break;
	//		default:
	//			throw new Exception("bad addit oper");
	//		}
	//	}
	//	return left;
	// }
};