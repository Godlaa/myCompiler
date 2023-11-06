#pragma once
#include <string>
#include <iostream>
#include <variant>
#include <fstream>
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
    kwPrint,
    kwVar,
};

enum eSpecialSumbols {
    ssComma,
    ssDot,
    ssRightCurveBrascet,
    ssLeftCurveBrascet,
    ssEqual,
    ssSemicolon,
    ssAssigment,
    ssColon,

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
        case kwVar:
            cout << "kwVar" << ' ';
            break;
        case kwPrint:
            cout << "kwPrint" << ' ';
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
        case ssAssigment:
            cout << "ssAssigment" << ' ';
            break;
        case ssColon:
            cout << "ssColon" << ' ';
            break;
        }
    }
};



