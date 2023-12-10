#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class Assembly
{
public:
	Assembly() {
		out.open(path);
		out << ".data\nfmtStr byte 'Result is: %d', 10, 0";
	}
	~Assembly() {	
		out.close();
	}
	void value_to_stack(string data) {
		string code = "\nmov rdx," + data + "\npush rdx";
		out << code;
	}
	void add(){
		out << "\npop rdx\npop rax\nadd rax, rdx\npush rax";
	}
	void substract() {
		out << "\npop rdx\npop rax\nsub rax, rdx\npush rax";
	}
	void mult() {
		out << "\npop rdx\npop rax\nmul rdx\npush rax";
	}
	void division() {
		out << "\npop eax\npop ax\ncdq\nidiv rdx\npush rax";
	}
	void assigment(string var) {
		string code = "\npop rax\nmov " + var + string(", rax");
		out << code;
	}
	void write_variable(string var) {
		out << var + " QWORD ?\n";
	}
	void prossed() {
		out <<  ".code\nprint proc\nsub rsp, 20h; Allocate stack space for function parameters(32 bytes shadow space)\nlea rcx, fmtStr; param 1: format string\nmov rdx, rax; param 3: the 1st integer specifier(1st % d in the format string)\ncall printf\nadd rsp, 20h; Restore the stack\nret\nprint endp\nexpression proc\n";
	}
	void ending() {
		out << "\ncall print\nret\nexpression ENDP\nend";
	}
	void writeinfile(string var) {
		out << "\nmov rax," + var;
	}
private:
	ofstream out;
	string path = "assembler.asm";
};

