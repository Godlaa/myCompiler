#pragma once
#include <string>
#include <iostream>
using namespace std;

enum class TokenType {
    INTEGER,        // int numbers
    REAL,           // float numbers
    BOOLEAN,        // states true or false.
    CHAR,           // characters
    STRING_LITERAL, // such as 'hello world'

    IDENTIFIER,     // such as abc
    KEYWORDS,       // such as if
    OPERATORS,      // such as  + - * /
    DELIMITER,      // such as ,
    END_OF_FILE,    // end of file
    UNKNOWN,
};
enum class TokenIdent {
    AND,
    FOR,
    TO,
    DOWNTO,
    DO,
    IF,
    THEN,
    ELSE,
    WHILE,
    REPEAT,
    UNTIL,
    FUNCTION,
    PROCEDURE,
    BEGIN,
    END,
    PROGRAM,
    FORWARD,
    GOTO,
    OR,
    NOT,
    CASE,
    OTHERWISE,
    WITH,
    IN,

    // I/O routine
    WRITE,
    WRITELN,
    READ,
    READLN,

    // type/var/const
    TYPE,
    VAR,
    PACKED,
    ARRAY,
    OF,
    RECORD,
    CONST,
    FILE,
    SET,
    STRING,
    NIL,

    // symbols
    LEFT_PAREN,        // (
    RIGHT_PAREN,       // )
    LEFT_SQUARE,       // [
    RIGHT_SQUARE,      // ]
    PLUS,              // +
    MINUS,             // -
    MULTIPLY,          // *
    DIVIDE,            // /
    COMMA,             // ,
    SEMICOLON,         // ;
    COLON,             // :
    ASSIGN,            // :=
    PERIOD,            // .
    DOT_DOT,           // ..
    UPARROW,           // ^
    DIV,               // div
    MOD,               // mod
    XOR,               // xor
    SHR,               // shr
    SHL,               // shl

    // comparation symbols
    LESS_OR_EQUAL,     // <=
    LESS_THAN,         // <
    GREATER_OR_EQUAL,  // >=
    GREATER_THAN,      // >
    EQUAL,             // =
    NOT_EQUAL,         // <>

    UNRESERVED
};

class ÑToken{
public:
	TokenType type;
	virtual void Print() { };
};

