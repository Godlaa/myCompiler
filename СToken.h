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
    // Пропускаем пробелы и переводы строк
    while (currentPosition < sourceCode.length() && (sourceCode[currentPosition] == ' ' || sourceCode[currentPosition] == '\n'))
        currentPosition++;

    if (currentPosition >= sourceCode.length()) {
        //  токен "конец файла"
        SpecialSymblos *token = new SpecialSymblos();
        token -> type = eTokenType::ttSpecialSymblos;
        token -> ss = eSpecialSumbols::ssDot;  // Пусть точка будет обозначением конца файла
        return token;
    }

    // Распознавание токенов
    char currentChar = sourceCode[currentPosition];

    // Распознаем числа
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

    // Добавь распознавание других типов токенов (идентификаторы, операторы, ключевые слова, специальные символы, типы и т. д.) здесь

    // Если не удалось распознать токен, вернем ошибку
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


