//
// Created by saumonbro on 2/19/25.

#include "Lexer.h"
#include <functional>
#include <iostream>

Lexer::Lexer(std::istream &input) : m_stream(input)
{
}

Token Lexer::Pop()
{
    if (m_currentToken.Type ==  TOKEN_NONE) {
        ComputeNextToken();
    }
    const Token res = m_currentToken;
    m_currentToken.Type = TOKEN_NONE;
    return res;
}

Token Lexer::Peek() {
    if (m_currentToken.Type ==  TOKEN_NONE) {
        ComputeNextToken();
    }
    return m_currentToken;
}


char Lexer::CurrentChar() const {
    int tmp = m_stream.peek();
    return static_cast<char>(tmp);
}

char Lexer::Advance() const {
    int tmp = m_stream.get();
   // std::cout << "Advancing and got " << std::string{static_cast<char>(tmp)} << std::endl;
    return static_cast<char>(tmp);
}

void Lexer::SkipWhile(const std::function<bool(int)>& predicate) const
{
    while (predicate(m_stream.peek()))
        m_stream.get();
}

std::string Lexer::ReadWhile(const std::function<bool(int)> &predicate) const
{
    std::string res;
    while (predicate(m_stream.peek()))
        res += std::string{static_cast<char>(m_stream.get())};
    return res;
}

void Lexer::SkipWhitespaces() const {
    SkipWhile(std::function<bool(int)>([] (const char c) -> bool {
        return std::isspace(c);
    }));
}

void Lexer::SkipComments() const {
    SkipWhile(std::function<bool(int)>([] (const char c) -> bool {
        return c != '\n';
    }));
}


std::unordered_map<char, std::string> m_operatorFollow = {
    {'=', "="},
    {'!', "="},
    {'+', "+="},
    {'-', "-="},
    {'<', "<="},
    {'>', ">="},
    {'&', "&"},
    {'|', "|"},
};

std::unordered_map<std::string, TokenType> m_operatorTypes =
{
    {"+", TOKEN_PLUS},
    {"-", TOKEN_MINUS},
    {"*", TOKEN_MUL},
    {"/", TOKEN_DIV},
    {"%", TOKEN_MOD},
    {"^", TOKEN_POW},
    {"<", TOKEN_LESS},
    {"<=", TOKEN_LESS_EQUAL},
    {">", TOKEN_GREATER},
    {">=", TOKEN_GREATER_EQUAL},
    {"=", TOKEN_EQUAL},
    {"!=", TOKEN_NOTEQUAL},
    {"!", TOKEN_NOT},
    {"==", TOKEN_ASSIGNMENT},
    {"&", TOKEN_REF},
    {"&&", TOKEN_AND},
    {"||", TOKEN_OR},

};


std::string GetOperator(char first, char next, const std::string &data) {
    for (const auto c : data)
    {
        if (c == next)
        {
            return std::string {first} + next;
        }
    }
    return std::string {first};
}


std::string Lexer::ReadOperation() const {
    const char cur = Advance();
  //  std::cout << "Cur : " << cur  << ", " << std::endl;
    if (const auto it = m_operatorFollow.find(cur); it != m_operatorFollow.end()) // Has potential next
    {
        return GetOperator(cur, Advance(), it->second);
    }
    return std::string {cur};
}

static bool isoperation(char c) {
    static const std::string s("+-*/%^&|=!");
    return s.find(c) != std::string::npos;
}

void Lexer::SetCurrentToken(TokenType type, const std::string& data = "NULL") {
    m_currentToken.Data = data;
    m_currentToken.Type = type;
}

int Lexer::ReturnDebug(std::string pref = "") {
    std::cout << pref << " Built : " << m_currentToken.Data << " of type : " << Token::tokenToString(m_currentToken.Type) << std::endl;
    return 0;
}

int Lexer::ComputeNextToken() {
    SkipWhitespaces();
    if (CurrentChar() == m_stream.eof()) {
        SetCurrentToken(TOKEN_EOF);
        return ReturnDebug("EOF");
    }
    if (char comment = CurrentChar(); comment == '#')
    {
        SkipComments();
    }
    SkipWhitespaces();
    char c = CurrentChar();


    if (std::isdigit(c)) {
        const std::string number = ReadWhile(std::function<bool(int)>([] (const char c) -> bool {
        return isdigit(c);
    }));
        SetCurrentToken(TOKEN_NUMBER, number);
        return ReturnDebug("Number");
    }

    if (std::isalpha(c)) {
        const std::string number = ReadWhile(std::function<bool(int)>([] (const char c) -> bool {
        return isalpha(c);
    }));
        SetCurrentToken(number == "set" ? TOKEN_SET : TOKEN_IDENTIFIER, number);
        return ReturnDebug("Identifier");
    }

    if (isoperation(c)) {
        const std::string number = ReadOperation();
        if (m_operatorTypes.find(number) == m_operatorTypes.end())
        {
            throw std::invalid_argument("Operator " + number +" not found");
        }
        //std::cout << "Getting " << Token::tokenToString(m_operatorTypes.at(number)) << " From " << number << std::endl;
        SetCurrentToken(m_operatorTypes.at(number), number);
        return ReturnDebug("Operation");
    }
    if (c == '{' || c == '}') {
        SetCurrentToken(c == '{' ? TOKEN_LEFT_BRACKET : TOKEN_RIGHT_BRACKET);
        Advance();
        return ReturnDebug("Bracket");
    }

    if (c == ';') {
        SetCurrentToken(TOKEN_SEMICOLON);
        Advance();
        return ReturnDebug("Semicolon");
    }

    if (c == '(' || c == ')') {
        SetCurrentToken(c == '(' ? TOKEN_LEFTPAR : TOKEN_RIGHTPAR);
        Advance();
        return ReturnDebug("Parenthesis");
    }
    SetCurrentToken(TOKEN_EOF);
    return ReturnDebug("FAILURE " + (c));
}
