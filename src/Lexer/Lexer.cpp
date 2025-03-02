//
// Created by saumonbro on 2/19/25.

#include <Lexer/Lexer.h>
#include <functional>
#include <iostream>

Lexer::Lexer(std::istream &input)
    : m_stream(input)
{}

Token Lexer::Pop()
{
    if (m_currentToken.Type == TOKEN_NONE)
    {
        ComputeNextToken();
    }
    const Token res = m_currentToken;
    m_currentToken.Type = TOKEN_NONE;
    return res;
}

Token Lexer::Peek()
{
    if (m_currentToken.Type == TOKEN_NONE)
    {
        ComputeNextToken();
    }
    return m_currentToken;
}

char Lexer::CurrentChar() const
{
    int tmp = m_stream.peek();
    return static_cast<char>(tmp);
}

char Lexer::Advance() const
{
    int tmp = m_stream.get();
    std::cout << "Advancing and got " << std::string{ static_cast<char>(tmp) }
              << std::endl;
    return static_cast<char>(tmp);
}

void Lexer::SkipWhile(const std::function<bool(int)> &predicate) const
{
    while (predicate(m_stream.peek()))
        m_stream.get();
}

std::string Lexer::ReadWhile(const std::function<bool(int)> &predicate) const
{
    std::string res;
    while (predicate(m_stream.peek()))
        res += std::string{ static_cast<char>(m_stream.get()) };
    return res;
}

void Lexer::SkipWhitespaces() const
{
    SkipWhile(std::function<bool(int)>(
        [](const char c) -> bool { return std::isspace(c); }));
}

void Lexer::SkipComments() const
{
    SkipWhile(std::function<bool(int)>(
        [](const char c) -> bool { return c != '\n'; }));
}

std::unordered_map<char, std::string> m_operatorFollow = {
    { '=', "=" },  { '!', "=" },  { '+', "+=" }, { '-', "-=" },
    { '<', "<=" }, { '>', ">=" }, { '&', "&" },  { '|', "|" },
};

std::unordered_map<std::string, TokenType> m_operatorTypes = {
    { "+", TOKEN_PLUS },       { "-", TOKEN_MINUS },
    { "*", TOKEN_MUL },        { "/", TOKEN_DIV },
    { "%", TOKEN_MOD },        { "^", TOKEN_POW },
    { "<", TOKEN_LESS },       { "<=", TOKEN_LESS_EQUAL },
    { ">", TOKEN_GREATER },    { ">=", TOKEN_GREATER_EQUAL },
    { "=", TOKEN_ASSIGNMENT }, { "!=", TOKEN_NOTEQUAL },
    { "!", TOKEN_NOT },        { "==", TOKEN_EQUAL },
    { "&", TOKEN_REF },        { "&&", TOKEN_AND },
    { "||", TOKEN_OR },

};

std::string Lexer::GetOperator(char first, char next,
                               const std::string &data) const
{
    for (const auto c : data)
    {
        if (c == next)
        {
            Advance();
            return std::string{ first } + next;
        }
    }
    return std::string{ first };
}

std::string Lexer::ReadOperation() const
{
    const char cur = Advance();
    if (const auto it = m_operatorFollow.find(cur);
        it != m_operatorFollow.end()) // Has potential next
    {
        return GetOperator(cur, CurrentChar(), it->second);
    }
    return std::string{ cur };
}

static bool isoperation(char c)
{
    static const std::string s("+-*/%^&|=!");
    return s.find(c) != std::string::npos;
}

void Lexer::SetCurrentToken(TokenType type, const std::string &data = "NULL")
{
    m_currentToken.Data = data;
    m_currentToken.Type = type;
}

int Lexer::ReturnDebug([[maybe_unused]] const std::string &pref = "")
{
    // std::cout << " Built : " << std::string{m_currentToken.Data} << " of type
    // : " << Token::tokenToString(m_currentToken.Type) << std::endl;
    return 0;
}

static const std::unordered_map<char, TokenType> SpecialCharacters = {
    { '{', TOKEN_LEFT_BRACKET }, { '}', TOKEN_RIGHT_BRACKET },
    { '(', TOKEN_LEFTPAR },      { ')', TOKEN_RIGHTPAR },
    { ';', TOKEN_SEMICOLON },    { ',', TOKEN_SEMICOLON },
};

static const std::unordered_map<std::string, TokenType> Keywords = {
    { "set", TOKEN_SET },      { "if", TOKEN_IF },
    { "else", TOKEN_ELSE },    { "return", TOKEN_RETURN },
    { "while", TOKEN_WHILE },  { "static", TOKEN_STATIC },
    { "int", TOKEN_INT_TYPE }, { "string", TOKEN_STRING_TYPE },

};

static bool isSpecialChar(const char c)
{
    return SpecialCharacters.find(c) != SpecialCharacters.end();
}

static bool isKeyword(const std::string &s)
{
    return Keywords.find(s) != Keywords.end();
}

int Lexer::ComputeNextToken()
{
    SkipWhitespaces();
    if (CurrentChar() == m_stream.eof())
    {
        SetCurrentToken(TOKEN_EOF);
        return ReturnDebug("EOF");
    }
    char comment = CurrentChar();
    while (comment == '#')
    {
        SkipComments();
        SkipWhitespaces();
        comment = CurrentChar();
        // std::cout << "Of course " << comment << std::endl;
    }
    SkipWhitespaces();
    char c = CurrentChar();

    if (std::isdigit(c))
    {
        const std::string number = ReadWhile(std::function<bool(int)>(
            [](const char c) -> bool { return isdigit(c); }));
        SetCurrentToken(TOKEN_NUMBER, number);
        return ReturnDebug("Number");
    }

    if (std::isalpha(c) || c == '_')
    {
        const std::string number = ReadWhile(std::function<bool(int)>(
            [](const char c) -> bool { return isalpha(c) || c == '_'; }));
        TokenType type = TOKEN_IDENTIFIER;
        if (isKeyword(number))
        {
            type = Keywords.at(number);
        }
        SetCurrentToken(type, number);
        return ReturnDebug("Identifier");
    }

    if (isoperation(c))
    {
        const std::string number = ReadOperation();
        if (m_operatorTypes.find(number) == m_operatorTypes.end())
        {
            throw std::invalid_argument("Operator " + number + " not found");
        }
        // std::cout << "Got operator " << number << std::endl;
        SetCurrentToken(m_operatorTypes.at(number), number);
        return ReturnDebug("Operation");
    }

    if (isSpecialChar(c))
    {
        Advance();
        SetCurrentToken(SpecialCharacters.at(c));
        return ReturnDebug("Special Character : " + c);
    }

    if (c == '"')
    {
        Advance();
        const std::string str = ReadWhile(std::function<bool(int)>(
            [](const char c) -> bool { return c != '"'; }));
        Advance();
        SetCurrentToken(TOKEN_STRING, str);
    }

    SetCurrentToken(TOKEN_EOF);
    return ReturnDebug("FAILURE " + (c));
}
