//
// Created by saumonbro on 2/19/25.
//

#include "Parser.h"

#include <iostream>
#include <map>

#include "AstAssignment.h"
#include "AstBinary.h"
#include "AstBlock.h"
#include "Function.h"
#include "AstIdentifier.h"
#include "AstLiteral.h"
#include "AstUnary.h"
#include "Program.h"

Parser::Parser(std::istream &input) : m_lexer(input), m_context(new std::map<std::string, std::unique_ptr<Ast>>())
{
}


std::unique_ptr<Ast> Parser::Factor()
{
    if (CurrentToken().Type == TOKEN_IDENTIFIER) {
        auto tmp = Eat(TOKEN_IDENTIFIER);
        (*m_context)[tmp] = nullptr;
        return std::make_unique<AstIdentifier>(tmp.data() ,m_context);
    }
    if (CurrentToken().Type == TOKEN_NUMBER) {
     return std::make_unique<AstLiteral>(std::atol(Eat(TOKEN_NUMBER).data()));
    }
    if (CurrentToken().Type == TOKEN_LEFTPAR) {
        Eat(TOKEN_LEFTPAR);
        auto res = Assignment();
        Eat(TOKEN_RIGHTPAR);
        return res;
    }
    if (CurrentToken().Type == TOKEN_PLUS || CurrentToken().Type == TOKEN_MINUS) {
        auto type = CurrentToken().Type;
        Eat(type);
        return std::make_unique<AstUnary>(Factor(), type);
    }


    throw std::runtime_error("Unexpected token in factor : " + Token::tokenToString(CurrentToken().Type));
}

std::unique_ptr<Ast> Parser::Pow()
{
    std::unique_ptr<Ast> res = Factor();
    if (CurrentToken().Type == TOKEN_POW)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Pow()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Mul()
{
    std::unique_ptr<Ast> res = Pow();
    while (CurrentToken().Type == TOKEN_DIV || CurrentToken().Type == TOKEN_MUL || CurrentToken().Type == TOKEN_MOD)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Pow()), op);
    }
    return res;
}


std::unique_ptr<Ast> Parser::Expr()
{
    std::unique_ptr<Ast> res = Mul();
    while (CurrentToken().Type == TOKEN_PLUS || CurrentToken().Type == TOKEN_MINUS)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Mul()), op);
    }
    return res;
}


std::unique_ptr<Ast> Parser::And()
{

    std::unique_ptr<Ast> res = Expr();
    while (CurrentToken().Type == TOKEN_AND)
    {
        TokenType op = CurrentToken().Type;
        Eat(TOKEN_AND);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Expr()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Or()
{
    std::unique_ptr<Ast> res = And();
    while (CurrentToken().Type == TOKEN_OR)
    {
        TokenType op = CurrentToken().Type;
        Eat(TOKEN_OR);
        res = std::make_unique<AstBinary>(std::move(res), std::move(And()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Assignment()
{
    if (CurrentToken().Type == TOKEN_SET) {
        Eat(TOKEN_SET);
        std::string id = CurrentToken().Data;
        Eat(TOKEN_IDENTIFIER);
        Eat(TOKEN_EQUAL);
        return std::make_unique<AstAssignment>(id, Assignment(), m_context);
    }
    return Or();
}

std::shared_ptr<Function> Parser::ParseFunction()
{
    // Read args
    std::cout << Token::tokenToString(CurrentToken().Type) << std::endl;
    std::string id = Eat(TOKEN_IDENTIFIER);
    std::vector<std::string> params;
    Eat(TOKEN_LEFTPAR);
    while (CurrentToken().Type != TOKEN_RIGHTPAR) {
        params.push_back(Eat(TOKEN_IDENTIFIER));
    }
    Eat(TOKEN_RIGHTPAR);
    Eat(TOKEN_LEFT_BRACKET);
    std::vector<std::unique_ptr<Ast>> children;

    m_context = std::make_shared<std::map<std::string, std::unique_ptr<Ast>>>();

    while (CurrentToken().Type != TOKEN_RIGHT_BRACKET) {
        children.push_back(Assignment());
        Eat(TOKEN_SEMICOLON);
        if (CurrentToken().Type == TOKEN_RIGHT_BRACKET) {
            Eat(TOKEN_RIGHT_BRACKET);
            break;
        }
    }

    std::shared_ptr<Function> res = std::make_unique<Function>(std::move(id), std::make_unique<AstBlock>(std::move(children)), std::move(params), m_context);
    m_functions[id] = res;
    return res;
}

// Change m_context everytime
std::unique_ptr<Program> Parser::Parse()
{
    std::unordered_map<std::string, std::shared_ptr<Function>> funcs;
    while (CurrentToken().Type != TOKEN_EOF)
    {
        if (CurrentToken().Type == TOKEN_IDENTIFIER) {
            funcs[CurrentToken().Data] = ParseFunction();
        }
        else {
            throw std::runtime_error("Unexpected token for now");
        }
    }

    return std::make_unique<Program>(std::move(funcs));
}

Token Parser::CurrentToken() {
    return m_lexer.Peek();
}

std::unordered_map<std::string, int> Parser::GetOffsets() {
    auto res = std::unordered_map<std::string, int>();
    int offset = 1;
    for (const auto& s : *m_context) {
        std::cout << s.first << " offsrt is " << offset * 8 << std::endl;
        res[s.first] = offset * 8;
        offset++;
    }
    return res;
}

std::string Parser::Eat(const TokenType type)
{
    TokenType curType = CurrentToken().Type;

    auto data = m_lexer.Pop().Data;
    if (curType != type) {
        throw std::invalid_argument("Expected type : " + Token::tokenToString(type) + " but got : "
            + Token::tokenToString(curType) + " with value " + data);
    }
    return data;
}

