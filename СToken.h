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
    ttSpecialSymbols,  // such as  + - * / ( ) [ ] { } ...
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
    kwReadln,
    kwElse,
    kwWhile,
    kwDo,
    kwFor,
    kwTo,
    kwToDownTo,
    kwUntil,
};

enum eSpecialSymbols {
    ssComma,
    ssDot,
    ssRightCurveBrascet,
    ssLeftCurveBrascet,
    ssEqual,
    ssSemicolon,
    ssAssigment,
    ssColon,
    ssPlus,
    ssMinus,
    ssDiv,
    ssMult,
    ssNoEqual,
    ssGreater,
    ssLesser,
    ssMod,
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

class SpecialSymbols : public Token {
public:
    eSpecialSymbols ss;
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
        case ssPlus:
            cout << "ssPlus" << ' ';
            break;
        case ssMinus:
            cout << "ssMinus" << ' ';
            break;
        case ssDiv:
            cout << "ssDiv" << ' ';
            break;
        case ssMult:
            cout << "ssMult" << ' ';
            break;
        }
    }
};




