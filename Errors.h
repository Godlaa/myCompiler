#pragma once
#define keyword_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected another key word\n").c_str()

#define spec_symbol_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected spec symbol\n").c_str()

#define token_type_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected another type\n").c_str()

#define ident_err ("Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " expected ident\n").c_str()

#define compatible_error cout << "Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " compatible_error\n";

#define no_op_between cout << "Row position: " + to_string(lexer->get_token_cords().first) + "  Position in line: " + to_string(lexer->get_token_cords().second) + " no_op_between\n";
	
