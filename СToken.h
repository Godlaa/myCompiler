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
    virtual void Print() { cout << type; };
};

class IdentToken : public Token {
    string ident;
    void Print() {  };
};

class KeyWordToken : public Token {
    eKeyWords kw;
};

class ConstToken : public Token {
public:
    std::variant<int, float, std::string, bool> data;
};

class SpecialSymblos : public Token {
public:
    eSpecialSumbols ss;
};


Token* getNextToken(int currentPosition, string sourceCode) {
    // ���������� ������� � �������� �����
    while (currentPosition < sourceCode.length() && (sourceCode[currentPosition] == ' ' || sourceCode[currentPosition] == '\n'))
        currentPosition++;

    if (currentPosition >= sourceCode.length()) {
        //  ����� "����� �����"
        SpecialSymblos *token = new SpecialSymblos();
        token -> type = eTokenType::ttSpecialSymblos;
        token -> ss = eSpecialSumbols::ssDot;  // ����� ����� ����� ������������ ����� �����
        return token;
    }

    // ������������� �������
    char currentChar = sourceCode[currentPosition];

    // ���������� �����
    if (isdigit(currentChar)) {
        ConstToken *token = new ConstToken();;
        token->type = eTokenType::ttConstants;
        token->data = (int)sourceCode[currentPosition];

        while (currentPosition < sourceCode.length() && isdigit(sourceCode[currentPosition])) {
            token->data =+ sourceCode[currentPosition];
            currentPosition++;
        }

        return token;
    }

    // ������ ������������� ������ ����� ������� (��������������, ���������, �������� �����, ����������� �������, ���� � �. �.) �����

    // ���� �� ������� ���������� �����, ������ ������
    Token* token = new Token();
    token->type = eTokenType::UNKNOWN;
    return token;
}


//enum class TokenIdent {
//    AND,
//    FOR,
//    TO,
//    DOWNTO,
//    DO,
//    IF,
//    THEN,
//    ELSE,
//    WHILE,
//    REPEAT,
//    UNTIL,
//    FUNCTION, 
//    PROCEDURE,
//    BEGIN,
//    END,
//    PROGRAM,
//    FORWARD,
//    GOTO,
//    OR,
//    NOT,
//    CASE,
//    OTHERWISE,
//    WITH,
//    IN,
//
//    // I/O routine
//    WRITE,
//    WRITELN,
//    READ,
//    READLN,
//
//    // type/var/const
//    TYPE,
//    VAR,
//    PACKED,
//    ARRAY,
//    OF,
//    RECORD,
//    CONST,
//    FILE,
//    SET,
//    STRING,
//    NIL,
//
//    // symbols
//    LEFT_PAREN,        // (
//    RIGHT_PAREN,       // )
//    LEFT_SQUARE,       // [
//    RIGHT_SQUARE,      // ]
//    PLUS,              // +
//    MINUS,             // -
//    MULTIPLY,          // *
//    DIVIDE,            // /
//    COMMA,             // ,
//    SEMICOLON,         // ;
//    COLON,             // :
//    ASSIGN,            // :=
//    PERIOD,            // .
//    DOT_DOT,           // ..
//    UPARROW,           // ^
//    DIV,               // div
//    MOD,               // mod
//    XOR,               // xor
//    SHR,               // shr
//    SHL,               // shl
//
//    // comparation symbols
//    LESS_OR_EQUAL,     // <=
//    LESS_THAN,         // <
//    GREATER_OR_EQUAL,  // >=
//    GREATER_THAN,      // >
//    EQUAL,             // =
//    NOT_EQUAL,         // <>
//
//    UNRESERVED
//};


