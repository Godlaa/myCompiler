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
    kwProgram,
    kwEnd,
    kwWriteln,
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
        cout << ident << ' ';
    };
};

class KeyWordToken : public Token {
public:
    eKeyWords kw;
    void Print() {
        switch (kw)
        {
        case kwIf:
            cout << "kwIf" << ' ';
            break;
        case kwThen:
            cout << "kwThen" << ' ';
            break;
        case kwBegin:
            cout << "kwBegin" << ' ';
            break;
        case kwProgram:
            cout << "kwProgram" << ' ';
            break;
        case kwEnd:
			cout << "kwEnd" << ' ';
			break;
        case kwWriteln:
            cout << "kwWriteln" << ' ';
            break;
        }
    };
};

class ConstToken : public Token {
public:
    std::variant<int, float, std::string, bool> data;
    void Print() { 
        if (data.index() == vtInt) {
			cout << std::get<int>(data) << ' ';
		}
        else if (data.index() == vtReal) {
			cout << std::get<float>(data) << ' ';
		}
        else if (data.index() == vtString) {
			cout << std::get<std::string>(data) << ' ';
		}
        else if (data.index() == vtBool) {
			cout << std::get<bool>(data) << ' ';
		}
    };
};

class SpecialSymblos : public Token {
public:
    eSpecialSumbols ss;
    void Print() { 
        switch (ss)
        {
        case ssComma:
            cout << "ssComma" << ' ';
            break;
        case ssDot:
            cout << "ssDot" << ' ';
            break;
        case ssRightCurveBrascet:
            cout << "ssRightCurveBrascet" << ' ';
            break;
        case ssLeftCurveBrascet:
            cout << "ssLeftCurveBrascet" << ' ';
            break;
        case ssEqual:
            cout << "ssEqual" << ' ';
            break;
        case ssSemicolon:
            cout << "ssSemicolon" << ' ';
            break;
        }
    }
};

Token* getNextToken(size_t& position, const string& input) {
    while (position < input.size() && isspace(input[position])) // skip space
        position++;

    if (position >= input.size()){
        Token *token = new Token();
        token->type = UNKNOWN;
        return token;
    }
    char currentChar = input[position];

    if (currentChar == ':') { // special simbols
        position++;
        if (position < input.size() && input[position] == '=') {
            position++;
            SpecialSymblos* token = new SpecialSymblos();
            token->type = ttSpecialSymblos;
            token->ss = ssEqual;
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
    else if (currentChar == ',') {
        position++;
        SpecialSymblos* token = new SpecialSymblos();
        token->type = ttSpecialSymblos;
        token->ss = ssComma;
        return token;
    }
    else if (currentChar == '.') {
        position++;
        SpecialSymblos* token = new SpecialSymblos();
        token->type = ttSpecialSymblos;
        token->ss = ssDot;
        return token;
    }
    else if (currentChar == '(') {
        position++;
        SpecialSymblos* token = new SpecialSymblos();
        token->type = ttSpecialSymblos;
        token->ss = ssLeftCurveBrascet;
        return token;
    }
    else if (currentChar == ')') {
        position++;
        SpecialSymblos* token = new SpecialSymblos();
        token->type = ttSpecialSymblos;
        token->ss = ssRightCurveBrascet;
        return token;
    }

    else if (isalpha(currentChar)) { // kw, ident
        string identifier;
        while (position < input.size() && (isalpha(input[position]) || isdigit(input[position])))
            identifier += input[position++];

        if (identifier == "program") {
            KeyWordToken* kwToken = new KeyWordToken();
            kwToken->type = ttKeywords;
            kwToken->kw = kwProgram;
            return kwToken;
        }
        else if (identifier == "if") {
			KeyWordToken* kwToken = new KeyWordToken();
			kwToken->type = ttKeywords;
			kwToken->kw = kwIf;
			return kwToken;
		}
		else if (identifier == "then") {
			KeyWordToken* kwToken = new KeyWordToken();
			kwToken->type = ttKeywords;
			kwToken->kw = kwThen;
			return kwToken;
		}
		else if (identifier == "begin") {
			KeyWordToken* kwToken = new KeyWordToken();
			kwToken->type = ttKeywords;
			kwToken->kw = kwBegin;
			return kwToken;
		}
        else if (identifier == "end") {
            KeyWordToken* kwToken = new KeyWordToken();
            kwToken->type = ttKeywords;
            kwToken->kw = kwEnd;
            return kwToken;
        }
        else if (identifier == "writeln") {
            KeyWordToken* kwToken = new KeyWordToken();
            kwToken->type = ttKeywords;
            kwToken->kw = kwWriteln;
            return kwToken;
        }
        IdentToken *idToken = new IdentToken();
        idToken->type = ttIdentifier;
        idToken->ident = identifier;
        return idToken;
    }
    else if (isdigit(currentChar) || currentChar == '\'') { // constatnts
        string number;
        while (position < input.size() && (isdigit(input[position]) || input[position] == '.'))
            number += input[position++];

        if (number.find('.') != string::npos) {
            ConstToken *realToken = new ConstToken();
            realToken->type = ttConstants;
            realToken->data = stof(number);
            return realToken;
        }
        else if(currentChar == '\'') { // string
            position++;
            while (position < input.size() && input[position] != '\'')
                number += input[position++];
            ConstToken* stringToken = new ConstToken();
            stringToken->type = ttConstants;
            stringToken->data = number;
            position++;
            return stringToken;
        }
        else { 
        ConstToken* intToken = new ConstToken();
        intToken->type = ttConstants;
        intToken->data = stoi(number);
        return intToken;
        }
    }

    Token* token = new Token();
    token->type = UNKNOWN;
    return token;
}

