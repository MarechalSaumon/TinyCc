//
// Created by saumonbro on 2/19/25.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <unordered_map>

enum TokenType
{
    TOKEN_NONE,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,
    TOKEN_EQUAL,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_GREATER_EQUAL,
    TOKEN_ASSIGNMENT,
    TOKEN_LESS_EQUAL,
    TOKEN_POW,
    TOKEN_NOTEQUAL,
    TOKEN_NOT,
    TOKEN_LEFTPAR,
    TOKEN_RIGHTPAR,
    TOKEN_EOF,
    TOKEN_REF,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SET,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
    TOKEN_WHILE,
    TOKEN_STATIC,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_STRING_TYPE,
    TOKEN_INT_TYPE,
};

struct Token
{
    std::string Data;
    TokenType Type;

public:
    explicit Token(const std::string& data = "", TokenType type = TOKEN_NONE);

    static std::string opToString(TokenType type) {
        static const std::unordered_map<TokenType, std::string> tokenMap = {
            {TOKEN_PLUS, "+"},
            {TOKEN_MINUS, "-"},
            {TOKEN_MUL, "*"},
            {TOKEN_DIV, "/"},
            {TOKEN_MOD, "%"},
            {TOKEN_EQUAL, "=="},
            {TOKEN_GREATER, ">"},
            {TOKEN_LESS, "<"},
            {TOKEN_GREATER_EQUAL, ">="},
            {TOKEN_ASSIGNMENT, "="},
            {TOKEN_LESS_EQUAL, "<="},
            {TOKEN_POW, "^"},
            {TOKEN_NOTEQUAL, "!="},
            {TOKEN_NOT, "!"},
            {TOKEN_REF, "&"},
            {TOKEN_AND, "&&"},
            {TOKEN_OR, "||"},
            };
        return tokenMap.at(type);
    }

    static std::string tokenToString(TokenType token) {
        static const std::unordered_map<TokenType, std::string> tokenMap = {
            {TOKEN_NONE, "TOKEN_NONE"},
            {TOKEN_IDENTIFIER, "TOKEN_IDENTIFIER"},
            {TOKEN_NUMBER, "TOKEN_NUMBER"},
            {TOKEN_PLUS, "TOKEN_PLUS"},
            {TOKEN_MINUS, "TOKEN_MINUS"},
            {TOKEN_MUL, "TOKEN_MUL"},
            {TOKEN_DIV, "TOKEN_DIV"},
            {TOKEN_MOD, "TOKEN_MOD"},
            {TOKEN_EQUAL, "TOKEN_EQUAL"},
            {TOKEN_GREATER, "TOKEN_GREATER"},
            {TOKEN_LESS, "TOKEN_LESS"},
            {TOKEN_GREATER_EQUAL, "TOKEN_GREATER_EQUAL"},
            {TOKEN_ASSIGNMENT, "TOKEN_ASSIGNMENT"},
            {TOKEN_LESS_EQUAL, "TOKEN_LESS_EQUAL"},
            {TOKEN_POW, "TOKEN_POW"},
            {TOKEN_NOTEQUAL, "TOKEN_NOTEQUAL"},
            {TOKEN_NOT, "TOKEN_NOT"},
            {TOKEN_LEFTPAR, "TOKEN_LEFTPAR"},
            {TOKEN_RIGHTPAR, "TOKEN_RIGHTPAR"},
            {TOKEN_EOF, "TOKEN_EOF"},
            {TOKEN_REF, "TOKEN_REF"},
            {TOKEN_AND, "TOKEN_AND"},
            {TOKEN_OR, "TOKEN_OR"},
            {TOKEN_SET, "TOKEN_SET"},
            {TOKEN_LEFT_BRACKET, "TOKEN_LEFT_BRACKET"},
            {TOKEN_RIGHT_BRACKET, "TOKEN_RIGHT_BRACKET"},
            {TOKEN_SEMICOLON, "TOKEN_SEMICOLON"},
            {TOKEN_COMMA, "TOKEN_COMMA"},
            {TOKEN_IF, "TOKEN_IF"},
            {TOKEN_ELSE, "TOKEN_ELSE"},
            {TOKEN_RETURN, "TOKEN_RETURN"},
            {TOKEN_WHILE, "TOKEN_WHILE"},
            {TOKEN_STATIC, "TOKEN_STATIC"},
            {TOKEN_CHAR, "TOKEN_CHAR"},
            {TOKEN_STRING, "TOKEN_STRING"},
            { TOKEN_STRING_TYPE, "TOKEN_STRING_TYPE"},
            {TOKEN_INT_TYPE, "TOKEN_INT_TYPE"},
        };

        return  tokenMap.at(token);
    }
};



#endif //TOKEN_H