class CIdent : public ÑToken {
	TokenIdent Ident;
    CIdent(TokenIdent Ident) {

        this->Ident = Ident;
        this->type = which_type();
    }
    static TokenType which_type(string& Ident) {

        // types
        if (Ident == "integer")
        {
            return TokenType::INTEGER;
        }
        if (Ident == "real")
        {
            return TokenType::REAL;
        }
        if (Ident == "boolean")
        {
            return TokenType::BOOLEAN;
        }
        if (Ident == "char")
        {
            return TokenType::CHAR;
        }
        if (Ident == "string literal")
        {
            return TokenType::STRING_LITERAL;
        }
        if (Ident == "identifier")
        {
            return TokenType::IDENTIFIER;
        }
        if (Ident == "keywords")
        {
            return TokenType::KEYWORDS;
        }
        if (Ident == "operators")
        {
            return TokenType::OPERATORS;
        }
        if (Ident == "delimiter")
        {
            return TokenType::DELIMITER;
        }
        if (Ident == "eof")
        {
            return TokenType::END_OF_FILE;
        }
        return TokenType::IDENTIFIER;
    }
    static TokenIdent which_ident(string &Ident) {
        // relationship operators // equal operators
        if (Ident == "<=")
        {
            return TokenIdent::LESS_OR_EQUAL;
        }
        if (Ident == "<")
        {
            return TokenIdent::LESS_THAN;
        }
        if (Ident == ">=")
        {
            return TokenIdent::GREATER_OR_EQUAL;
        }
        if (Ident == ">")
        {
            return TokenIdent::GREATER_THAN;
        }
        if (Ident == "=")
        {
            return TokenIdent::EQUAL;
        }
        if (Ident == "<>")
        {
            return TokenIdent::NOT_EQUAL;
        }

        // math operators
        if (Ident == "+")
            return TokenIdent::PLUS;

        if (Ident == "-")
            return TokenIdent::MINUS;

        if (Ident == "*")
            return TokenIdent::MULTIPLY;

        if (Ident == "/")
            return TokenIdent::DIVIDE;

        // brackets
        if (Ident == "(")
            return TokenIdent::LEFT_PAREN;
        if (Ident == ")")
            return TokenIdent::RIGHT_PAREN;
        if (Ident == "[")
            return TokenIdent::LEFT_SQUARE;
        if (Ident == "]")
            return TokenIdent::RIGHT_SQUARE;

        // assign
        if (Ident == ":=")
            return TokenIdent::ASSIGN;
    }
    static string token_ident_to_string(TokenIdent ident) {
        switch (ident)
        {
        case TokenIdent::AND:
            return "AND";
            break;
        case TokenIdent::FOR:
            return "FOR";
            break;
        case TokenIdent::TO:
            return "TO";
            break;
        case TokenIdent::DOWNTO:
            return "DOWNTO";
            break;
        case TokenIdent::DO:
            return "DO";
            break;
        case TokenIdent::IF:
            return "IF";
            break;
        case TokenIdent::THEN:
            return "THEN";
            break;
        case TokenIdent::ELSE:
            return "ELSE";
            break;
        case TokenIdent::WHILE:
            return "WHILE";
            break;
        case TokenIdent::REPEAT:
            return "REPEAT";
            break;
        case TokenIdent::UNTIL:
            return "UNTIL";
            break;
        case TokenIdent::FUNCTION:
            return "FUNCTION";
            break;
        case TokenIdent::PROCEDURE:
            return "PROCEDURE";
            break;
        case TokenIdent::BEGIN:
            return "BEGIN";
            break;
        case TokenIdent::END:
            return "END";
            break;
        case TokenIdent::PROGRAM:
            return "PROGRAM";
            break;
        case TokenIdent::FORWARD:
            return "FORWARD";
            break;
        case TokenIdent::GOTO:
            return "GOTO";
            break;
        case TokenIdent::OR:
            return "OR";
            break;
        case TokenIdent::NOT:
            return "NOT";
            break;
        case TokenIdent::CASE:
            return "CASE";
            break;
        case TokenIdent::OTHERWISE:
            return "OTHERWISE";
            break;
        case TokenIdent::WITH:
            return "WITH";
            break;
        case TokenIdent::IN:
            return "IN";
            break;
        case TokenIdent::WRITE:
            return "WRITE";
            break;
        case TokenIdent::WRITELN:
            return "WRITELN";
            break;
        case TokenIdent::READ:
            return "READ";
            break;
        case TokenIdent::READLN:
            return "READLN";
            break;
        case TokenIdent::TYPE:
            return "TYPE";
            break;
        case TokenIdent::VAR:
            return "VAR";
            break;
        case TokenIdent::PACKED:
            return "PACKED";
            break;
        case TokenIdent::ARRAY:
            return "ARRAY";
            break;
        case TokenIdent::OF:
            return "OF";
            break;
        case TokenIdent::RECORD:
            return "RECORD";
            break;
        case TokenIdent::CONST:
            return "CONST";
            break;
        case TokenIdent::FILE:
            return "FILE";
            break;
        case TokenIdent::SET:
            return "SET";
            break;
        case TokenIdent::STRING:
            return "STRING";
            break;
        case TokenIdent::NIL:
            return "NIL";
            break;
        case TokenIdent::LEFT_PAREN:
            return "LEFT_PAREN";
            break;
        case TokenIdent::RIGHT_PAREN:
            return "RIGHT_PAREN";
            break;
        case TokenIdent::LEFT_SQUARE:
            return "LEFT_SQUARE";
            break;
        case TokenIdent::RIGHT_SQUARE:
            return "RIGHT_SQUARE";
            break;
        case TokenIdent::PLUS:
            return "PLUS";
            break;
        case TokenIdent::MINUS:
            return "MINUS";
            break;
        case TokenIdent::MULTIPLY:
            return "MULTIPLY";
            break;
        case TokenIdent::DIVIDE:
            return "DIVIDE";
            break;
        case TokenIdent::COMMA:
            return "COMMA";
            break;
        case TokenIdent::SEMICOLON:
            return "SEMICOLON";
            break;
        case TokenIdent::COLON:
            return "COLON";
            break;
        case TokenIdent::ASSIGN:
            return "ASSIGN";
            break;
        case TokenIdent::PERIOD:
            return "PERIOD";
            break;
        case TokenIdent::DOT_DOT:
            return "DOT_DOT";
            break;
        case TokenIdent::UPARROW:
            return "UPARROW";
            break;
        case TokenIdent::DIV:
            return "DIV";
            break;
        case TokenIdent::MOD:
            return "MOD";
            break;
        case TokenIdent::XOR:
            return "XOR";
            break;
        case TokenIdent::SHR:
            return "SHR";
            break;
        case TokenIdent::SHL:
            return "SHL";
            break;
        case TokenIdent::LESS_OR_EQUAL:
            return "LESS_OR_EQUAL";
            break;
        case TokenIdent::LESS_THAN:
            return "LESS_THAN";
            break;
        case TokenIdent::GREATER_OR_EQUAL:
            return "GREATER_OR_EQUAL";
            break;
        case TokenIdent::GREATER_THAN:
            return "GREATER_THAN";
            break;
        case TokenIdent::EQUAL:
            return "EQUAL";
            break;
        case TokenIdent::NOT_EQUAL:
            return "NOT_EQUAL";
            break;
        case TokenIdent::UNRESERVED:
            return "UNRESERVED";
            break;
        default:
            return "";
        }
    }
	void Print() { std :: cout << token_ident_to_string(Ident); }
};

