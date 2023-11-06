#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ÑToken.h"
class Lexer {
public:
    string exceptions = "";
    std::shared_ptr<Token> getNextToken(size_t& position, size_t& row, const string& input) {
        while (position < input.size() && isspace(input[position])) // skip space
            position++;

        if (position >= input.size()) {
            shared_ptr<Token> token = make_shared<Token>();
            token->type = UNKNOWN;
            return token;
        }
        char currentChar = input[position];

        if (currentChar == ':') { // special simbols
            position++;
            if (position < input.size() && input[position] == '=') {
                position++;
                shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
                token->type = ttSpecialSymblos;
                token->ss = ssAssigment;
                return token;
            }
            else {
                shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
                token->type = ttSpecialSymblos;
                token->ss = ssColon;
                return token;
            }
        }
        else if (currentChar == ';') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssSemicolon;
            return token;
        }
        else if (currentChar == ',') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssComma;
            return token;
        }
        else if (currentChar == '.') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssDot;
            return token;
        }
        else if (currentChar == '(') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssLeftCurveBrascet;
            return token;
        }
        else if (currentChar == ')') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssRightCurveBrascet;
            return token;
        }
        else if (currentChar == '=') {
            position++;
            shared_ptr<SpecialSymblos> token = make_shared<SpecialSymblos>();
            token->type = ttSpecialSymblos;
            token->ss = ssEqual;
            return token;
        }
        else if (isalpha(currentChar)) { // kw, ident
            string identifier;
            shared_ptr<KeyWordToken>  kwToken = make_shared<KeyWordToken>();
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
            shared_ptr<IdentToken> idToken = make_shared<IdentToken>();
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
                    shared_ptr<ConstToken> realToken = make_shared<ConstToken>(); // real
                    if (input[position] != ';' || count(number.begin(), number.end(), '.') > 1) throw exception("Lex error in code!");
                    else {
                        realToken->type = ttConstants;
                        realToken->data = stof(number);
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

                shared_ptr<ConstToken> stringToken = make_shared<ConstToken>(); // string
                stringToken->type = ttConstants;
                stringToken->data = number;
                position++;
                return stringToken;
            }
            else {
                shared_ptr<ConstToken>  intToken = make_shared<ConstToken>(); // int
                intToken->type = ttConstants;
                intToken->data = stoi(number);
                return intToken;
            }
        }
        shared_ptr<Token> token = make_shared<Token>();
        token->type = UNKNOWN;
        return token;
    }
};
