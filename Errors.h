#pragma once
#define keyword_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected another key word\n").c_str()

#define spec_symbol_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected spec symbol\n").c_str()

#define token_type_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected another type\n").c_str()

#define ident_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected ident\n").c_str()

void compatible_error() {
	cout << "compatible_error\n";
}

void no_op_between() {
	cout << "no_op_between\n";
}