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
};


