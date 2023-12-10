#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class Assembly
{
public:
	Assembly() {
		out.open(path);
		out << ".data\n";
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
		out <<  ".code\nexpression proc\n";
	}
	void ending() {
		out << "\nret\nexpression ENDP\nend";
	}
	void writeinfile(string var) {
		out << "\nmov rax," + var;
	}
private:
	ofstream out;
	string path = "assembler.txt";
};

