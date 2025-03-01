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

        std::string ReadWhile(const std::function<bool(int)> &predicate) const;

        void SkipWhitespaces() const;

        void SkipComments() const;


        std::string ReadOperation() const;

    private:
    //std::unordered_map<char, std::string> m_operatorFollow;
   // std::unordered_map<std::string, TokenType> m_operatorTypes;

    char CurrentChar() const;

    char Advance() const;


        void SetCurrentToken(TokenType type, const std::string &data);

        int ReturnDebug(std::string pref);

        int ComputeNextToken();

        std::istream& m_stream;

        Token m_currentToken;


};



#endif //LEXER_H
