#pragma once
#include <string>
#include <iostream>
#include <variant>
using namespace std;

enum eTokenType {
    ttConstants, // Constants int float ...
    ttIdentifier,     // such as abc
    ttKeywords,       // such as if
    ttSpecialSymblos,  // such as  + - * / ( ) [ ] { } ...
    UNKNOWN,
};
enum eKeyWords {
    kwIf,
    kwThen,
    kwBegin,
};
enum eSpecialSumbols {
    ssComma,
    ssDot,
    ssRightCurveBrascet,
    ssLeftCurveBrascet,
    ssEqual,
    ssSemicolon,
};
enum eVariantType {
    vtInt,
    vtReal,
    vtString,
    vtBool
};

class Token {
public:
    eTokenType type;
    virtual void Print() {  };
};

class IdentToken : public Token {
public:
    string ident;
    void Print( ) { 
        cout << ident << endl;
    };
};

class KeyWordToken : public Token {
    eKeyWords kw;
    void Print() { 
        switch (kw)
        {
        case kwIf:
            cout << "kwIf";
        }
    };
};

class ConstToken : public Token {
public:
    std::variant<int, float, std::string, bool> data;
    void Print() { 

    };
};

class SpecialSymblos : public Token {
public:
    eSpecialSumbols ss;
    void Print() { 
        switch (ss)
        {
        case ssComma:
            cout << "ssComma" << endl;
            break;
        case ssDot:
            cout << "ssDot" << endl;
            break;
        case ssRightCurveBrascet:
            cout << "ssRightCurveBrascet" << endl;
            break;
        case ssLeftCurveBrascet:
            cout << "ssLeftCurveBrascet" << endl;
            break;
        case ssEqual:
            cout << "ssEqual" << endl;
            break;
        case ssSemicolon:
            cout << "ssSemicolon" << endl;
            break;
        }
    }
};

Token* getNextToken(size_t& position, const string& input) {
    while (position < input.size() && isspace(input[position]))
        position++;

    if (position >= input.size()){
        Token *token = new Token();
        token->type = UNKNOWN;
        return token;
    }

    char currentChar = input[position];

    if (currentChar == ':') {
        position++;
        if (position < input.size() && input[position] == '=') {
            position++;
            SpecialSymblos *token = new SpecialSymblos();
            token -> type = ttSpecialSymblos;
            token -> ss = ssEqual;
            return token;
        }
    }
    else if (currentChar == ';') {
        position++;
        SpecialSymblos *token = new SpecialSymblos();
        token -> type = ttSpecialSymblos;
        token -> ss = ssSemicolon;
        return token;
    }
    else if (isalpha(currentChar)) {
        // Идентификатор начинается с буквы
        string identifier;
        while (position < input.size() && (isalpha(input[position]) || isdigit(input[position])))
            identifier += input[position++];

        IdentToken *idToken = new IdentToken();
        idToken->type = ttIdentifier;
        idToken->ident = identifier;
        return idToken;
    }
    else if (isdigit(currentChar)) {
        // Числовая константа - может быть целочисленной или вещественной
        string number;
        while (position < input.size() && (isdigit(input[position]) || input[position] == '.'))
            number += input[position++];

        if (number.find('.') != string::npos) {
            ConstToken *realToken = new ConstToken();
            realToken->type = ttConstants;
            realToken->data = stof(number);
            return realToken;
        }
        else {
            ConstToken *intToken = new ConstToken();
            intToken->type = ttConstants;
            intToken->data = stoi(number);
            return intToken;
        }
    }

    Token* token = new Token();
    token->type = UNKNOWN;
    return token;
}

