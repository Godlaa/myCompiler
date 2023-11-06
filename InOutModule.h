#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ÑToken.h"
#include "Lexer.h"
using namespace std;

class InOutModule {
private:
    string line = "";
    size_t position = 0;
    string path;
    size_t row = 1;
public:
    InOutModule() {
        this->path = "D:\\myCompiler\\PascalCode.txt";;
    }
    void ReadCode() {
        ifstream file(path);
        cout << "Code\n";
        if (file.is_open())
        {
            while (getline(file, line))
            {
                cout << line;
                cout << '\n';
            }
        }
    }
    void ReadTokens(Lexer lex) {
        ifstream file(path);
        cout << "\n\nTokens\n";
        if (file.is_open())
        {
            while (getline(file, line))
            {
                while (position < line.size()) {
                    shared_ptr<Token> token = lex.getNextToken(position, row, line); // reading tokens in row
                    token->Print();
                }
                row++;
                cout << '\n';
                position = 0;
            }
        }
        file.close();
        cout << lex.exceptions;
    }
};


