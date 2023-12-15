#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class Assembly
{
public:
	Assembly() {
		out.open(path);
		out << "includelib ucrt.lib\nincludelib legacy_stdio_definitions.lib\nEXTERN printf : PROC\n.data\ntemp real4 ?\nfmtStr byte 'Result is: %d', 10, 0\nfmtStr2 byte 'Result is: %f', 10, 0\n";
	}
	~Assembly() {
		out.close();
	}
	void value_to_int_stack(string data) {
		string code = "\nmov rdx," + data + "\npush rdx";
		out << code;
	}
	void value_to_float_stack_const(string floatnum) {
		int tens = 1;
		string code = "";
		string int_part = "";
		string float_part = "";
		for (size_t i = 0; i < floatnum.length(); i++)
		{
			if (isdigit(floatnum[i])) int_part += floatnum[i];
			if (floatnum[i] == '.')
			{
				for (size_t j = i + 1; j < floatnum.length(); j++)
				{
					if (floatnum[j] != '0') float_part += floatnum[j];
					tens *= 10;
				}
				break; 
			}
		}
		if (int_part == "0")
		{
			code = "\nmov rcx," + float_part + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp";
			code += "\nmov rcx," + to_string(tens) + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp";
			code += "\nfdiv";
		}
		else if(float_part != "")
		{
			code = "\nmov rcx," + float_part + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp";
			code += "\nmov rcx," + to_string(tens) + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp";
			code += "\nfdiv";

			code += "\nmov rcx," + int_part + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp" + "\nfadd";
		}
		else
		{
			code += "\nmov rcx," + int_part + "\ncvtsi2ss xmm1, rcx" + "\nmovd temp, xmm1" + "\nfld temp";
		}
		out << code;
	}
	void value_to_float_stack_ident(string data) {
		string code = "\nfld " + data;
		out << code;
	}
	void add(eVariantType vt1, eVariantType vt2){
		if (vt1 == vtInt && vt2 == vtInt)
		{
			out << "\npop rdx\npop rax\nadd rax, rdx\npush rax";
		}
		else if(vt1 == vtReal && vt2 == vtReal)
		{
			out << "\nfadd";
		}
		else // случ int - float | float - int
		{
			out << "\npop rax\ncvtsi2ss xmm1, rax\nmovd temp, xmm1\nfld temp\nfadd";
		}
	}
	void substract(eVariantType vt1, eVariantType vt2) {
		if (vt1 == vtInt && vt2 == vtInt)
		{
			out << "\npop rdx\npop rax\nsub rax, rdx\npush rax";
		}
		else if (vt1 == vtReal && vt2 == vtReal)
		{
			out << "\nfsub";
		}
		else // случ int - float | float - int 
		{
			out << "\npop rax\ncvtsi2ss xmm1, rax\nmovd temp, xmm1\nfld temp\nfsub";
		}
	}
	void mult(eVariantType vt1, eVariantType vt2) {
		if (vt1 == vtInt && vt2 == vtInt)
		{
			out << "\npop rdx\npop rax\nmul rdx\npush rax";
		}
		else if (vt1 == vtReal && vt2 == vtReal)
		{
			out << "\nfmul";
		}
		else // случ int - float | float - int 
		{
			out << "\npop rax\ncvtsi2ss xmm1, rax\nmovd temp, xmm1\nfld temp\nfmul";
		}	
	}
	void division(eVariantType vt1, eVariantType vt2) {
		if (vt1 == vtInt && vt2 == vtInt)
		{
			out << "\npop rbx\npop rax\ncqo\nidiv rbx\npush rax";
		}
		else if (vt1 == vtReal && vt2 == vtReal)
		{
			out << "\nfdiv";
		}
		else // случ int - float | float - int
		{
			out << "\npop rax\ncvtsi2ss xmm1, rax\nmovd temp, xmm1\nfld temp\nfdiv";
		}
	}
	void assigment(string var, eVariantType vt1, eVariantType vt2) {
		string code = "";
		if (vt2 == vtInt && vt1 == vtInt)
		{
			code = "\npop rax\nmov " + var + string(", rax");
			out << code;
		}
		else if (vt1 == vtReal && vt2 == vtReal)
		{
			code = "\nfstp " + var;
			out << code;
		}
		else if(vt1 == vtInt)
		{
			code = "\nfistp " + var;
			out << code;
		}
	}
	void write_variable(string var, eVariantType vt) {
		switch (vt)
		{
		case vtInt:
			out << var + " QWORD ?\n";
			break;
		case vtReal:
			out << var + " real4 ?\n";
			break;
		case None:
			break;
		}
	}
	void prossed() {
		string code = "\n.code\nprint proc\nsub rsp, 20h\nlea rcx, fmtStr; param 1: format string\nmov rdx, rax\ncall printf\nadd rsp, 20h; Restore the stack\nret\nprint endp\n" +
			string ("\nprintFloat proc\nsub rsp, 20h\nmovss xmm0, [temp]\ncvtss2sd xmm1, xmm0\nmovd rdx, xmm1\nlea rcx, fmtStr2\ncall printf\nadd rsp, 20h\nret\nprintFloat endp\nexpression proc\n");
		out << code;
	}
	void ending() {
		out << "\nret\nexpression ENDP\nend";
	}
	void print_to_console(string var, eVariantType vt) {
		switch (vt)
		{
		case vtInt:
			out << "\nmov rax," + var + "\ncall print";
			break;
		case vtReal:
			out << "\nfld " + var + "\nfstp temp" + "\ncall printfloat";
			break;
		default:
			break;
		}	
	}
private:
	ofstream out;
	string path = "assembler.asm";
};

