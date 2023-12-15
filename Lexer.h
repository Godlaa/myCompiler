#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ÑToken.h"
class Lexer {
private:
    size_t position;
    size_t row;
    ifstream file;
    string exceptions = "";
    string input;
    void Read_input() {
        if (position >= input.size()) {
            input = "";
            getline(file, input);
            position = 0;
            row++;
        }
    }
public:
    Lexer() {
        position = 0;
        row = 1;
        file.open("D:\\myCompiler\\PascalCode.txt");
        getline(file, input);
    }
    ~Lexer() {
        file.close();
    }
    void print_lex_errors() {
        cout << exceptions;
    }
    pair<int, int> get_token_cords() 
    {
        return pair<int, int>(row, position);
    }
    std::unique_ptr<Token> getNextToken() {
        Read_input();
        while (position < input.size() && isspace(input[position])) position++; // skip space 
        if (position >= input.size()) {
            return nullptr;
        }
        char currentChar = input[position];

        if (currentChar == ':') { // special simbols
            position++;
            if (position < input.size() && input[position] == '=') {
                position++;
                auto token = make_unique<SpecialSymbols>();
                token->type = ttSpecialSymbols;
                token->ss = ssAssigment;
                return token;
            }
            else {
                auto token = make_unique<SpecialSymbols>();
                token->type = ttSpecialSymbols;
                token->ss = ssColon;
                return token;
            }
        }
        else if (currentChar == ';') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssSemicolon;
            return token;
        }
        else if (currentChar == ',') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssComma;
            return token;
        }
        else if (currentChar == '.') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssDot;
            return token;
        }
        else if (currentChar == '(') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssLeftCurveBrascet;
            return token;
        }
        else if (currentChar == ')') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssRightCurveBrascet;
            return token;
        }
        else if (currentChar == '=') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssEqual;
            return token;
        }
        else if (currentChar == '+') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssPlus;
            return token;
        }
        else if (currentChar == '-') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssMinus;
            return token;
        }
        else if (currentChar == '*') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssMult;
            return token;
        }
        else if (currentChar == '/') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssDiv;
            return token;
        }
        else if (currentChar == '>') {
            position++;
            auto token = make_unique<SpecialSymbols>();
            token->type = ttSpecialSymbols;
            token->ss = ssGreater;
            return token;
        }
        else if (currentChar == '<') {
            position++;
            if (position < input.size() && input[position] == '>') {
                position++;
                auto token = make_unique<SpecialSymbols>();
                token->type = ttSpecialSymbols;
                token->ss = ssNoEqual;
                return token;
            }
            else {
                auto token = make_unique<SpecialSymbols>();
                token->type = ttSpecialSymbols;
                token->ss = ssLesser;
                return token;
            }
        }
        else if (isalpha(currentChar)) { // kw, ident
            string identifier;
            auto kwToken = make_unique<KeyWordToken>();
            kwToken->type = ttKeywords;
            while (position < input.size() && (isalpha(input[position]) || isdigit(input[position])))
                identifier += input[position++];

            if (identifier == "program") {
                kwToken->kw = kwProgram;
                return kwToken;
            }
            else if (identifier == "if") {
                kwToken->kw = kwIf;
                return kwToken;
            }
            else if (identifier == "then") {
                kwToken->kw = kwThen;
                return kwToken;
            }
            else if (identifier == "begin") {
                kwToken->kw = kwBegin;
                return kwToken;
            }
            else if (identifier == "end") {
                kwToken->kw = kwEnd;
                return kwToken;
            }
            else if (identifier == "writeln") {
                kwToken->kw = kwWriteln;
                return kwToken;
            }
            else if (identifier == "program") {
                kwToken->kw = kwProgram;
                return kwToken;
            }
            else if (identifier == "print") {
                kwToken->kw = kwPrint;
                return kwToken;
            }
            else if (identifier == "var") {
                kwToken->kw = kwVar;
                return kwToken;
            }
            else if (identifier == "else") {
                kwToken->kw = kwElse;
                return kwToken;
            }
            else if (identifier == "while") {
                kwToken->kw = kwWhile;
                return kwToken;
            }
            else if (identifier == "do") {
                kwToken->kw = kwDo;
                return kwToken;
            }
            else if (identifier == "for") {
                kwToken->kw = kwFor;
                return kwToken;
            }
            else if (identifier == "to") {
                kwToken->kw = kwTo;
                return kwToken;
            }
            else if (identifier == "downto") {
                kwToken->kw = kwTo;
                return kwToken;
            }
            else if (identifier == "until") {
                kwToken->kw = kwUntil;
                return kwToken;
            }
            else if (identifier == "and") {
                kwToken->kw = kwUntil;
                return kwToken;
            }
            else if (identifier == "or") {
                kwToken->kw = kwUntil;
                return kwToken;
            }
            else if (identifier == "not") {
                kwToken->kw = kwUntil;
                return kwToken;
            }
            auto idToken = make_unique<IdentToken>();
            idToken->type = ttIdentifier;
            idToken->ident = identifier;
            return idToken;
        }
        else if (isdigit(currentChar) || currentChar == '\'') { // constatnts
            string number;
            try
            {
                while (position < input.size() && (isdigit(input[position]) || input[position] == '.')) {
                    number += input[position++];
                }
            }
            catch (const std::exception& exp)
            {
                exceptions += exp.what();
                exceptions += " Row position: " + to_string(row) + "  Position in line: " + to_string(position) + '\n';
            }
            if (number.find('.') != string::npos) {
                try
                {
                    auto realToken = make_unique<ConstToken>(); // real
                    if (count(number.begin(), number.end(), '.') > 1) throw exception("Lex error in code!");
                    else {
                        realToken->type = ttConstants;
                        realToken->data = number;
                        return realToken;
                    }
                }
                catch (const std::exception& exp)
                {
                    exceptions += exp.what();
                    exceptions += " Row position: " + to_string(row) + "  Position in line: " + to_string(position - number.length() + 1) + '\n';
                }
            }
            else if (currentChar == '\'') { // string
                position++;
                while (position < input.size() && input[position] != '\'')
                    number += input[position++];
                try
                {
                    if (position == input.size() && input[position] != '\'')
                        throw exception("Lex error in code!");
                }
                catch (const std::exception& exp)
                {
                    exceptions += exp.what();
                    exceptions += " Row position: " + to_string(row) + "  Position in line: " + to_string(position) + '\n';
                }

                auto stringToken = make_unique<ConstToken>(); // string
                stringToken->type = ttConstants;
                stringToken->data = number;
                position++;
                return stringToken;
            }
            else {
                auto intToken = make_unique<ConstToken>(); // int
                intToken->type = ttConstants;
                intToken->data = stoi(number);
                return intToken;
            }
        }
        auto token = make_unique<Token>();
        token->type = UNKNOWN;
        return token;
    }
};
