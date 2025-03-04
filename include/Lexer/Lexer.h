//
// Created by saumonbro on 2/19/25.
//

#ifndef LEXER_H
#define LEXER_H
#include <functional>
#include <istream>
#include <memory>
#include <streambuf>

#include "Token.hpp"

class Lexer
{
public:
    explicit Lexer(std::istream &input);

    Token Pop();
    Token Peek();

    void SkipWhile(const std::function<bool(int)> &predicate) const;

    std::string ReadWhile(const std::function<bool(int)> &predicate);

    std::string ReadWhile(const std::function<bool(int)> &predicate) const;

    void SkipWhitespaces();

    void SkipWhitespaces() const;

    void SkipComments() const;

    std::string GetOperator(char first, char next, const std::string &data);

    std::string ReadOperation();

    std::string GetOperator(char first, char next,
                            const std::string &data) const;

    std::string ReadOperation() const;

    int GetLineNumber() const;

    std::string GetCurrentLine();

    std::string GetCurrentLine() const;


private:
    // std::unordered_map<char, std::string> m_operatorFollow;
    // std::unordered_map<std::string, TokenType> m_operatorTypes;

    [[nodiscard]] char CurrentChar() const;

    char Advance();

    void SetCurrentToken(TokenType type, const std::string &data);

    int ReturnDebug(const std::string &pref);

    int ComputeNextToken();

    std::istream &m_stream;

    Token m_currentToken;
    int currentLine = 1;
    std::string currentLineStr;
};

#endif // LEXER_H
